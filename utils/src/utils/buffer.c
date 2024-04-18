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
    if (buffer->stream != 0)
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

void *extract_data_from_buffer(t_buffer * buffer)
{
    if (buffer->size == 0)
    {
        printf("ERROR , buffer = 0");
        exit(EXIT_FAILURE);
        // log error
    }
    if (buffer->size < 0)
    {
        ("ERROR , buffer < 0");
        // log another error
        exit(EXIT_FAILURE);
    }

    int size_full_buffer;
    memcpy(&size_full_buffer, buffer->stream, sizeof(int));

    void * full_buffer = malloc(size_full_buffer);
    memcpy(full_buffer, buffer->stream + sizeof(int), size_full_buffer);

    int nuevo_size = buffer->size - sizeof(int) - size_full_buffer;
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

    memcpy(nuevo_stream, buffer->stream + sizeof(int) + size_full_buffer, nuevo_size);
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