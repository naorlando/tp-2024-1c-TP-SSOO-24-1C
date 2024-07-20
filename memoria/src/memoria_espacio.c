#include "memoria_espacio.h"

uint16_t largo_tipo_de_dato = sizeof(uint32_t);

void* read_data(uint32_t frame_number, uint32_t offset, uint32_t size_value) {
    uint32_t total_offset = frame_number * (espacio_memoria->page_size) + offset;
    void* data_leida = malloc(size_value);
    
    pthread_mutex_lock(&MUTEX_MEMORY_SPACE);
    memcpy(data_leida, (uint8_t*)espacio_memoria->memory_space + total_offset, size_value);
    pthread_mutex_unlock(&MUTEX_MEMORY_SPACE);
    
    return data_leida;
}

// Función para escribir datos en la memoria
void write_data(uint32_t frame_number, uint32_t offset, void* data, uint32_t data_size) {
    uint32_t total_offset = frame_number * espacio_memoria->page_size + offset;
    
    pthread_mutex_lock(&MUTEX_MEMORY_SPACE);
    memcpy((uint8_t*)espacio_memoria->memory_space + total_offset, data, data_size);
    pthread_mutex_unlock(&MUTEX_MEMORY_SPACE);
}


void *read_page(uint32_t frame_number)
{

    void *page_data = malloc(espacio_memoria->page_size);
    uint32_t total_offset = frame_number * espacio_memoria->page_size;

    pthread_mutex_lock(&MUTEX_MEMORY_SPACE);
    memcpy(page_data, espacio_memoria->memory_space + total_offset, espacio_memoria->page_size);
    pthread_mutex_unlock(&MUTEX_MEMORY_SPACE);

    return page_data;
}

void write_page(uint32_t frame_number, void *page_data)
{

    uint32_t total_offset = frame_number * espacio_memoria->page_size;

    pthread_mutex_lock(&MUTEX_MEMORY_SPACE);
    memcpy(espacio_memoria->memory_space + total_offset, page_data, espacio_memoria->page_size);
    pthread_mutex_unlock(&MUTEX_MEMORY_SPACE);
}

void destroy_memory(memory_t *mem)
{

    free(espacio_memoria->memory_space);
    free(mem);
}

uint32_t calcular_frame_restante(uint32_t frame_number, uint32_t page_size, uint32_t offset)
{
    uint32_t temp = frame_number * page_size + offset;
    if (temp >= page_size)
    {
        return temp - page_size + 1;
    }
    else
    {
        return page_size - temp + 1;
    }
}