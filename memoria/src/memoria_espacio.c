#include "memoria_espacio.h"

extern memory_t *espacio_memoria;
bool is_memory_created = false;
uint16_t largo_tipo_de_dato = sizeof(uint32_t);
pthread_mutex_t MUTEX_MEMORY_SPACE;

memory_t *initialize_memory(size_t total_size, size_t page_size)
{
    memory_t *mem = malloc(sizeof(memory_t));
    mem->memory_space = malloc(total_size);
    mem->page_size = page_size;
    mem->num_frames = total_size / page_size;

    is_memory_created = true;
    return mem;
}

void *get_memory_address(uint32_t frame_number, size_t offset)
{
    return 0;
    // return (char *)espacio_memoria->memory_space + frame_number * espacio_memoria->page_size + offset;
}

uint32_t read_data(uint32_t frame_number, uint32_t offset)
{
    if (!_es_operable_sobre_memoria(offset))
    {
        exit(EXIT_FAILURE);
    }

    uint32_t total_offset = frame_number * (espacio_memoria->page_size) + offset;
    uint32_t data_leida = 0;

    pthread_mutex_lock(&MUTEX_MEMORY_SPACE);
    memcpy(&data_leida, espacio_memoria->memory_space + total_offset, largo_tipo_de_dato);
    pthread_mutex_unlock(&MUTEX_MEMORY_SPACE);

    return data_leida;
}

void write_data(uint32_t frame_number, uint32_t offset, uint32_t data)
{
    if (!_es_operable_sobre_memoria(offset))
    {
        exit(EXIT_FAILURE);
    }

    uint32_t total_offset = frame_number * espacio_memoria->page_size + offset;

    pthread_mutex_lock(&MUTEX_MEMORY_SPACE);
    memcpy(espacio_memoria->memory_space + total_offset, &data, largo_tipo_de_dato);
    pthread_mutex_unlock(&MUTEX_MEMORY_SPACE);
}

void *read_page(uint32_t frame_number)
{
    if (!_es_operable_sobre_memoria(0))
    {
        exit(EXIT_FAILURE);
    }

    void *page_data = malloc(espacio_memoria->page_size );
    uint32_t total_offset = frame_number * espacio_memoria->page_size ;

    pthread_mutex_lock(&MUTEX_MEMORY_SPACE);
    memcpy(page_data, espacio_memoria -> memory_space + total_offset, espacio_memoria->page_size );
    pthread_mutex_unlock(&MUTEX_MEMORY_SPACE);

    return page_data;
}

void write_page(uint32_t frame_number, void *page_data)
{
    if (!_es_operable_sobre_memoria(0))
    {
        exit(EXIT_FAILURE);
    }

    uint32_t total_offset = frame_number * espacio_memoria->page_size;

    pthread_mutex_lock(&MUTEX_MEMORY_SPACE);
    memcpy(espacio_memoria -> memory_space  + total_offset, page_data, espacio_memoria->page_size);
    pthread_mutex_unlock(&MUTEX_MEMORY_SPACE);
}

void destroy_memory(memory_t *mem)
{
    // for (size_t i = 0; i < mem->num_processes; ++i)
    // {
    //     if (espacio_memoria->page_tables[i] != NULL)
    //     {
    //         finalize_process(mem, i);
    //     }
    // }
    // free(espacio_memoria->page_tables);
    free(espacio_memoria->memory_space);
    // bitarray_destroy(espacio_memoria->frame_usage);
    free(mem);
}

bool _es_operable_sobre_memoria(uint32_t offset)
{
    if (!is_memory_created)
    {
        log_error(logger_memoria, "Para escribir sobre la memoria primero debe crearse. Aborting...");
        return false;
    }
    if (offset % largo_tipo_de_dato != 0 && offset != 0)
    {
        log_error(logger_memoria, "El offset debe ser divisible por el tamaño de dato guardado. Aborting...");
        return false;
    }
    if (offset >= espacio_memoria->page_size)
    {
        log_error(logger_memoria, "El offset ser menor que el marco. Aborting...");
        return false;
    }
    return true;
}