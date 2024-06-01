#ifndef INSTRUCCION_H_
#define INSTRUCCION_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/string.h>
#include <commons/collections/list.h>
#include "utils/utils.h"

typedef enum {
    SET,
    SUM,
    SUB,
    MOV_IN,
    MOV_OUT,
    RESIZE,
    WAIT ,
    SIGNAL ,
    JNZ,
    COPY_STRING,
    IO_GEN_SLEEP ,
    IO_STDIN_READ ,
    IO_STDOUT_WRITE ,
    IO_FS_CREATE ,
    IO_FS_DELETE ,
    IO_FS_TRUNCATE ,
    IO_FS_WRITE ,
    IO_FS_READ ,
    EXIT
}t_name_instruction;

typedef struct {
    t_name_instruction name;
    t_list* params;
}t_instruction;

// Crea una nueva instrucción a partir de una línea de texto.
// Pre: La línea de texto debe ser válida y no NULL.
// Post: Retorna un puntero a una estructura t_instruction con la línea de instrucción duplicada.
t_instruction* crear_instruccion(char*);

// Crea una nueva instrucción a partir de un nombre de instrucción y una lista de parámetros.
// Pre: El nombre de instrucción y la lista de parámetros deben ser válidos y no NULL.
// Post: Retorna un puntero a una estructura t_instruction con el nombre y los parámetros asignados.
t_instruction* crear_instruccion_con_parametros(t_name_instruction, t_list*);

// Elimina una instrucción.
// Pre: El puntero a t_instruction debe ser válido y no NULL.
// Post: La memoria asignada para la instrucción y su línea es liberada.
void eliminar_instruccion(t_instruction*);

// Obtiene la línea de la instrucción.
// Pre: El puntero a t_instruction debe ser válido y no NULL.
// Post: Retorna un puntero a la cadena de texto de la instrucción.
t_name_instruction obtener_nombre_instruccion(t_instruction*);

// Obtiene la lista de parámetros de una instrucción.
// Pre: El puntero a t_instruction debe ser válido y no NULL.
// Post: Retorna un puntero a la lista de parámetros (t_list*) de la instrucción. 
//       Si la instrucción no tiene parámetros, retorna NULL.
t_list* obtener_parametros(t_instruction*);

// Libera la memoria asignada a un parámetro.
// Pre: El puntero al parámetro debe ser válido y no NULL.
// Post: La memoria asignada al parámetro es liberada.
void free_parametro(void*);

/**********************FUNCIONES PRIVADAS**********************/

// Obtiene el tipo de instrucción a partir de una cadena de caracteres.
// Pre: El puntero a la cadena de caracteres 'linea' debe ser válido y no NULL.
// Post: Retorna el valor correspondiente del enum t_name_instruction si la cadena
// coincide con alguna instrucción válida. Si no coincide, retorna NULL e imprime
// un mensaje de error en stderr.
t_name_instruction _obtener_tipo_instruccion(char*); 

// Crea una lista de parámetros a partir de un array de cadenas de texto.
// Pre: El puntero al array de cadenas de texto (array_parametros) debe ser válido y no NULL.
//      La primera posición del array (array_parametros[0]) contiene la instrucción y no se 
//      debe incluir en la lista de parámetros.
// Post: Retorna un puntero a una lista de parámetros (t_list*). 
//       Si no se puede crear la lista, retorna NULL.
t_list* _lista_parametros(char**); 

// Lee un parámetro del array de cadenas de texto en el índice especificado.
// Pre: El puntero al array de cadenas de texto (parametros) debe ser válido y no NULL.
//      El índice debe estar dentro del rango válido del array.
// Post: Retorna un puntero a la cadena de texto del parámetro en el índice especificado.
//       Si el índice está fuera del rango válido, retorna NULL.
char* _leer_parametro(char**, int);

// Destruye una instrucción.
// pre: el puntero a la instrucción debe ser válido y no NULL.
// post: la memoria asignada a la instrucción es liberada.
void instruccion_destroy(t_instruction *instruccion);

#endif