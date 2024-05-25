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
