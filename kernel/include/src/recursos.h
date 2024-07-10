#ifndef RECURSOS_H_
#define RECURSOS_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "variables_globales.h"
#include "utils/utils.h"
#include "utils/estructuras.h"

// Declaraciones de funciones

// Añadir un recurso a la tabla
bool add_recurso(t_recurso *new_recurso);

// Obtener un recurso de la tabla
t_recurso *get_recurso(const char *nombre);

// Liberar la memoria de un recurso
void free_recurso(void *recurso);

// Obtener el tamaño de la tabla de recursos
uint8_t sizeof_table_recursos();

// Verificar si la tabla de recursos está vacía
bool is_empty_recurso_table();

void asignar_proceso_a_recurso(t_recurso *recurso, int pid);
void remover_proceso_de_recurso(t_recurso *recurso, int pid);
void bloquear_proceso(t_recurso *recurso, t_PCB *pcb);
t_PCB* desbloquear_proceso(t_recurso *recurso);
bool proceso_asignado_a_recurso(t_recurso *recurso, int pid);
//bool pid_match(void* pid_ptr);

#endif // RESOURCES_H_
