#ifndef INICIALIZAR_TLB_H_
#define INICIALIZAR_TLB_H_



#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "commons/collections/list.h"

typedef struct {
    int32_t pid;
    int32_t page;
    int32_t frame;
    int32_t cont_referencia; // Usado para LRU
} t_TLB;



// Prototipos de funciones
void inicializar_TLB(uint32_t entradas, t_list **);


#endif