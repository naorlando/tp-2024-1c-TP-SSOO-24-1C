#include "proceso.h"

t_proceso* crear_proceso(uint32_t pid, char* path) {
    t_proceso* proceso_nuevo = malloc(sizeof(t_proceso));

    if(proceso_nuevo == NULL) {
        return NULL;
    }

    proceso_nuevo->pid = pid;
    proceso_nuevo->path_absoluto = string_duplicate(path);

    if(proceso_nuevo->path_absoluto == NULL) {
        return NULL;
    }

    return proceso_nuevo;
} 

t_instruction* obtener_siguiente_instruccion(t_proceso* proceso, uint32_t num_instruccion) {
    FILE* codigo = fopen(obtener_path_absoluto(proceso), "r");

    if(codigo == NULL) {
        return NULL;
    }

    char buffer[MAX_LINE_LENGTH];
    int num_linea_actual = 1;
    char *linea = NULL;

    while (fgets(buffer, sizeof(buffer), codigo) != NULL) {
        if (num_linea_actual == num_instruccion) {
            linea = malloc(strlen(buffer) + 1);
            if (linea != NULL) {
                strcpy(linea, buffer);
            }
            break;
        }
        num_linea_actual++;
    }

    if (linea == NULL) {
        fprintf(stderr, "Error: Línea %d no encontrada en el archivo %s\n", num_linea_actual, obtener_path_absoluto(proceso));
    }

    fclose(codigo);
    return crear_instruccion(linea);
}

void liberar_proceso(t_proceso* proceso) {
    char* path_absoluto = obtener_path_absoluto(proceso);

    if(path_absoluto != NULL) {
        free(path_absoluto);
    }

    free(proceso);
}

uint8_t obtener_pid(t_proceso* proceso) {
    return proceso->pid;
}

char* obtener_path_absoluto(t_proceso* proceso) {
    return proceso->path_absoluto;
}