
#ifndef INICIALIZAR_ESPACIO_H_
#define INICIALIZAR_ESPACIO_H_

// Standard library
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h> 
#include <stdbool.h>

// Commons


// Libraries


//project


typedef struct {
    void* memory_space;
    size_t page_size;
    size_t num_frames;
} memory_t;


memory_t* initialize_memory(size_t total_size, size_t page_size);

#endif