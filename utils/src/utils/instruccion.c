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

// Serializa una instrucción en el buffer.

// TODO: revisar implementación

void serialize_instruccion(t_buffer *buffer, t_instruccion *instruccion) {
    // Serializar el nombre de la instrucción
    buffer_add_uint32(buffer, (uint32_t)instruccion->name);

    // Serializar el número de parámetros
    uint32_t num_params = list_size(instruccion->params);
    buffer_add_uint32(buffer, num_params);

    // Serializar cada parámetro
    for (int i = 0; i < num_params; i++) {
        char* param = list_get(instruccion->params, i);
        uint32_t param_length = strlen(param) + 1;
        buffer_add_uint32(buffer, param_length);
        buffer_add(buffer, param, param_length);
    }
}

// Deserializa una instrucción desde el buffer.
void deserialize_instruccion(t_buffer *buffer, t_instruccion *instruccion) {
    // Deserializar el nombre de la instrucción
    instruccion->name = (t_name_instruct)buffer_read_uint32(buffer);

    // Deserializar el número de parámetros
    uint32_t num_params = buffer_read_uint32(buffer);
    instruccion->params = list_create();

    // Deserializar cada parámetro
    for (uint32_t i = 0; i < num_params; i++) {
        uint32_t param_length = buffer_read_uint32(buffer);
        char* param = malloc(param_length);
        buffer_read(buffer, param, param_length);
        list_add(instruccion->params, param);
    }
}

// Libera la memoria asignada para una instrucción.
void instruccion_destroy(t_instruccion *instruccion) {
    if (instruccion) {
        // Liberar cada parámetro
        void _destroy_param(char* param) {
            free(param);
        }
        list_destroy_and_destroy_elements(instruccion->params, (void*)_destroy_param);
        free(instruccion);
    }
}
