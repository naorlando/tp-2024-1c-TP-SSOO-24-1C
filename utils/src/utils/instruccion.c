#include "instruccion.h"

t_instruction* crear_instruccion(char* linea) {
    t_instruction* instruccion_nueva = malloc(sizeof(t_instruction));

    if(instruccion_nueva == NULL) {
        return NULL;
    }

    char** partes_instruccion = string_split(linea, " ");

    instruccion_nueva->linea_instruccion = string_duplicate(linea);

    if(instruccion_nueva->linea_instruccion == NULL) {
        return NULL;
    }

    return instruccion_nueva;
}

void eliminar_instruccion(t_instruction* instruccion) {
    char* linea_instruccion = obtener_instruccion(instruccion);

    if(linea_instruccion != NULL) {
        free(linea_instruccion);
    }

    free(instruccion);
}

char* obtener_instruccion(t_instruction* instruccion) {
    return instruccion->linea_instruccion;
}

t_name_instruction obtener_tipo_instruccion(char* linea) {
    if (strcmp(instruccion, "SET") == 0) return SET;
    if (strcmp(instruccion, "SUM") == 0) return SUM;
    if (strcmp(instruccion, "SUB") == 0) return SUB;
    if (strcmp(instruccion, "MOV_IN") == 0) return MOV_IN;
    if (strcmp(instruccion, "MOV_OUT") == 0) return MOV_OUT;
    if (strcmp(instruccion, "RESIZE") == 0) return RESIZE;
    if (strcmp(instruccion, "WAIT") == 0) return WAIT;
    if (strcmp(instruccion, "SIGNAL") == 0) return SIGNAL;
    if (strcmp(instruccion, "JNZ") == 0) return JNZ;
    if (strcmp(instruccion, "COPY_STRING") == 0) return COPY_STRING;
    if (strcmp(instruccion, "IO_GEN_SLEEP") == 0) return IO_GEN_SLEEP;
    if (strcmp(instruccion, "IO_STDIN_READ") == 0) return IO_STDIN_READ;
    if (strcmp(instruccion, "IO_STDOUT_WRITE") == 0) return IO_STDOUT_WRITE;
    if (strcmp(instruccion, "IO_FS_CREATE") == 0) return IO_FS_CREATE;
    if (strcmp(instruccion, "IO_FS_DELETE") == 0) return IO_FS_DELETE;
    if (strcmp(instruccion, "IO_FS_TRUNCATE") == 0) return IO_FS_TRUNCATE;
    if (strcmp(instruccion, "IO_FS_WRITE") == 0) return IO_FS_WRITE;
    if (strcmp(instruccion, "IO_FS_READ") == 0) return IO_FS_READ;
    if (strcmp(instruccion, "EXIT") == 0) return EXIT;

    // En caso de error
    fprintf(stderr, "Error: Instrucción desconocida '%s'\n", instruccion);
    return NULL; // Valor que no está en el enum
}
