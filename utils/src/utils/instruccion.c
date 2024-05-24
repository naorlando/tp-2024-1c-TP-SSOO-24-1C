#include "instruccion.h"

t_instruccion* crear_instruccion(char* linea) {
    t_instruccion* instruccion_nueva = malloc(sizeof(t_instruccion));

    if(instruccion_nueva == NULL) {
        return NULL;
    }

    instruccion_nueva->linea_instruccion = string_duplicate(linea);

    if(instruccion_nueva->linea_instruccion == NULL) {
        return NULL;
    }

    return instruccion_nueva;
}

void eliminar_instruccion(t_instruccion* instruccion) {
    char* linea_instruccion = obtener_instruccion(instruccion);

    if(linea_instruccion != NULL) {
        free(linea_instruccion);
    }

    free(instruccion);
}

char* obtener_instruccion(t_instruccion* instruccion) {
    return instruccion->linea_instruccion;
}

// deserializa una instrucción desde un buffer con un stream que tiene una estructura de t_instruction:
// typedef struct {
//     uint32_t pid;
//     char* path;
// } t_proceso;
void deserialize_instruccion(t_buffer *buffer, t_instruction *instruccion) {

}
