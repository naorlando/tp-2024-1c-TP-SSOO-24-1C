#ifndef RECURSOS_H_
#define RECURSOS_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "variables_globales.h"
#include "utils/utils.h"
#include "utils/estructuras.h"
#include "commons/collections/list.h"
#include "commons/collections/dictionary.h"

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

// Incrementar la cantidad de instancias de un recurso:
void incrementar_recurso(t_recurso *recurso);

// Decrementar la cantidad de instancias de un recurso:
void decrementar_recurso(t_recurso *recurso); 

void bloquear_proceso(t_recurso *recurso, t_PCB *pcb);
t_PCB* desbloquear_proceso(t_recurso *recurso);

void asignar_proceso_a_recurso(char *nombre_recurso, uint32_t pid);
bool recurso_match(void* recurso_ptr);

void print_dictionary();
bool remover_proceso_de_recurso(char *nombre_recurso, uint32_t pid);


#endif // RESOURCES_H_
