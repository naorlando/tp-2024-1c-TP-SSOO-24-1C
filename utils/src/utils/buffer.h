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

// Message buffer
typedef struct
{
    uint32_t size; // Payload size
    void *stream;  // Payload
} t_buffer;

// Buffer
t_buffer *buffer_create();
void buffer_destroy(t_buffer *buffer);
void buffer_add(t_buffer *buffer, void *stream, uint32_t size);
void * extract_data_from_buffer(t_buffer *buffer);
char * extract_string_buffer (t_buffer* buffer) ;
t_buffer* recive_full_buffer (int socket);

// NUEVAS ABSTRACCIONES:
void buffer_add_uint32(t_buffer *buffer, uint32_t data); // Agrega un uint32_t al buffer
uint32_t buffer_read_uint32(t_buffer *buffer); // Lee un uint32_t del buffer y avanza el offset
void buffer_add_uint8(t_buffer *buffer, uint8_t data); // Agrega un uint8_t al buffer
uint8_t buffer_read_uint8(t_buffer *buffer); // Lee un uint8_t del buffer y avanza el offset
// void buffer_add_string(t_buffer *buffer, uint32_t length, char *string); // Agrega string al buffer con un uint32_t adelante indicando su longitud
void buffer_add_string(t_buffer *buffer, char *string);
//char *buffer_read_string(t_buffer *buffer, uint32_t *length); // Lee un string y su longitud del buffer y avanza el offset

#endif 