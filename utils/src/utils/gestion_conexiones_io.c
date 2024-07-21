#include "gestion_conexiones_io.h"

//========================================================
// FUNCIONES AVANZADAS DE T_IO_CONNECTION CON T_DICTIONARY
//========================================================

t_IO_connection* get_IO_connection(char* nombre_interfaz, t_dictionary* io_connections, pthread_mutex_t* mutex_dictionary) 
{
    pthread_mutex_lock(mutex_dictionary);
    t_IO_connection* io_connection = dictionary_get(io_connections, nombre_interfaz);
    pthread_mutex_unlock(mutex_dictionary);
    return io_connection;
}

void agregar_IO_connection(t_IO_connection* io_connection, t_dictionary* io_connections, pthread_mutex_t* mutex_dictionary)
{
    pthread_mutex_lock(mutex_dictionary);
    char* nombre_interfaz = obtener_nombre_conexion(io_connection);

    if (!dictionary_has_key(io_connections, nombre_interfaz)) {
        dictionary_put(io_connections, nombre_interfaz, io_connection);
    }
    
    pthread_mutex_unlock(mutex_dictionary);
}

t_IO_connection* nuevo_IO_cliente_conectado(int cliente_io, t_log* logger)
{
    t_IO_interface* io_interface = recv_IO_interface(cliente_io);

    if (io_interface == NULL) {
        log_error(logger, "Error al recibir la interfaz de E/S del cliente.");
        liberar_conexion(cliente_io);
        return NULL;
    }

    // Crear la estructura t_IO_connection
    t_IO_connection* io_connection = crear_IO_connection(obtener_nombre_IO_interface(io_interface), obtener_tipo_IO_interface(io_interface), cliente_io);
    if (io_connection == NULL) {
        log_error(logger, "Error al crear la conexión de E/S.");
        liberar_conexion(cliente_io);
        return NULL;
    }

    // Libero la io_interface
    liberar_IO_interface(io_interface);

    return io_connection;
}

t_IO_connection* recibir_io_connection(int cliente_io, t_log* logger, int header_valido) 
{
    int cod_op = recibir_operacion(cliente_io);

    if(cod_op == header_valido) {
        return nuevo_IO_cliente_conectado(cliente_io, logger);
    } else {
        log_error(logger, "Error al recibir un cliente IO. Operación incorrecta: %d", cod_op);
        liberar_conexion(cliente_io);
    }
    return NULL;
}