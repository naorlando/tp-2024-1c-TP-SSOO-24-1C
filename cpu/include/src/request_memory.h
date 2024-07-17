#ifndef REQUEST_MEMORY_H
#define REQUEST_MEMORY_H

#include <stdio.h>
#include <commons/log.h>
#include <stdbool.h>

#include "variables_globales.h"
#include "utils/instruccion.h"
#include "utils/estructuras.h"
#include "protocolo_cpu.h"
#include "cpu_interrupt.h"
#include "mmu.h"
#include "tlb.h"
#include "cpu_config.h"
#include "cpu_dispatch.h"


size_t obtener_tamano_registro(const char *nombre);


void exec_mov_in(uint32_t direccion_logica, char *nombre, uint32_t *memory_value);
void exec_mov_out(uint32_t direccion_logica, uint32_t write_value, uint32_t tamanio);
int read_from_memory(uint32_t pid, uint32_t logical_address, uint32_t *memory_value,  uint32_t cantidad_paginas);
int write_into_memory(uint32_t pid, uint32_t logical_address, uint32_t cpu_register,  uint32_t cantidad_paginas);
uint8_t ajustar_tamano_proceso(uint32_t pid, uint32_t  nuevo_tamano);
void exec_io_stdin_read(uint32_t interfaz ,uint32_t direccion_logica , uint32_t tamano );
void copiar_cadena(uint32_t origen, uint32_t destino, int tamano);

#endif