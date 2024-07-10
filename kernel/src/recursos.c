#include "recursos.h"

bool add_recurso(t_recurso *new_recurso) {
    char *key = strdup(new_recurso->nombre);
    if (key == NULL)
        return false;

    new_recurso->procesos_asignados = list_create();
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
    list_destroy_and_destroy_elements(recurso_eliminar->procesos_asignados, free);
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

void asignar_proceso_a_recurso(t_recurso *recurso, int pid) {
    list_add(recurso->procesos_asignados, (void *)(intptr_t)pid);
}


void bloquear_proceso(t_recurso *recurso, t_PCB *pcb) {
    queue_push(recurso->cola_bloqueados, pcb);
}

t_PCB* desbloquear_proceso(t_recurso *recurso) {
    return queue_pop(recurso->cola_bloqueados);
}

// void remover_proceso_de_recurso(t_recurso *recurso, int pid) {
//     bool pid_match(void* pid_ptr) {
//         return (intptr_t)pid_ptr == (intptr_t)pid;
//     }
//     list_remove_and_destroy_by_condition(recurso->procesos_asignados, pid_match, free);
// }

// bool proceso_asignado_a_recurso(t_recurso *recurso, int pid) {
//     bool pid_match(void* pid_ptr) {
//         return (intptr_t)pid_ptr == (intptr_t)pid;
//     }
//     return list_any_satisfy(recurso->procesos_asignados, pid_match);
// }

// Función de coincidencia de PID
// bool pid_match(void* pid_ptr, void* target_pid) {
//     return (intptr_t)pid_ptr == (intptr_t)target_pid;
// }
// void remover_proceso_de_recurso(t_recurso *recurso, int pid) {
//     list_remove_and_destroy_by_condition(recurso->procesos_asignados, pid_match, (void *)(intptr_t)pid);
// }

// bool proceso_asignado_a_recurso(t_recurso *recurso, int pid) {
//     return list_remove_element(recurso->procesos_asignados, pid);
// }

bool remove_asignado_a_recurso(t_recurso *recurso, int pid) {
    return list_remove_element(recurso->procesos_asignados, (void *)(intptr_t)pid);
}