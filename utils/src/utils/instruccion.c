#include "instruccion.h"

t_instruction* crear_instruccion(char* linea) {
    t_instruction* instruccion_nueva = malloc(sizeof(t_instruction));

    if(instruccion_nueva == NULL) {
        return NULL;
    }

    char** partes_instruccion = string_split(linea, " ");

    if(partes_instruccion == NULL) return NULL;

    instruccion_nueva->name = _obtener_tipo_instruccion(partes_instruccion[0]);
    instruccion_nueva->params = _lista_parametros(partes_instruccion);

    if(instruccion_nueva->name == -1 || instruccion_nueva->params == NULL) {
        return NULL;
    }

    return instruccion_nueva;
}

void eliminar_instruccion(t_instruction* instruccion) {
    t_list* parametros = obtener_parametros(instruccion);

    if(parametros != NULL) {
        list_destroy_and_destroy_elements(parametros, free_parametro);
    }

    free(instruccion);
}

t_name_instruction obtener_nombre_instruccion(t_instruction* instruccion) {
    return instruccion->name;
}

t_name_instruction _obtener_tipo_instruccion(char* linea) {
    if (strcmp(linea, "SET") == 0) return SET;
    if (strcmp(linea, "SUM") == 0) return SUM;
    if (strcmp(linea, "SUB") == 0) return SUB;
    if (strcmp(linea, "MOV_IN") == 0) return MOV_IN;
    if (strcmp(linea, "MOV_OUT") == 0) return MOV_OUT;
    if (strcmp(linea, "RESIZE") == 0) return RESIZE;
    if (strcmp(linea, "WAIT") == 0) return WAIT;
    if (strcmp(linea, "SIGNAL") == 0) return SIGNAL;
    if (strcmp(linea, "JNZ") == 0) return JNZ;
    if (strcmp(linea, "COPY_STRING") == 0) return COPY_STRING;
    if (strcmp(linea, "IO_GEN_SLEEP") == 0) return IO_GEN_SLEEP;
    if (strcmp(linea, "IO_STDIN_READ") == 0) return IO_STDIN_READ;
    if (strcmp(linea, "IO_STDOUT_WRITE") == 0) return IO_STDOUT_WRITE;
    if (strcmp(linea, "IO_FS_CREATE") == 0) return IO_FS_CREATE;
    if (strcmp(linea, "IO_FS_DELETE") == 0) return IO_FS_DELETE;
    if (strcmp(linea, "IO_FS_TRUNCATE") == 0) return IO_FS_TRUNCATE;
    if (strcmp(linea, "IO_FS_WRITE") == 0) return IO_FS_WRITE;
    if (strcmp(linea, "IO_FS_READ") == 0) return IO_FS_READ;
    if (strcmp(linea, "EXIT") == 0) return EXIT;

    // En caso de error
    fprintf(stderr, "Error: Instrucción desconocida '%s'\n", linea);
    return -1; // Valor que no está en el enum
}

t_list* obtener_parametros(t_instruction* instruccion) {
    t_list* parametros = instruccion->params;

    if(parametros == NULL) return NULL;

    return parametros;
}

t_list* _lista_parametros(char** array_parametros) {
    t_list* parametros = malloc(sizeof(t_list));

    if(parametros == NULL) return NULL;

    //Se arranca de 1 porque la posicion 0 tiene la instruccion
    for(int i = 1; i < arrayLength(array_parametros); i++) {
        list_add(parametros, _leer_parametro(array_parametros, i));
    }

    return parametros;
}

char* _leer_parametro(char** parametros, int index) {
    char* parametro = NULL;

    if(index >= 0 && index < arrayLength(parametros)) {
        parametro = parametros[index];
    }

    return parametro;
}

void free_parametro(void* parametro) {
    char* parametro_eliminar = (char*)parametro;

    free(parametro_eliminar);
}
