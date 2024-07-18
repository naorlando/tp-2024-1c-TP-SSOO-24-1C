#include "manager_ios.h"


t_IO_connection* nuevo_IO_cliente_conectado(int cliente_io)
{
    t_IO_interface* io_interface = recv_IO_interface(cliente_io);

    if (io_interface == NULL) {
        log_error(logger_kernel, "Error al recibir la interfaz de E/S del cliente.");
        liberar_conexion(cliente_io);
        return NULL;
    }

    // Crear la estructura t_IO_connection
    t_IO_connection* io_connection = crear_IO_connection(obtener_nombre_IO_interface(io_interface), obtener_tipo_IO_interface(io_interface), cliente_io);
    if (io_connection == NULL) {
        log_error(logger_kernel, "Error al crear la conexión de E/S.");
        liberar_conexion(cliente_io);
        return NULL;
    }

    // Libero la io_interface
    liberar_IO_interface(io_interface);

    return io_connection;
}

void agregar_IO_connection(t_IO_connection* io_connection)
{
    pthread_mutex_lock(&MUTEX_DICTIONARY);
    char* nombre_interfaz = obtener_nombre_conexion(io_connection);

    if (!dictionary_has_key(io_connections, nombre_interfaz)) {
        dictionary_put(io_connections, nombre_interfaz, io_connection);
    }
    
    pthread_mutex_unlock(&MUTEX_DICTIONARY);
}

t_IO_connection* get_IO_connection(char* nombre_interfaz) 
{
    pthread_mutex_lock(&MUTEX_DICTIONARY);
    t_IO_connection* io_connection = dictionary_get(io_connections, nombre_interfaz);
    pthread_mutex_unlock(&MUTEX_DICTIONARY);
    return io_connection;
}

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

void* obtener_siguiente_proceso(t_IO_connection* cliente_io)
{
    return obtener_proceso_bloqueado(cliente_io);
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

t_IO_connection* recibir_io_connection(int cliente_io) 
{
    int cod_op = recibir_operacion(cliente_io);

    if(cod_op == MSG_IO_KERNEL) {
        return nuevo_IO_cliente_conectado(cliente_io);
    } else {
        log_error(logger_kernel, "Error al recibir un cliente IO. Operación incorrecta: %d", cod_op);
        liberar_conexion(cliente_io);
    }
    return NULL;
}
