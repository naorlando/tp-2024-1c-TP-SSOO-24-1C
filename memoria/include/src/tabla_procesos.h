#ifndef TABLA_PROCESOS_H_
#define TABLA_PROCESOS_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "variables_globales.h"
#include "proceso.h"
#include "utils/utils.h"

// Agrega un proceso a la tabla de procesos.
// Pre: El puntero a t_proceso debe ser válido y no NULL, la tabla de procesos debe existir.
// Post: Retorna true si el proceso se ha agregado exitosamente, o false si hubo un error.
bool agregar_proceso(t_proceso*);

// Obtiene un proceso de la tabla de procesos dado su pid.
// Pre: La tabla de procesos no debe estar vacía.
// Post: Retorna un puntero al proceso si se encuentra, o NULL si no se encuentra o hubo un error.
t_proceso* obtener_proceso(uint32_t);

// Elimina un proceso de la tabla de procesos dado su pid.
// Pre: La tabla de procesos no debe estar vacía.
// Post: Retorna true si el proceso se ha eliminado exitosamente, o false si no se encuentra o hubo un error.
bool eliminar_proceso(uint32_t);

// Libera la memoria asignada a un proceso.
// Pre: El puntero a proceso debe ser válido y no NULL.
// Post: La memoria asociada al proceso ha sido liberada.
void free_proceso(void*);

// Obtiene la cantidad de procesos activos en la tabla de procesos.
// Pre: La tabla de procesos debe existir.
// Post: Retorna el número de procesos activos en la tabla de procesos.
uint8_t cantidad_procesos_activos();

// Elimina todos los elementos de la tabla de procesos y libera su memoria.
// Pre: La tabla de procesos debe existir.
// Post: Todos los procesos en la tabla han sido eliminados y su memoria liberada.
void eliminar_tabla();

// Crea la tabla de procesos.
// Pre: La tabla de procesos no debe existir previamente.
// Post: La tabla de procesos ha sido creada y está lista para ser utilizada.
void crear_tabla_procesos();

/**********************FUNCIONES PRIVADAS**********************/
// Verifica si la tabla de procesos está vacía.
// Pre: La tabla de procesos debe existir.
// Post: Retorna true si la tabla está vacía, o false si contiene elementos.
bool _esta_vacia_tabla();

// Verifica si la tabla de procesos existe.
// Pre: Ninguna.
// Post: Retorna true si la tabla de procesos existe, o false si no existe.
bool _existe_tabla();

#endif