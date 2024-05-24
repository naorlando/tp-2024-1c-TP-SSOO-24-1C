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
