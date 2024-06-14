#ifndef TABLA_PAGINAS_H
#define TABLA_PAGINAS_H

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



typedef struct {
    uint32_t frame_number;
    bool valid;
} page_table_entry_t;

typedef struct {
    page_table_entry_t* entries;
    size_t num_entries;
} page_table_t;

typedef struct {
    unsigned char* bits;
    size_t size;
} bitarray_t;

bitarray_t* bitarray_create(size_t num_bits);
void bitarray_set(bitarray_t* bitarray, size_t index);
void bitarray_clear(bitarray_t* bitarray, size_t index);
int bitarray_test(bitarray_t* bitarray, size_t index);
void bitarray_destroy(bitarray_t* bitarray);

// Funciones de manejo de páginas

page_table_t* create_page_table(size_t num_pages);
void destroy_page_table(page_table_t* pt);
int get_page_frame(page_table_t* pt, size_t page_number, uint32_t* frame_number);
int add_page(page_table_t* pt, size_t page_number, uint32_t frame_number);
void remove_page(page_table_t* pt, size_t page_number);


#endif