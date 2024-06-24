#include "protocolo_entradasalida.h"
#include "utils/solicitudes_io.h"
#include <readline/readline.h>

//======================================================
//        FUNCIONES DE ENTRADA/SALIDA GENERICA
//======================================================

// Función que atiende una instrucción genérica de entrada/salida, 
// la cual consiste en esperar un tiempo determinado
void atender_instruccion_generica(int fd) {
    t_io_generica *io_generica = deserializar_io_generica(recibir_buffer(&fd, sizeof(int)));
    if (io_generica != NULL) {
        int tiempo_espera = io_generica->tiempo_sleep * obtener_tiempo_unidad_trabajo(entradasalida_config);
        log_info(logger_entradasalida, "Esperando %d milisegundos", tiempo_espera);
        sleep(tiempo_espera);
        log_info(logger_entradasalida, "Operacion IO_GEN_SLEEP finalizada");
        enviar_confirmacion_io(fd);
        destruir_io_generica(io_generica);
    } else {
        log_error(logger_entradasalida, "Error al recibir IO genérica");
    }
}

//======================================================
//          FUNCIONES DE ENTRADA/SALIDA STDIN
//======================================================

// Función que atiende una instrucción de entrada/salida STDIN,
// la cual consiste en leer un texto desde la consola y guardarlo en memoria
void atender_instruccion_stdin(int fd) {
    t_io_stdin *io_stdin = deserializar_io_stdin(recibir_buffer(&fd, sizeof(int)));
    if (io_stdin != NULL) {
        char *input = readline("Ingrese un texto: ");
        escribir_memoria(io_stdin->direccion_fisica, input, io_stdin->tamanio);
        free(input);
        enviar_confirmacion_io(fd);
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

// Función que atiende una instrucción de entrada/salida STDOUT,
// la cual consiste en leer un texto desde memoria y mostrarlo por consola
void atender_instruccion_stdout(int fd) {
    t_io_stdout *io_stdout = deserializar_io_stdout(recibir_buffer(&fd, sizeof(int)));
    if (io_stdout != NULL) {
        char *valor = leer_memoria(io_stdout->direccion_fisica, io_stdout->tamanio);
        printf("%.*s", io_stdout->tamanio, valor);
        free(valor);
        enviar_confirmacion_io(fd);
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
void enviar_confirmacion_io(int fd) {
    t_package *package = package_create(MSG_KERNEL_IO, 0);
    package_send(package, fd);
    package_destroy(package);
}

void send_IO_interface_kernel() 
{
    send_IO_interface(fd_kernel, nombre_interfaz, obtener_tipo_interfaz(entradasalida_config));
}