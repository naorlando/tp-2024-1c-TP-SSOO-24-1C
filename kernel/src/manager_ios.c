#include "manager_ios.h"

procesar_solicitud_func obtener_procesador_solicitud(int tipo_conexion) 
{
    switch(tipo_conexion) {
        case GENERICA:
            return (procesar_solicitud_func) procesar_solicitud_generica;
        case STDIN:
            return (procesar_solicitud_func) procesar_solicitud_stdin;
        case STDOUT:
            return (procesar_solicitud_func) procesar_solicitud_stdout;
        case DIALFS:
            return (procesar_solicitud_func) procesar_solicitud_dialfs;
        default:
            return NULL;
    }
}

int procesar_solicitud_IO(int fd, void* solicitud, procesar_solicitud_func procesar_func) 
{
    return procesar_func(fd, solicitud);
}

int procesar_solicitud_generica(int fd, t_solicitud_io_generica* solicitud_io_gen) 
{
    t_io_generica* io_gen = obtener_io_solicitud_generica(solicitud_io_gen);
    char* nombre_interfaz = obtener_nombre_solicitud_generica(solicitud_io_gen);

    return send_kernel_io_gen_sleep(fd, nombre_interfaz, io_gen);
}

int procesar_solicitud_stdin(int fd, t_solicitud_io_stdin* solicitud_io_stdin) 
{
    t_io_stdin* io_stdin = obtener_io_solicitud_stdin(solicitud_io_stdin);
    char* nombre_interfaz = obtener_nombre_solicitud_stdin(solicitud_io_stdin);

    return send_kernel_io_stdin(fd, nombre_interfaz, io_stdin);
}

int procesar_solicitud_stdout(int fd, t_solicitud_io_stdout* solicitud_io_stdout) 
{
    t_io_stdout* io_stdout = obtener_io_solicitud_stdout(solicitud_io_stdout);
    char* nombre_interfaz = obtener_nombre_solicitud_stdout(solicitud_io_stdout);

    return send_kernel_io_stdout(fd, nombre_interfaz, io_stdout);
}

// TODO: Terminar de implementar
void procesar_solicitud_dialfs()
{

}

void procesar_respuesta_io(int fd, char* nombre_interfaz)
{
    t_response* response = recv_response(fd);

    if (response == NULL) {
        log_error(logger_kernel, "Error al recibir el response de la E/S %s", nombre_interfaz);
    }else {
        char* resultado_str = get_process_response(response) ? "true" : "false";
        log_info(logger_kernel, "Procesamiento de la interfaz '%s': %s para el PID <%d>", nombre_interfaz, resultado_str, get_pid_response(response));
    }

    delete_response(response);
}

void proceso_solicita_io(void* solicitud)
{
    
}
