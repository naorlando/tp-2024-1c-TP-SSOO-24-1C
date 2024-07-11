#include "protocolo_entradasalida.h"
#include "utils/solicitudes_io.h"
#include <readline/readline.h>

//======================================================
//        FUNCIONES DE ENTRADA/SALIDA GENERICA
//======================================================

// Función que atiende una solicitud genérica de entrada/salida, 
// la cual consiste en esperar un tiempo determinado
void atender_solicitud_generica(int fd) {
    t_io_generica *io_generica = recv_io_generica(fd);

    if (io_generica != NULL) {
        log_info(logger_entradasalida, "PID: <%d> - Operacion: <SLEEP>", obtener_pid_generica(io_generica));

        bool resultado = ejecutar_unidades_de_trabajo(io_generica);
        
        log_info(logger_entradasalida, "Operacion IO_GEN_SLEEP finalizada");

        t_response* response = create_response(resultado, obtener_pid_generica(io_generica));
        send_confirmacion_io(fd, MSG_IO_KERNEL_GENERICA, response);

        destruir_io_generica(io_generica);
    } else {
        log_error(logger_entradasalida, "Error al recibir la solicitud IO GENERICA. ABORTANDO");
    }
}

//======================================================
//          FUNCIONES DE ENTRADA/SALIDA STDIN
//======================================================

// Función que atiende una solicitud de entrada/salida STDIN,
// la cual consiste en leer un texto desde la consola y guardarlo en memoria
void atender_solicitud_stdin(int fd) {
    t_io_stdin *io_stdin = recv_io_stdin(fd);

    if (io_stdin != NULL) {
        log_info(logger_entradasalida, "PID: <%d> - Operacion: <READ>", obtener_pid_stdin(io_stdin));

        char *input = readline("Ingrese un texto: ");

        escribir_memoria(io_stdin->direccion_fisica, input, io_stdin->tamanio);
        free(input);

        log_info(logger_entradasalida, "Operacion READ finalizada");

        //TODO: modificar la funcion para que no este harcodeado el 'true'
        t_response* response = create_response(true, obtener_pid_stdin(io_stdin));
        send_confirmacion_io(fd, MSG_IO_KERNEL_STDIN, response);

        destruir_io_stdin(io_stdin);
    } else {
        log_error(logger_entradasalida, "Error al recibir IO STDIN");
    }
}

// Función auxiliar que escribe en memoria el valor recibido
void escribir_memoria(uint32_t direccion_fisica, char *valor, uint32_t tamanio) {
    t_package *package = package_create(MSG_WRITE_MEMORY, sizeof(t_buffer));
    t_buffer *buffer = get_buffer(package);
    buffer_add_uint32(buffer, direccion_fisica);
    buffer_add_uint32(buffer, tamanio);
    buffer_add_string(buffer, valor);
    package_send(package, fd_memoria);
    package_destroy(package);
}

//======================================================
//          FUNCIONES DE ENTRADA/SALIDA STDOUT
//======================================================

// Función que atiende una solicitud de entrada/salida STDOUT,
// la cual consiste en leer un texto desde memoria y mostrarlo por consola
void atender_solicitud_stdout(int fd) {
    t_io_stdout* io_stdout = recv_io_stdout(fd);

    if (io_stdout != NULL) {
        log_info(logger_entradasalida, "PID: <%d> - Operacion: <WRITE>", obtener_pid_stdout(io_stdout));

        char *valor = leer_memoria(io_stdout->direccion_fisica, io_stdout->tamanio);
        printf("%.*s", io_stdout->tamanio, valor);

        free(valor);

        //TODO: modificar la funcion para que no este harcodeado el 'true'
        t_response* response = create_response(true, obtener_pid_stdout(io_stdout));
        send_confirmacion_io(fd, MSG_IO_KERNEL_STDOUT, response);

        log_info(logger_entradasalida, "Operacion WRITE finalizada");

        destruir_io_stdout(io_stdout);
    } else {
        log_error(logger_entradasalida, "Error al recibir IO STDOUT");
    }
}

// Función auxiliar que lee la memoria de la dirección física y devuelve el valor leído
char *leer_memoria(uint32_t direccion_fisica, uint32_t tamanio) {
    t_package *package = package_create(MSG_READ_MEMORY, sizeof(t_buffer));
    t_buffer *buffer = get_buffer(package);
    buffer_add_uint32(buffer, direccion_fisica);
    buffer_add_uint32(buffer, tamanio);
    package_send(package, fd_memoria);
    package_destroy(package);

    t_package *response = package_create(NULL_HEADER, 0);
    package_recv(response, fd_memoria);
    char *valor = strdup(extract_string_buffer(get_buffer(response)));
    package_destroy(response);
    return valor;
}

//======================================================
//          FUNCIONES DE ENTRADA/SALIDA DIALFS
//======================================================

// A implementar!



//======================================================
//               FUNCIONES COMUNES
//======================================================

// Función auxiliar que envía una confirmación de la operación de entrada/salida
void send_confirmacion_io(int fd, t_msg_header header, t_response* response) {
    send_response(fd, header, response);
}

void send_IO_interface_kernel() 
{
    send_IO_interface(fd_kernel, nombre_interfaz, obtener_tipo_interfaz(entradasalida_config));
}