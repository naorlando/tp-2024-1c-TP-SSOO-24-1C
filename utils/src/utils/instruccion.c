#include "instruccion.h"

t_instruction* crear_instruccion(char* linea) {
    t_instruction* instruccion_nueva = malloc(sizeof(t_instruction));

    if(instruccion_nueva == NULL) {
        return NULL;
    }

    char** partes_instruccion = split(linea, " ");

    if(partes_instruccion == NULL) return NULL;

    instruccion_nueva->name = _obtener_tipo_instruccion(partes_instruccion[0]);
    instruccion_nueva->params = _lista_parametros(partes_instruccion);

    if(instruccion_nueva->name == -1 || instruccion_nueva->params == NULL) {
        return NULL;
    }

    array_string_destroy(partes_instruccion);

    return instruccion_nueva;
}

t_instruction* crear_instruccion_con_parametros(t_name_instruction name, t_list* params) {
    t_instruction* instruccion_nueva = malloc(sizeof(t_instruction));

    if(instruccion_nueva == NULL || params == NULL) {
        return NULL;
    }

    instruccion_nueva->name = name;
    instruccion_nueva->params = params;

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
    t_list* parametros = list_create();

    if(parametros == NULL) return NULL;

    int cant_parametros = arrayLength(array_parametros);

    //Se arranca de 1 porque la posicion 0 tiene la instruccion
    for(int i = 1; i < cant_parametros; i++) {
        list_add(parametros, _leer_parametro(array_parametros, i));
    }

    return parametros;
}

char* _leer_parametro(char** parametros, int index) {
    char* parametro = NULL;

    int cant_parametros = arrayLength(parametros);

    if(index >= 0 && index < cant_parametros) {
        parametro = my_strdup(parametros[index]);
    }

    return parametro;
}

void free_parametro(void* parametro) {
    char* parametro_eliminar = (char*)parametro;

    free(parametro_eliminar);
}

uint32_t obtener_instruction_size(t_instruction* instruccion) 
{
    uint32_t size = 0;

    // Sumo el nombre de la instruccion (es un enum)
    size += sizeof(obtener_nombre_instruccion(instruccion));

    // obtengo los parametros que tiene la instruccion
    t_list* params = obtener_parametros(instruccion);
    uint32_t num_params = list_size(params);

    // Sumo la cantidad de paramtros
    size += sizeof(num_params);

    for (int i = 0; i < num_params; i++) {
        size += sizeof(uint32_t); // para el tamaño del parametro

        char* param = (char*)list_get(params, i);

        size += (strlen(param)+1); // para el parametro
    }
    
    return size;
}

//TODO: ELIMINAR --> NO SE USA!
// Libera la memoria asignada para una instrucción.
void instruccion_destroy(t_instruction *instruccion) {
    if (instruccion) {
        // Liberar cada parámetro
        void _destroy_param(char* param) {
            free(param);
        }
        list_destroy_and_destroy_elements(instruccion->params, (void*)_destroy_param);
        free(instruccion);
    }
}