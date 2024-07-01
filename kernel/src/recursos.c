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
t_recurso *get_recurso(const char *nombre) {
    if (dictionary_is_empty(recursos_dictionary))
        return NULL;

    t_recurso *recurso = (t_recurso *)dictionary_get(recursos_dictionary, nombre);
    return (recurso != NULL) ? recurso : NULL;
}

// Eliminar un recurso de la tabla
bool delete_recurso(const char *nombre) {
    if (dictionary_is_empty(recursos_dictionary))
        return false;

    bool isDelete = true;

    if (dictionary_has_key(recursos_dictionary, nombre)) {
        dictionary_remove_and_destroy(recursos_dictionary, nombre, free_recurso);
    } else {
        isDelete = false;
    }

    return isDelete;
}

// Destruir la tabla de recursos
void delete_recurso_table() {
    if (!dictionary_is_empty(recursos_dictionary)) {
        dictionary_clean_and_destroy_elements(recursos_dictionary, free_recurso);
    }
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
