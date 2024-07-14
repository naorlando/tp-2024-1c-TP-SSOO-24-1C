#include "protocolo_entradasalida.h"


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

//======================================================
//          FUNCIONES DE ENTRADA/SALIDA DIALFS
//======================================================

void atender_solicitud_dialfs(int fd) {
    t_io_dialfs* io_dialfs = deserializar_io_dialfs(recibir_buffer(&fd, sizeof(int)));
    if (io_dialfs != NULL) {
        bool operacion_exitosa = false;
        
        switch(io_dialfs->operacion) {
            case IO_FS_CREATE:
                operacion_exitosa = crear_archivo(dialfs, io_dialfs->nombre_archivo);
                log_info(logger_entradasalida, "PID: %d - Crear Archivo: %s", io_dialfs->pid, io_dialfs->nombre_archivo);
                break;
            case IO_FS_DELETE:
                operacion_exitosa = eliminar_archivo(dialfs, io_dialfs->nombre_archivo);
                log_info(logger_entradasalida, "PID: %d - Eliminar Archivo: %s", io_dialfs->pid, io_dialfs->nombre_archivo);
                break;
            case IO_FS_TRUNCATE:
                operacion_exitosa = truncar_archivo(dialfs, io_dialfs->nombre_archivo, io_dialfs->tamanio);
                log_info(logger_entradasalida, "PID: %d - Truncar Archivo: %s - Tamaño: %d", io_dialfs->pid, io_dialfs->nombre_archivo, io_dialfs->tamanio);
                break;
            case IO_FS_WRITE:
                operacion_exitosa = escribir_archivo(dialfs, io_dialfs->nombre_archivo, io_dialfs->datos, io_dialfs->tamanio, io_dialfs->offset);
                log_info(logger_entradasalida, "PID: %d - Escribir Archivo: %s - Tamaño a Escribir: %d - Puntero Archivo: %d", io_dialfs->pid, io_dialfs->nombre_archivo, io_dialfs->tamanio, io_dialfs->offset);
                break;
            case IO_FS_READ:
                void* buffer = malloc(io_dialfs->tamanio);
                operacion_exitosa = leer_archivo(dialfs, io_dialfs->nombre_archivo, buffer, io_dialfs->tamanio, io_dialfs->offset);
                if (operacion_exitosa) {
                    enviar_datos_leidos(fd, buffer, io_dialfs->tamanio);
                }
                free(buffer);
                log_info(logger_entradasalida, "PID: %d - Leer Archivo: %s - Tamaño a Leer: %d - Puntero Archivo: %d", io_dialfs->pid, io_dialfs->nombre_archivo, io_dialfs->tamanio, io_dialfs->offset);
                break;
            default:
                log_error(logger_entradasalida, "Operación DialFS no reconocida");
                break;
        }
        
        if (io_dialfs->operacion == IO_FS_READ && operacion_exitosa) {
            // Ya se enviaron los datos leídos, no es necesario enviar confirmación adicional
        } else {
            enviar_confirmacion_io(fd, operacion_exitosa);
        }
        
        destruir_io_dialfs(io_dialfs);
    } else {
        log_error(logger_entradasalida, "Error al recibir IO DialFS");
        enviar_confirmacion_io(fd, false);
    }
    
    // Comprobamos si es necesario realizar una compactación
    if (es_necesario_compactar(dialfs)) {
        log_info(logger_entradasalida, "Inicio Compactación.");
        compactar_fs(dialfs);
        log_info(logger_entradasalida, "Fin Compactación.");
    }
}

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