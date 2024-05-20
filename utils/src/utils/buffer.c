#include "buffer.h"

/* *************************************************
*           PRIMITIVAS DEL BUFFER
*  *************************************************/

t_buffer* buffer_create(uint32_t size)
{
    t_buffer* buffer = malloc(sizeof(t_buffer));

    if(buffer == NULL) return NULL;

    buffer->size = size;
    buffer->offset = 0;
    buffer->stream = malloc(size);

    if(buffer->stream == NULL) {
        free(buffer);
        return NULL;
    }

    return buffer;
}

// Buffer destroy
void buffer_destroy(t_buffer* buffer)
{
    if (buffer->stream != NULL)
    {
        free(buffer->stream);
    }
    free(buffer);
}

// Buffer add
void buffer_add(t_buffer* buffer, void* stream, uint32_t size)
{

    buffer->stream = realloc(buffer->stream, buffer->size + size + sizeof(uint32_t));

    memcpy(buffer->stream + buffer->size, &size, sizeof(uint32_t));
    memcpy(buffer->stream + buffer->size + sizeof(uint32_t), stream, size);

    buffer->size += size + sizeof(uint32_t);
}

t_buffer* recive_full_buffer(int socket)
{
    uint32_t bytes_recive;
    if (recv(socket, &(bytes_recive), sizeof(uint32_t), MSG_WAITALL) > 0)
    {
        t_buffer* new_buffer= buffer_create(bytes_recive);
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

void* extract_data_from_buffer(t_buffer* buffer)
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


char* extract_string_buffer(t_buffer* buffer)
{
    char* content = extract_data_from_buffer(buffer);
    return content;
}

void buffer_read_data(t_buffer* buffer, void* data, uint32_t size) {

    if(buffer_is_end(buffer)) {
        //El buffer no tiene mas datos para leer
        return;
    }

    if(!buffer_can_read(buffer, size)) {
        //El buffer no puede leer el tamaño pedido
        return;
    }

    //Copio los datos desde el buffer
    memcpy(data, buffer->stream + buffer->offset, size);

    //Actualizo el offset del buffer para leer desde esta posicion
    //en la siguiente llamada
    buffer->offset += size;
}

void buffer_add_data(t_buffer* buffer, void* data, uint32_t size) {
    if(buffer_is_end(buffer)){
        //Se lleno el buffer
        return;
    }

    if(!buffer_can_write(buffer, size)) {
        //El buffer no puede agregar el tamaño pedido
        return;
    }

    //Agrego datos al buffer
    memcpy(buffer->stream + buffer->offset, data, size);

    //Actualizo el offset del buffer para agregar desde esta posicion
    //en la siguiente llamada
    buffer->offset += size;
}

bool buffer_is_end(t_buffer* buffer){
    return (buffer->offset == buffer->size);
}

// bool buffer_can_read(t_buffer* buffer, uint32_t size) {
//     return buffer_can_advance(buffer, size);
// }

bool buffer_can_read(t_buffer* buffer, uint32_t size) {
    return buffer_can_advance(buffer, size);
}

bool buffer_can_advance(t_buffer* buffer, uint32_t size){
    return (buffer->offset + size <= buffer->size);
}

//-----Funciones para leer tipos de datos del buffer-----
uint32_t buffer_read_uint32(t_buffer* buffer){
    uint32_t data;

    buffer_read_data(buffer, &data, sizeof(uint32_t));

    return data;
}

uint8_t buffer_read_uint8(t_buffer* buffer){
    uint8_t data;

    buffer_read_data(buffer, &data, sizeof(uint8_t));

    return data;
}

uint16_t buffer_read_uint16(t_buffer* buffer){
    uint16_t data;

    buffer_read_data(buffer, &data, sizeof(uint16_t));

    return data;
}

char* buffer_read_string(t_buffer* buffer, uint32_t* length){
    *length = buffer_read_uint32(buffer);

    char* string = malloc(*length + 1);

    if(string == NULL) {
        //No hay espacio de memoria
        return NULL;
    }

    buffer_read_data(buffer, string, *length);
    string[*length] = '\0'; // Añado fin de cadena

    return string;
}


//-----Funciones para escribir datos en el buffer-----
void buffer_add_uint8(t_buffer* buffer, uint8_t data){
    buffer_add_data(buffer, &data, sizeof(uint8_t));
}

void buffer_add_uint16(t_buffer* buffer, uint16_t data){
    buffer_add_data(buffer, &data, sizeof(uint16_t));
}

void buffer_add_uint32(t_buffer* buffer, uint32_t data){
    buffer_add_data(buffer, &data, sizeof(uint32_t));
}

void buffer_add_string(t_buffer* buffer, char* data){
    buffer_add_data(buffer, data, strlen(data));
}