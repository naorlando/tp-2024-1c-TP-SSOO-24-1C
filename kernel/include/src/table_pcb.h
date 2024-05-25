#ifndef TABLE_PCB_H_
#define TABLE_PCB_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "variables_globales.h"
#include "utils/utils.h"
#include "utils/estructuras.h"

bool add_pcb(t_PCB *);

void create_pcb_table();

t_PCB *get_pcb(uint32_t);

bool delete_pcb(uint32_t);

void delete_pcb_table();

bool _is_empty_table();

uint8_t sizeof_table_pcb();

void free_pcb(void *);

#endif