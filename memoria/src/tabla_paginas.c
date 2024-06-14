#include "tabla_paginas.h"

bitarray_t* bitarray_create(size_t num_bits) {
    bitarray_t* bitarray = malloc(sizeof(bitarray_t));
    bitarray->size = (num_bits + 7) / 8;
    bitarray->bits = calloc(bitarray->size, sizeof(unsigned char));
    return bitarray;
}

void bitarray_set(bitarray_t* bitarray, size_t index) {
    bitarray->bits[index / 8] |= (1 << (index % 8));
}

void bitarray_clear(bitarray_t* bitarray, size_t index) {
    bitarray->bits[index / 8] &= ~(1 << (index % 8));
}

int bitarray_test(bitarray_t* bitarray, size_t index) {
    return bitarray->bits[index / 8] & (1 << (index % 8));
}

void bitarray_destroy(bitarray_t* bitarray) {
    free(bitarray->bits);
    free(bitarray);
}

// Funciones de manejo de páginas

page_table_t* create_page_table(size_t num_pages) {
    page_table_t* pt = malloc(sizeof(page_table_t));
    pt->num_entries = num_pages;
    pt->entries = malloc(sizeof(page_table_entry_t) * num_pages);
    for (size_t i = 0; i < num_pages; ++i) {
        pt->entries[i].valid = false;
    }
    return pt;
}

void destroy_page_table(page_table_t* pt) {
    free(pt->entries);
    free(pt);
}

int get_page_frame(page_table_t* pt, size_t page_number, uint32_t* frame_number) {
    if (page_number >= pt->num_entries || !pt->entries[page_number].valid) {
        return -1; // Página no válida
    }
    *frame_number = pt->entries[page_number].frame_number;
    return 0;
}

int add_page(page_table_t* pt, size_t page_number, uint32_t frame_number) {
    if (page_number >= pt->num_entries) {
        return -1; // Número de página fuera de rango
    }
    pt->entries[page_number].frame_number = frame_number;
    pt->entries[page_number].valid = true;
    return 0;
}

void remove_page(page_table_t* pt, size_t page_number) {
    if (page_number < pt->num_entries) {
        pt->entries[page_number].valid = false;
    }
}