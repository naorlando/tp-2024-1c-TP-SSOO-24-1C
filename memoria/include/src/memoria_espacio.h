#ifndef MEMORIA_ESPACIO_H
#define MEMORIA_ESPACIO_H

// Standard library
#include <stdio.h>
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
    size_t total_size;
    size_t page_size;
    size_t num_frames;
    bitarray_t* frame_usage;
    page_table_t** page_tables;
    size_t num_processes;
} memory_t;

memory_t* initialize_memory(size_t total_size, size_t page_size);
void create_process(memory_t* mem, size_t num_pages);
void finalize_process(memory_t* mem, size_t process_id);
void* get_memory_address(memory_t* mem, uint32_t frame_number, size_t offset);

int read_data(memory_t* mem, size_t process_id, uint32_t logical_address, void* buffer, size_t size);
int write_data(memory_t* mem, size_t process_id, uint32_t logical_address, const void* data, size_t size);

int read_page(memory_t* mem, size_t process_id, size_t page_number, void* buffer);
int write_page(memory_t* mem, size_t process_id, size_t page_number, const void* data);

void destroy_memory(memory_t* mem);

#endif