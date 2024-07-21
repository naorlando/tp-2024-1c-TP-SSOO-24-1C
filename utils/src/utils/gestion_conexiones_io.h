#ifndef GESTION_CONEXIONES_IO_
#define GESTION_CONEXIONES_IO_

#include <commons/collections/dictionary.h>
#include <commons/log.h>
#include "protocolo.h"
#include "sockets.h"
#include "io_interface.h"

//========================================================
// FUNCIONES AVANZADAS DE T_IO_CONNECTION CON T_DICTIONARY
//========================================================

// Obtiene una conexión de entrada/salida a partir de su nombre.
// Pre: El puntero a la cadena de nombre_interfaz y los punteros a t_dictionary y pthread_mutex_t deben ser válidos y no NULL.
// Post: Retorna un puntero a t_IO_connection si la conexión existe, o NULL si no se encuentra.
t_IO_connection* get_IO_connection(char* nombre_interfaz, t_dictionary* io_connections, pthread_mutex_t* mutex_dictionary);

// Agrega una nueva conexión de entrada/salida al diccionario de conexiones.
// Pre: Los punteros a t_IO_connection, t_dictionary y pthread_mutex_t deben ser válidos y no NULL.
// Post: Agrega la conexión al diccionario si no existe previamente.
void agregar_IO_connection(t_IO_connection* io_connection, t_dictionary* io_connections, pthread_mutex_t* mutex_dictionary);

// Crea una nueva conexión de entrada/salida para un cliente conectado.
// Pre: El file descriptor del cliente (cliente_io) debe ser válido. El puntero a t_log debe ser válido y no NULL.
// Post: Retorna un puntero a t_IO_connection si la conexión fue creada exitosamente, o NULL si hubo un error.
t_IO_connection* nuevo_IO_cliente_conectado(int cliente_io, t_log* logger);

// Recibe y valida una nueva conexión de entrada/salida para un cliente.
// Pre: El file descriptor del cliente (cliente_io), el puntero a t_log y el header_valido deben ser válidos.
// Post: Retorna un puntero a t_IO_connection si la conexión fue recibida y validada exitosamente, o NULL si hubo un error.
t_IO_connection* recibir_io_connection(int cliente_io, t_log* logger, int header_valido);


#endif // GESTION_CONEXIONES_IO_