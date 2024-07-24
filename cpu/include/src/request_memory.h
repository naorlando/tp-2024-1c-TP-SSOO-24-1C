#ifndef REQUEST_MEMORY_H
#define REQUEST_MEMORY_H

#include <stdio.h>
#include <commons/log.h>
#include <stdbool.h>

#include "variables_globales.h"
#include "utils/instruccion.h"
#include "utils/estructuras.h"
#include "utils/protocolo.h"
#include "utils/io_frames_lib.h"
#include "protocolo_cpu.h"
#include "cpu_interrupt.h"
#include "mmu.h"
#include "tlb.h"
#include "cpu_config.h"
#include "cpu_dispatch.h"

size_t obtener_tamano_registro(const char *nombre);

void exec_mov_in(uint32_t direccion_logica, char *nombre, void *memory_value);
void exec_mov_out(uint32_t direccion_logica, void *write_value, uint32_t tamanio);
int read_from_memory(uint32_t pid, uint32_t logical_address, void *memory_value, uint32_t cantidad_paginas, uint32_t tamano);
int write_into_memory(uint32_t pid, uint32_t logical_address, void *write_value, uint32_t cantidad_paginas, uint32_t tamano);
uint8_t ajustar_tamano_proceso(uint32_t pid, uint32_t nuevo_tamano);
t_io_frames *exec_io_frames(uint32_t pid, uint32_t direccion_logica, uint32_t tamano);
void copiar_cadena(uint32_t origen, uint32_t destino, int tamano);

#endif