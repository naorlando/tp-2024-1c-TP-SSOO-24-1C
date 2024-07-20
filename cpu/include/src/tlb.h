#ifndef TLB_H
#define TLB_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "variables_globales.h"

// Estructura de la TLB

bool obtener_marco(uint32_t pid, uint32_t page, uint32_t* frame);
void reemplazar(uint32_t pid, uint32_t page, uint32_t frame);
void limpiar_proceso_TLB(uint32_t pid);
void imprimir_tlb();
int32_t _proxima_referencia_tlb();

#endif // TLB_H
