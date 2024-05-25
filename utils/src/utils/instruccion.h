#ifndef INSTRUCCION_H_
#define INSTRUCCION_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/string.h>
#include <commons/collections/list.h>
#include "utils/utils.h"
#include "utils/buffer.h"

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

// Elimina una instrucción.
// Pre: El puntero a t_instruction debe ser válido y no NULL.
// Post: La memoria asignada para la instrucción y su línea es liberada.
void eliminar_instruccion(t_instruction*);

// Obtiene la línea de la instrucción.
// Pre: El puntero a t_instruction debe ser válido y no NULL.
// Post: Retorna un puntero a la cadena de texto de la instrucción.
t_name_instruction obtener_nombre_instruccion(t_instruction*);

t_list* obtener_parametros(t_instruction*);

void free_parametro(void*);

/**********************FUNCIONES PRIVADAS**********************/

// Obtiene el tipo de instrucción a partir de una cadena de caracteres.
// Pre: El puntero a la cadena de caracteres 'linea' debe ser válido y no NULL.
// Post: Retorna el valor correspondiente del enum t_name_instruction si la cadena
// coincide con alguna instrucción válida. Si no coincide, retorna NULL e imprime
// un mensaje de error en stderr.
t_name_instruction _obtener_tipo_instruccion(char*); 

t_list* _lista_parametros(char**); 

char* _leer_parametro(char**, int);

// Serializa una instrucción en el buffer.
// pre: el buffer y la instrucción deben ser válidos y no NULL.
// post: la instrucción es serializada en el buffer.
void serialize_instruccion(t_buffer *buffer, t_instruction *instruccion);

// Deserializa una instrucción desde el buffer.
// pre: el buffer y la instrucción deben ser válidos y no NULL.
// post: la instrucción es deserializada desde el buffer.
void deserialize_instruccion(t_buffer *buffer, t_instruction *instruccion);

// Destruye una instrucción.
// pre: el puntero a la instrucción debe ser válido y no NULL.
// post: la memoria asignada a la instrucción es liberada.
void instruccion_destroy(t_instruction *instruccion);

#endif