#ifndef BUFFER_H_
#define BUFFER_H_

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <commons/log.h>
#include <utils/sockets.h>

/* *************************************************
*           DEFINICION DE TIPO DE DATO
*  *************************************************/
typedef struct
{
    uint32_t size; // Payload size
    uint32_t offset; //Offset
    void* stream;  // Payload
} t_buffer;


/* *************************************************
*           PRIMITIVAS DEL BUFFER
*  *************************************************/
// Crea un buffer.
// Pre: El size del buffer es mayor a 0.
// Post: Se retorna un buffer con un size pasado por parametro y offset en 0.
t_buffer* buffer_create(uint32_t);

// Destruye un buffer.
// Pre: El buffer ha sido creado previamente.
// Post: Se libera la memoria asignada al buffer.
void buffer_destroy(t_buffer* );

// Agrega datos al buffer.
// Pre: El buffer ha sido creado previamente.
// Post: Se agregan los datos al buffer junto con su tamaño.
void buffer_add(t_buffer*, void*, uint32_t);

// Extrae los datos de un buffer y los devuelve.
// Pre: El buffer ha sido creado previamente y contiene datos.
// Post: Se extraen los datos del buffer y se devuelve un puntero a ellos.
void* extract_data_from_buffer(t_buffer*);

// Extrae una cadena de caracteres del buffer y la devuelve.
// Pre: El buffer ha sido creado previamente y contiene una cadena de caracteres.
// Post: Se extrae la cadena de caracteres del buffer y se devuelve.
char* extract_string_buffer(t_buffer*) ;

// Recibe un buffer completo desde un socket.
// Pre: El socket está conectado y listo para recibir datos.
// Post: Se recibe un buffer completo desde el socket y se devuelve.
t_buffer* recive_full_buffer(int);

// Lee datos del buffer, los guarda en un puntero proporcionado
// y avanza el offset segun el tamaño leido.
// Pre: El buffer ha sido creado previamente y contiene datos suficientes.
// Post: Se leen los datos del buffer y se almacenan en el puntero proporcionado.
void buffer_read_data(t_buffer*, void*, uint32_t);

// Agrega datos al buffer y avanza el offset segun el tamaño guardado.
// Pre: El buffer ha sido creado previamente y tiene espacio suficiente.
// Post: Se agregan los datos al buffer.
void buffer_add_data(t_buffer*, void*, uint32_t);

// Verifica si el buffer ha alcanzado el final.
// Pre: El buffer ha sido creado previamente.
// Post: Devuelve true si el buffer ha alcanzado el final, de lo contrario, false.
bool buffer_is_end(t_buffer*);

// Verifica si se pueden leer los datos con el tamaño proporcionado del buffer.
// Pre: El buffer ha sido creado previamente y contiene datos suficientes.
// Post: Devuelve true si se pueden leer los datos del buffer, de lo contrario, false.
bool buffer_can_read(t_buffer*, uint32_t);

// Verifica si se pueden escribir datos con el tamaño proporcionado en el buffer.
// Pre: El buffer ha sido creado previamente y tiene espacio suficiente.
// Post: Devuelve true si se pueden escribir datos en el buffer, de lo contrario, false.
bool buffer_can_write(t_buffer*, uint32_t);

// Verifica si se puede avanzar en el buffer.
// Pre: El buffer ha sido creado previamente y tiene espacio suficiente.
// Post: Devuelve true si se puede avanzar en el buffer, de lo contrario, false.
bool buffer_can_advance(t_buffer*, uint32_t);

// Lee un entero de 32 bits del buffer.
// Pre: El buffer ha sido creado previamente y contiene un entero de 32 bits.
// Post: Se lee el entero de 32 bits del buffer y se devuelve.
uint32_t buffer_read_uint32(t_buffer*);

// Lee un entero de 8 bits del buffer.
// Pre: El buffer ha sido creado previamente y contiene un entero de 8 bits.
// Post: Se lee el entero de 8 bits del buffer y se devuelve.
uint8_t buffer_read_uint8(t_buffer*);

// Lee un entero de 16 bits del buffer.
// Pre: El buffer ha sido creado previamente y contiene un entero de 16 bits.
// Post: Se lee el entero de 16 bits del buffer y se devuelve.
uint16_t buffer_read_uint8(t_buffer* buffer);

// Lee una cadena de caracteres del buffer.
// Pre: El buffer ha sido creado previamente y contiene una cadena de caracteres.
// Post: Se lee la cadena de caracteres del buffer y se devuelve junto con su longitud.
char* buffer_read_string(t_buffer*, uint32_t*);

// Agrega un entero de 8 bits al buffer.
// Pre: El buffer ha sido creado previamente y tiene espacio suficiente.
// Post: Se agrega el entero de 8 bits al buffer.
void buffer_add_uint8(t_buffer*, uint8_t);

// Agrega un entero de 16 bits al buffer.
// Pre: El buffer ha sido creado previamente y tiene espacio suficiente.
// Post: Se agrega el entero de 16 bits al buffer.
void buffer_add_uint16(t_buffer*, uint16_t);

// Agrega un entero de 32 bits al buffer.
// Pre: El buffer ha sido creado previamente y tiene espacio suficiente.
// Post: Se agrega el entero de 32 bits al buffer.
void buffer_add_uint32(t_buffer*, uint32_t);

// Agrega una cadena de caracteres al buffer.
// Pre: El buffer ha sido creado previamente y tiene espacio suficiente.
// Post: Se agrega la cadena de caracteres al buffer.
void buffer_add_string(t_buffer*, char*);
#endif