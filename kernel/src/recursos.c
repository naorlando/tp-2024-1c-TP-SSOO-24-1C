#include "recursos.h"

// Añadir un recurso a la tabla
bool add_recurso(t_recurso *new_recurso) {
    char *key = strdup(new_recurso->nombre); // Usamos el nombre del recurso como clave
    if (key == NULL)
        return false;

    dictionary_put(recursos_dictionary, key, new_recurso);
    return true;
}

// Obtener un recurso de la tabla
t_recurso * get_recurso(const char * nombre) {
    if (recursos_dictionary == NULL) {
        log_error(logger_kernel, "El diccionario de recursos no está inicializado.");
        return NULL;
    }

    if (dictionary_is_empty(recursos_dictionary)) {
        log_warning(logger_kernel, "El diccionario de recursos está vacío.");
        return NULL;
    }

    if (nombre == NULL) {
        log_error(logger_kernel, "El nombre del recurso es NULL.");
        return NULL;
    }

    t_recurso * recurso = (t_recurso *)dictionary_get(recursos_dictionary, (char*)nombre);
    if (recurso == NULL) {
        log_warning(logger_kernel, "El recurso %s no existe en el diccionario.", nombre);
    }

    return recurso;
}


// Liberar la memoria de un recurso
void free_recurso(void *recurso) {
    t_recurso *recurso_eliminar = (t_recurso *)recurso;
    free(recurso_eliminar->nombre);
    free(recurso_eliminar);
}

// Obtener el tamaño de la tabla de recursos
uint8_t sizeof_table_recursos() {
    return dictionary_size(recursos_dictionary);
}

// Verificar si la tabla de recursos está vacía
bool is_empty_recurso_table() {
    return dictionary_is_empty(recursos_dictionary);
}
