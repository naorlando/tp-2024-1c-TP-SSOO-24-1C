#ifndef MEMORIA_ESPACIO_H
#define MEMORIA_ESPACIO_H


#include "variables_globales.h"
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
#include "tabla_paginas.h"


typedef struct {
    void* memory_space;
    size_t page_size;
    size_t num_frames;

} memory_t;


memory_t* initialize_memory(size_t total_size, size_t page_size);

void* get_memory_address(uint32_t frame_number, size_t offset);

uint32_t read_data(uint32_t frame_number, uint32_t offset);

void write_data(uint32_t frame, uint32_t offset,uint32_t value);

void *read_page(uint32_t frame_number);

void write_page(uint32_t frame_number, void *page_data);

bool _es_operable_sobre_memoria(uint32_t offset);

void destroy_memory(memory_t* mem);

#endif