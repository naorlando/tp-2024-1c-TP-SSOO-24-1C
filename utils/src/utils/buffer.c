#include "buffer.h"

t_buffer *buffer_create(uint32_t size)
{
    t_buffer *buffer = malloc(sizeof(t_buffer));

    buffer->size = 0;
    buffer->stream = NULL;

    return buffer;
}

// Libera la memoria asociada al buffer
void buffer_destroy(t_buffer *buffer)
{
    if (buffer->stream != 0)
    {
        free(buffer->stream);
    }
    free(buffer);
}

// Agrega un stream al buffer en la posición actual y avanza el offset
void buffer_add(t_buffer *buffer, void *stream, uint32_t size)
{
      buffer->stream = realloc(buffer->stream, buffer->size + size + sizeof(uint32_t));

    memcpy(buffer->stream + buffer->size, &size, sizeof(uint32_t));
    memcpy(buffer->stream + buffer->size + sizeof(uint32_t), stream, size);

    buffer->size += size + sizeof(uint32_t);
}

// Guarda size bytes del principio del buffer en la dirección data y avanza el offset
void buffer_read(t_buffer *buffer, void *data, uint32_t size)
{
}