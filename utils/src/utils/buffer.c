#include "buffer.h"

/*
 *      Buffer
 */

// Buffer create
t_buffer *buffer_create()
{

    t_buffer *buffer = malloc(sizeof(t_buffer));

    buffer->size = 0;
    buffer->stream = NULL;

    return buffer;
}

// Buffer destroy
void buffer_destroy(t_buffer *buffer)
{
    if (buffer->stream != NULL)
    {
        free(buffer->stream);
    }
    free(buffer);
}

// Buffer add
void buffer_add(t_buffer *buffer, void *stream, uint32_t size)
{

    buffer->stream = realloc(buffer->stream, buffer->size + size + sizeof(uint32_t));

    memcpy(buffer->stream + buffer->size, &size, sizeof(uint32_t));
    memcpy(buffer->stream + buffer->size + sizeof(uint32_t), stream, size);

    buffer->size += size + sizeof(uint32_t);
}

t_buffer *recive_full_buffer(int socket)
{
    t_buffer *new_buffer = malloc(sizeof(t_buffer));
    if (recv(socket, &(new_buffer->size), sizeof(int), MSG_WAITALL) > 0)
    {
        new_buffer->stream = malloc(new_buffer->size);
        if (recv(socket, new_buffer->stream, new_buffer->size, MSG_WAITALL) > 0)
        {
            return new_buffer;
        }
        else
        {
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        exit(EXIT_FAILURE);
    }
}

void *extract_data_from_buffer(t_buffer *buffer)
{
    if (buffer->stream == NULL)
    {
        printf("ERROR , buffer->stream");
        exit(EXIT_FAILURE);
    }
    if (buffer->size == 0)
    {
        printf("ERROR , buffer = 0");
        exit(EXIT_FAILURE);
        // log error
    }
    if (buffer->size < 0)
    {
        printf("ERROR , buffer < 0");
        // log another error
        exit(EXIT_FAILURE);
    }

    uint32_t size_full_buffer;
    memcpy(&size_full_buffer, buffer->stream, sizeof(uint32_t));

    void *full_buffer = malloc(size_full_buffer);

    if (size_full_buffer <= 0)
    {
        printf("ERROR , size_full_buffer");
        exit(EXIT_FAILURE);
    }

    memcpy(full_buffer, buffer->stream + sizeof(uint32_t), size_full_buffer);

     if (full_buffer != NULL)
    {
        uint32_t * f_B = full_buffer;
        printf("ERROR , full_buffer %ls \n",f_B);
        exit(EXIT_FAILURE);
    }

    uint32_t nuevo_size = buffer->size - sizeof(uint32_t) - size_full_buffer;
    if (nuevo_size == 0)
    {
        buffer->size = 0;
        free(buffer->stream);

        buffer->stream = NULL;
        return full_buffer;
    }
    if (nuevo_size < 0)
    {
        perror("\n[ERROR] BUFFER con tamaño negativo");
        exit(EXIT_FAILURE);
    }

    void *nuevo_stream = malloc(nuevo_size);

    memcpy(nuevo_stream, buffer->stream + sizeof(uint32_t) + size_full_buffer, nuevo_size);
    free(buffer->stream);

    buffer->size = nuevo_size;
    buffer->stream = nuevo_stream;

    return full_buffer;
}


char *extract_string_buffer(t_buffer *buffer)
{
    char *content = extract_data_from_buffer(buffer);
    return content;
}

// NUEVAS ABSTRACCIONES:

// --------> TESTEAR...

// Agrega un uint32_t al buffer
void buffer_add_uint32(t_buffer *buffer, uint32_t data) {
    buffer->stream = realloc(buffer->stream, buffer->size + sizeof(uint32_t));
    memcpy(buffer->stream + buffer->size, &data, sizeof(uint32_t));
    buffer->size += sizeof(uint32_t);
}

// Agrega un uint8_t al buffer
void buffer_add_uint8(t_buffer *buffer, uint8_t data) {
    buffer->stream = realloc(buffer->stream, buffer->size + sizeof(uint8_t));
    memcpy(buffer->stream + buffer->size, &data, sizeof(uint8_t));
    buffer->size += sizeof(uint8_t);
}

// Agrega un string al buffer con un uint32_t adelante indicando su longitud:

// void buffer_add_string(t_buffer *buffer, uint32_t length, char *string) {
//     buffer->stream = realloc(buffer->stream, buffer->size + sizeof(uint32_t) + length);
//     memcpy(buffer->stream + buffer->size, &length, sizeof(uint32_t));
//     memcpy(buffer->stream + buffer->size + sizeof(uint32_t), string, length);
//     buffer->size += sizeof(uint32_t) + length;
// }
void buffer_add_string(t_buffer *buffer, char *string) {
    buffer_add(buffer, string, strlen(string) + 1);
}

// Lee un uint32_t del buffer y avanza el offset
uint32_t buffer_read_uint32(t_buffer *buffer) {
    uint32_t data;
    memcpy(&data, buffer->stream, sizeof(uint32_t));
    buffer->stream += sizeof(uint32_t);
    buffer->size -= sizeof(uint32_t);
    return data;
}

// Lee un uint8_t del buffer y avanza el offset
uint8_t buffer_read_uint8(t_buffer *buffer) {
    uint8_t data;
    memcpy(&data, buffer->stream, sizeof(uint8_t));
    buffer->stream += sizeof(uint8_t);
    buffer->size -= sizeof(uint8_t);
    return data;
}

// Lee un string y su longitud del buffer y avanza el offset
// char *buffer_read_string(t_buffer *buffer, uint32_t *length) {
//     memcpy(length, buffer->stream, sizeof(uint32_t));
//     buffer->stream += sizeof(uint32_t);
//     char *string = malloc(*length);
//     memcpy(string, buffer->stream, *length);
//     buffer->stream += *length;
//     buffer->size -= sizeof(uint32_t) + *length;
//     return string;
// }
