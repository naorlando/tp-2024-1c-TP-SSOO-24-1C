#ifndef INSTRUCCION_H_
#define INSTRUCCION_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/string.h>

typedef struct {
    char* linea_instruccion;
} t_instruccion;

// Crea una nueva instrucción a partir de una línea de texto.
// Pre: La línea de texto debe ser válida y no NULL.
// Post: Retorna un puntero a una estructura t_instruccion con la línea de instrucción duplicada.
t_instruccion* crear_instruccion(char*);

// Elimina una instrucción.
// Pre: El puntero a t_instruccion debe ser válido y no NULL.
// Post: La memoria asignada para la instrucción y su línea es liberada.
void eliminar_instruccion(t_instruccion*);

// Obtiene la línea de la instrucción.
// Pre: El puntero a t_instruccion debe ser válido y no NULL.
// Post: Retorna un puntero a la cadena de texto de la instrucción.
char* obtener_instruccion(t_instruccion*);

#endif