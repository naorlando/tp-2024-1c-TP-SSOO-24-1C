#ifndef INSTRUCCION_H_
#define INSTRUCCION_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/string.h>

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
}t_name_instruct;

typedef struct {
    t_name_instruct name;
    t_list * params;
}t_instruction;

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