#ifndef MEMORIA_ESPACIO_H_
#define MEMORIA_ESPACIO_H_

// Standard library
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h> 
#include <stdbool.h>

// Commons
#include <commons/log.h>

// Libraries
#include <semaphore.h>
#include <pthread.h>
#include <string.h>

//project
#include "variables_globales.h"




void* read_data(uint32_t frame_number, uint32_t offset, uint32_t size);

void write_data(uint32_t frame, uint32_t offset, void* value, uint32_t data_size);

void *read_page(uint32_t frame_number);

void write_page(uint32_t frame_number, void *page_data);

void destroy_memory(memory_t* mem);

uint32_t calcular_frame_restante(uint32_t frame_number, uint32_t page_size, uint32_t offset);




#endif