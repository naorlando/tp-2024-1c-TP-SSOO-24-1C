#include "inicializar_espacio.h"
//static memory_t * espacio_memoria = initialize_memory(memoria_config->TAM_MEMORIA,memoria_config->TAM_PAGINA);



memory_t *initialize_memory(size_t total_size, size_t page_size)
{
    memory_t *mem = malloc(sizeof(memory_t));
    mem->memory_space = malloc(total_size);
    mem->page_size = page_size;
    mem->num_frames = total_size / page_size;

    //is_memory_created = true;
    return mem;
}