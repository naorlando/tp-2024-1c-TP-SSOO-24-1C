#ifndef PROCESO_H_
#define PROCESO_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <commons/string.h>
#include "variables_globales.h"
#include "utils/instruccion.h"

typedef struct {
    uint32_t pid;
    char* path_absoluto;
} t_proceso;

// Crea un nuevo proceso.
// Pre: El pid debe ser un valor único y path debe ser una cadena de texto válida.
// Post: Retorna un puntero a una estructura t_proceso con el pid y la ruta absoluta 
// especificados.
t_proceso* crear_proceso(uint32_t, char*);

// Elimina un proceso.
// Pre: El puntero a t_proceso debe ser válido y no NULL.
// Post: La memoria asignada para el proceso y su ruta absoluta es liberada.
void liberar_proceso(t_proceso*);

// Obtiene la siguiente instrucción de un proceso.
// Pre: El proceso debe ser válido y num_instruccion debe ser un número positivo.
// Post: Retorna un puntero a una estructura t_instruccion con la instrucción 
// obtenida o NULL si no se encuentra.
t_instruction* obtener_siguiente_instruccion(t_proceso*, uint32_t);

// Obtiene el ID del proceso.
// Pre: El puntero a t_proceso debe ser válido y no NULL.
// Post: Retorna el pid del proceso.
uint32_t obtener_pid(t_proceso*);

// Obtiene la ruta absoluta del proceso.
// Pre: El puntero a t_proceso debe ser válido y no NULL.
// Post: Retorna un puntero a la cadena de texto con la ruta absoluta.
char* obtener_path_absoluto(t_proceso*);

#endif