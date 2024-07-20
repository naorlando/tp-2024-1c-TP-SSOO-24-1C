#include "protocolo_entradasalida.h"

bool operacion_exitosa = false;

//======================================================
//        FUNCIONES DE ENTRADA/SALIDA GENERICA
//======================================================

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
//        FUNCIONES DE ENTRADA/SALIDA STDIN
//======================================================

void atender_solicitud_stdin(int fd) {
    t_io_stdin *io_stdin = recv_io_stdin(fd);

    if (io_stdin != NULL) {
        log_info(logger_entradasalida, "PID: <%d> - Interfaz: <%s> - Operacion: <READ>", 
                 io_stdin->pid, io_stdin->nombre_interfaz);

        char *input = readline("Ingrese un texto: ");

        int resultado = escribir_memoria_stdin(io_stdin->pid, 
                                               io_stdin->direccion_logica, 
                                               io_stdin->tamanio, 
                                               input);
        
        free(input);

        log_info(logger_entradasalida, "Operacion READ finalizada");

        bool operacion_exitosa = (resultado == 0);
        t_response* response = create_response(operacion_exitosa, io_stdin->pid);
        send_confirmacion_io(fd, MSG_IO_KERNEL_STDIN, response);

        destruir_io_stdin(io_stdin);
    } else {
        log_error(logger_entradasalida, "Error al recibir IO STDIN");
    }
}

//======================================================
//        FUNCIONES DE ENTRADA/SALIDA STDOUT
//======================================================

void atender_solicitud_stdout(int fd) {
    t_io_stdout* io_stdout = recv_io_stdout(fd);

    if (io_stdout != NULL) {
        log_info(logger_entradasalida, "PID: <%d> - Interfaz: <%s> - Operacion: <WRITE>", 
                 io_stdout->pid, io_stdout->nombre_interfaz);

        char *valor = leer_memoria_stdout(io_stdout->pid, 
                                          io_stdout->direccion_logica, 
                                          io_stdout->tamanio);
        
        if (valor != NULL) {
            printf("%.*s", io_stdout->tamanio, valor);
            free(valor);

            t_response* response = create_response(true, io_stdout->pid);
            send_confirmacion_io(fd, MSG_IO_KERNEL_STDOUT, response);

            log_info(logger_entradasalida, "Operacion WRITE finalizada");
        } else {
            log_error(logger_entradasalida, "Error al leer de memoria para IO STDOUT");
            t_response* response = create_response(false, io_stdout->pid);
            send_confirmacion_io(fd, MSG_IO_KERNEL_STDOUT, response);
        }

        destruir_io_stdout(io_stdout);
    } else {
        log_error(logger_entradasalida, "Error al recibir IO STDOUT");
    }
}

//======================================================
//        FUNCIONES DE ENTRADA/SALIDA DIALFS
//======================================================

void atender_solicitud_dialfs(int fd)
{
    t_io_dialfs* io_dialfs = recv_io_dialfs(fd);
    
    if (io_dialfs != NULL) {
        log_info(logger_entradasalida, "PID: <%d> - Operacion: <%s>", io_dialfs->pid, get_operation_name(io_dialfs->operacion));
        
        bool operacion_exitosa = false;
        
        switch(io_dialfs->operacion) {
            case IO_FS_CREATE:
                operacion_exitosa = crear_archivo(io_dialfs->nombre_archivo);
                log_info(logger_entradasalida, "PID: %d - Crear Archivo: %s", io_dialfs->pid, io_dialfs->nombre_archivo);
                break;
            case IO_FS_DELETE:
                operacion_exitosa = eliminar_archivo(io_dialfs->nombre_archivo);
                log_info(logger_entradasalida, "PID: %d - Eliminar Archivo: %s", io_dialfs->pid, io_dialfs->nombre_archivo);
                break;
            case IO_FS_TRUNCATE:
                operacion_exitosa = truncar_archivo(io_dialfs->nombre_archivo, io_dialfs->tamanio);
                log_info(logger_entradasalida, "PID: %d - Truncar Archivo: %s - Tamaño: %d", io_dialfs->pid, io_dialfs->nombre_archivo, io_dialfs->tamanio);
                break;
            case IO_FS_WRITE:
                // Los datos a escribir están en memoria en la dirección lógica dada
                // TODO: Se necesitaría una función para obtener los datos de la memoria usando la dirección lógica
                void* datos = obtener_datos_de_memoria(io_dialfs->direccion_logica, io_dialfs->tamanio);
                operacion_exitosa = escribir_archivo(io_dialfs->nombre_archivo, datos, io_dialfs->tamanio, io_dialfs->puntero_archivo);
                free(datos);
                log_info(logger_entradasalida, "PID: %d - Escribir Archivo: %s - Tamaño a Escribir: %d - Puntero Archivo: %d", io_dialfs->pid, io_dialfs->nombre_archivo, io_dialfs->tamanio, io_dialfs->puntero_archivo);
                break;
            case IO_FS_READ:
                void* buffer = malloc(io_dialfs->tamanio);
                operacion_exitosa = leer_archivo(io_dialfs->nombre_archivo, buffer, io_dialfs->tamanio, io_dialfs->puntero_archivo);
                if (operacion_exitosa) {
                    // Escribo los datos leídos en la memoria en la dirección lógica dada
                    escribir_datos_en_memoria(io_dialfs->direccion_logica, buffer, io_dialfs->tamanio);
                    enviar_datos_leidos(fd, buffer, io_dialfs->tamanio);
                }
                free(buffer);
                log_info(logger_entradasalida, "PID: %d - Leer Archivo: %s - Tamaño a Leer: %d - Puntero Archivo: %d", io_dialfs->pid, io_dialfs->nombre_archivo, io_dialfs->tamanio, io_dialfs->puntero_archivo);
                break;
            default:
                log_error(logger_entradasalida, "Operación DialFS no reconocida");
                break;
        }
        
        if (io_dialfs->operacion != IO_FS_READ || !operacion_exitosa) {
            t_response* response = create_response(operacion_exitosa, io_dialfs->pid);
            send_confirmacion_io(fd, MSG_IO_KERNEL_DIALFS, response);
        }
        
        destruir_io_dialfs(io_dialfs);
    } else {
        log_error(logger_entradasalida, "Error al recibir la solicitud IO DIALFS");
        t_response* response = create_response(false, 0);
        send_confirmacion_io(fd, MSG_IO_KERNEL_DIALFS, response);
    }
}

//======================================================
//               FUNCIONES COMUNES
//======================================================

void send_confirmacion_io(int fd, t_msg_header header, t_response* response) {
    send_response(fd, header, response);
}

void send_IO_interface_kernel() 
{
    send_IO_interface(fd_kernel, nombre_interfaz, obtener_tipo_interfaz(entradasalida_config));
}