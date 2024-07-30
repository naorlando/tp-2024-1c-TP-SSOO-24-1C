#include "protocolo_entradasalida.h"

//======================================================
//        FUNCIONES DE ENTRADA/SALIDA GENERICA
//======================================================

// Función que atiende una solicitud genérica de entrada/salida,
// la cual consiste en esperar un tiempo determinado
void atender_solicitud_generica(int fd)
{
    t_io_generica *io_generica = recv_io_generica(fd);

    if (io_generica != NULL)
    {
        log_info(logger_entradasalida, "PID: <%d> - Operacion: <SLEEP>", obtener_pid_generica(io_generica));

        bool resultado = ejecutar_unidades_de_trabajo(io_generica);

        log_info(logger_entradasalida, "Operacion IO_GEN_SLEEP finalizada");

        t_response *response = create_response(resultado, obtener_pid_generica(io_generica));
        send_confirmacion_io(fd, MSG_IO_KERNEL_GENERICA, response);

        destruir_io_generica(io_generica);
    }
    else
    {
        log_error(logger_entradasalida, "Error al recibir la solicitud IO GENERICA. ABORTANDO");
    }
}

//======================================================
//        FUNCIONES DE ENTRADA/SALIDA STDIN
//======================================================

// Función que atiende una solicitud de entrada/salida STDIN,
// la cual consiste en leer un texto desde la consola y guardarlo en memoria
void atender_solicitud_stdin(int fd)
{
    t_io_stdin *io_stdin = recv_io_stdin(fd);

    if (io_stdin != NULL)
    {
        uint32_t tamanio_io_stdin = get_tamano_total_io_frames(io_stdin->frames_data);

        log_info(logger_entradasalida, "PID: <%d> - Operacion: <READ>", obtener_pid_stdin(io_stdin));

        char *input = leer_entrada_limitada(tamanio_io_stdin);
        escribir_memoria(io_stdin->frames_data, input);
        free(input);
        log_info(logger_entradasalida, "Operacion READ finalizada");

        // TODO: modificar la funcion para que no este harcodeado el 'true'
        t_response *response = create_response(true, obtener_pid_stdin(io_stdin));
        send_confirmacion_io(fd, MSG_IO_KERNEL_STDIN, response);

        destruir_io_stdin(io_stdin);
    }
    else
    {
        log_error(logger_entradasalida, "Error al recibir IO STDIN");
    }
}

//======================================================
//        FUNCIONES DE ENTRADA/SALIDA STDOUT
//======================================================

// Función que atiende una solicitud de entrada/salida STDOUT,
// la cual consiste en leer un texto desde memoria y mostrarlo por consola
void atender_solicitud_stdout(int fd)
{
    t_io_stdout *io_stdout = recv_io_stdout(fd);

    if (io_stdout != NULL)
    {
        log_info(logger_entradasalida, "PID: <%d> - Operacion: <WRITE>", obtener_pid_stdout(io_stdout));

        char *valor = leer_memoria(io_stdout->frames_data);
        log_info(logger_entradasalida, "TAMAÑO LEIDO : <%d> - CADENA : <%s>", get_tamano_total_io_frames(io_stdout->frames_data), valor);

        // TODO: modificar la funcion para que no este harcodeado el 'true'
        t_response *response = create_response(true, obtener_pid_stdout(io_stdout));
        send_confirmacion_io(fd, MSG_IO_KERNEL_STDOUT, response);

        log_info(logger_entradasalida, "Operacion WRITE finalizada");

        destruir_io_stdout(io_stdout);
    }
    else
    {
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
            {
                t_io_dialfs_cd* dialfs_cd = get_dialfs_generic(io_dialfs);
                operacion_exitosa = crear_archivo_dialfs(dialfs_cd->nombre_archivo);
                log_info(logger_entradasalida, "PID: %d - Crear Archivo: %s", io_dialfs->pid, dialfs_cd->nombre_archivo);
                break;
            }
            case IO_FS_DELETE:
            {
                t_io_dialfs_cd* dialfs_cd = get_dialfs_generic(io_dialfs);
                operacion_exitosa = eliminar_archivo_dialfs(dialfs_cd->nombre_archivo);
                log_info(logger_entradasalida, "PID: %d - Eliminar Archivo: %s", io_dialfs->pid, dialfs_cd->nombre_archivo);
                break;
            }
            // case IO_FS_TRUNCATE:
            // {
            //     t_io_dialfs_truncate* dialfs_truncate = get_dialfs_generic(io_dialfs);
            //     //operacion_exitosa = truncar_archivo_dialfs(dialfs_truncate->nombre_archivo, dialfs_truncate->tamanio);
            //     log_info(logger_entradasalida, "PID: %d - Truncar Archivo: %s - Tamaño: %d", io_dialfs->pid, dialfs_truncate->nombre_archivo, dialfs_truncate->tamanio);
            //     break;
            // }
            // case IO_FS_WRITE:
            // {    // Los datos a escribir están en memoria en la dirección lógica dada
            //     // TODO: Se necesitaría una función para obtener los datos de la memoria usando la dirección lógica
            //     // void* datos = obtener_datos_de_memoria(io_dialfs->direccion_logica, io_dialfs->tamanio);
            //     // operacion_exitosa = escribir_archivo_dialfs(io_dialfs->nombre_archivo, datos, io_dialfs->tamanio, io_dialfs->puntero_archivo);
            //     // free(datos);
            //     t_io_dialfs_rw* dialfs_rw = get_dialfs_generic(io_dialfs);
            //     log_info(logger_entradasalida, "PID: %d - Escribir Archivo: %s - Tamaño a Escribir: %d - Puntero Archivo: %d", io_dialfs->pid, dialfs_rw->nombre_archivo, dialfs_rw->tamanio, dialfs_rw->puntero_archivo);
            //     break;
            // }
            // case IO_FS_READ:
            // {
            //     t_io_dialfs_rw* dialfs_rw = get_dialfs_generic(io_dialfs);
            //     //void* buffer = malloc(io_dialfs->tamanio);
            //     //operacion_exitosa = leer_archivo_dialfs(dialfs_rw->nombre_archivo, buffer, dialfs_rw->tamanio, dialfs_rw->puntero_archivo);
            //     if (operacion_exitosa) {
            //         // Escribo los datos leídos en la memoria en la dirección lógica dada
            //         //escribir_datos_en_memoria(io_dialfs->direccion_logica, buffer, io_dialfs->tamanio);
            //         //enviar_datos_leidos(fd, buffer, io_dialfs->tamanio);
            //     }
            //     //free(buffer);
            //     log_info(logger_entradasalida, "PID: %d - Leer Archivo: %s - Tamaño a Leer: %d - Puntero Archivo: %d", io_dialfs->pid, dialfs_rw->nombre_archivo, dialfs_rw->tamanio, dialfs_rw->puntero_archivo);
            //     break;
            // }
            default:
                log_error(logger_entradasalida, "Operación DialFS no reconocida");
                break;
        }
        
        if (io_dialfs->operacion != IO_FS_READ || operacion_exitosa) {
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

// Función auxiliar que envía una confirmación de la operación de entrada/salida
void send_confirmacion_io(int fd, t_msg_header header, t_response *response)
{
    send_response(fd, header, response);
}

void send_IO_interface_kernel()
{
    send_IO_interface(fd_kernel, nombre_interfaz, obtener_tipo_interfaz(entradasalida_config), MSG_IO_KERNEL);
}

void send_IO_interface_memoria()
{
    send_IO_interface(fd_memoria, nombre_interfaz, obtener_tipo_interfaz(entradasalida_config), MSG_IO_MEMORIA);
}
