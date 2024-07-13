#include "recursos.h"

bool add_recurso(t_recurso *new_recurso) {
    char *key = strdup(new_recurso->nombre);
    if (key == NULL)
        return false;

    dictionary_put(recursos_dictionary, key, new_recurso);
    return true;
}

t_recurso *get_recurso(const char *nombre) {
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

    t_recurso *recurso = (t_recurso *)dictionary_get(recursos_dictionary, (char *)nombre);
    if (recurso == NULL) {
        log_warning(logger_kernel, "El recurso %s no existe en el diccionario.", nombre);
    }

    return recurso;
}

void free_recurso(void *recurso) {
    t_recurso *recurso_eliminar = (t_recurso *)recurso;
    free(recurso_eliminar->nombre);
    free(recurso_eliminar);
}

uint8_t sizeof_table_recursos() {
    return dictionary_size(recursos_dictionary);
}

bool is_empty_recurso_table() {
    return dictionary_is_empty(recursos_dictionary);
}

void incrementar_recurso(t_recurso *recurso) {
    recurso->instancias++;
}

void decrementar_recurso(t_recurso *recurso) {
    recurso->instancias--;
}

void bloquear_proceso(t_recurso *recurso, t_PCB *pcb) {
    queue_push(recurso->cola_bloqueados, pcb);
}

t_PCB* desbloquear_proceso(t_recurso *recurso) {
    return queue_pop(recurso->cola_bloqueados);
}

void asignar_proceso_a_recurso(char *nombre_recurso, u_int32_t pid) {
    t_list *recursos = dictionary_get(recursos_asignados_por_pid, uint32_to_string(pid));
    if (recursos == NULL) {
        recursos = list_create();
        dictionary_put(recursos_asignados_por_pid, uint32_to_string(pid), recursos);
    }
    list_add(recursos, (void *) nombre_recurso);
}

bool remover_proceso_de_recurso(char *nombre_recurso, u_int32_t pid) {
    t_list *recursos = dictionary_get(recursos_asignados_por_pid, uint32_to_string(pid));
    if (recursos != NULL) {
        list_remove_element(recursos, nombre_recurso);
        if (list_is_empty(recursos)) {
            dictionary_remove_and_destroy(recursos_asignados_por_pid, uint32_to_string(pid), (void *) list_destroy);
        }
        return true;
    }else{
        log_error(logger_kernel, "No se encontró el proceso %d en la lista de recursos asignados.", pid);
        return false;
    }
}