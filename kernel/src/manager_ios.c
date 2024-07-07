#include "manager_ios.h"


char* nuevo_IO_cliente_conectado(int cliente_io)
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
        liberar_IO_interface(io_interface);
        liberar_conexion(cliente_io);
        return NULL;
    }

    return obtener_nombre_conexion(io_connection);
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