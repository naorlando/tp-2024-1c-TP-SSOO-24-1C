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

#endif