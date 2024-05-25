#ifndef TABLA_PROCESOS_H_
#define TABLA_PROCESOS_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "variables_globales.h"
#include "proceso.h"
#include "utils/utils.h"

bool agregar_proceso(t_proceso*);

t_proceso* obtener_proceso(uint32_t);

bool eliminar_proceso(uint32_t);

void free_proceso(void*);

uint8_t cantidad_procesos_activos();

void eliminar_tabla();

void crear_tabla_procesos();

/**********************FUNCIONES PRIVADAS**********************/

bool _esta_vacia_tabla();

#endif