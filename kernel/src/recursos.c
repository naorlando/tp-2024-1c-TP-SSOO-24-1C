#include "recursos.h"

bool add_recurso(t_recurso *new_recurso) {
    char *key = my_strdup(new_recurso->nombre);
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
    // semaforo: de DETENER_PLANIFICACION.
    queue_push(recurso->cola_bloqueados, pcb);
    log_info(logger_kernel, "Proceso %d bloqueado por recurso %s", pcb->pid, recurso->nombre);
}

t_PCB* desbloquear_proceso(t_recurso *recurso) {
    return queue_pop(recurso->cola_bloqueados);
}


void asignar_proceso_a_recurso(char *nombre_recurso, uint32_t pid) {
    t_list *recursos = dictionary_get(recursos_asignados_por_pid, uint32_to_string(pid));
    if (recursos == NULL) {
        recursos = list_create();
        dictionary_put(recursos_asignados_por_pid, uint32_to_string(pid), recursos);
    }
    list_add(recursos, (void *) nombre_recurso);
    print_dictionary();
}

bool remover_proceso_de_recurso(char *nombre_recurso, uint32_t pid) {
    char* pid_str = uint32_to_string(pid);

    t_list *recursos = dictionary_get(recursos_asignados_por_pid, pid_str);
    if (recursos != NULL) {
        bool recurso_match(void* recurso_ptr) {
            return strcmp((char*) recurso_ptr, nombre_recurso) == 0;
        }
        (char*)list_remove_by_condition(recursos, recurso_match);
        // if (list_is_empty(recursos)) {
        //     dictionary_remove_and_destroy(recursos_asignados_por_pid, pid_str, (void *) list_destroy);
        // }
        free(pid_str);  // Liberar la memoria asignada para pid_str
        print_dictionary();
        return true;
    } else {
        log_error(logger_kernel, "No se encontró el proceso %d en la lista de recursos asignados.", pid);
        free(pid_str);  // Liberar la memoria asignada para pid_str
        return false;
    }
}

void print_dictionary() {
    void print_item(char* key, void* value) {
        t_list* recursos = (t_list*)value;
        printf("PID: %s -> Recursos: ", key);
        for (int i = 0; i < list_size(recursos); i++) {
            printf("%s ", (char*)list_get(recursos, i));
        }
        printf("\n");
    }
    dictionary_iterator(recursos_asignados_por_pid, print_item);
}

// manejo de chequeo de recursos al finalizar un proceso:
void liberar_recursos_de_proceso(u_int32_t pid)
{
// chequeo que el proceso es valido y se encuentra en el diccionario de pcbs:
    if(get_pcb(pid) == NULL){
        log_error(logger_kernel, "No se encontro el proceso (con PID: %d) en el diccionario de PCBs ", pid);
        return;
    }
// chequeo que el proceso tiene recursos asignados:
    t_list *recursos = dictionary_get(recursos_asignados_por_pid, uint32_to_string(pid));
    if (recursos == NULL) {
        log_info(logger_kernel, "El proceso %d no tiene recursos asignados, se puede eliminar tranquilamente.", pid);
        return;
    }
// Si tiene, les hago signal a los recursos en cuestión que no se habían liberado.
    while (!list_is_empty(recursos)) {
        char *nombre_recurso = (char *)list_get(recursos, 0);
        t_recurso *recurso = get_recurso(nombre_recurso);
        if (recurso != NULL) {
            incrementar_recurso(recurso);
            log_info(logger_kernel, "Se libera recurso %s (SIN PREVIAMENTE SER LIBERADO) del PID = %d", nombre_recurso, pid);
            print_dictionary(); //antes
            remover_proceso_de_recurso(nombre_recurso, pid);
            //sem_post(&recurso->sem);
        }
        //free(nombre_recurso); // Si la memoria del nombre del recurso fue asignada dinámicamente, liberarla.
    }
}

void free_resource(t_PCB *pcb) {
    log_info(logger_kernel, "Liberando recursos del proceso %d", pcb->pid);

    // Liberar recursos asociados al proceso
    liberar_recursos_de_proceso(pcb->pid);

    // Verificar si el proceso está en el diccionario antes de eliminarlo
    char* pid_str = uint32_to_string(pcb->pid);
    if (dictionary_has_key(recursos_asignados_por_pid, pid_str)) {
        dictionary_remove_and_destroy(recursos_asignados_por_pid, pid_str, (void *)list_destroy);
        log_info(logger_kernel, "Proceso %d eliminado del diccionario de recursos asignados.", pcb->pid);
    } else {
        log_warning(logger_kernel, "Proceso %d no se encontró en el diccionario de recursos asignados.", pcb->pid);
    }
    free(pid_str);
}



void handle_wait(t_PCB *pcb, char *nombre_recurso, bool from_signal) {

    pthread_mutex_lock(&MUTEX_RECURSOS);

    t_recurso *recurso = get_recurso(nombre_recurso);
    
    if (recurso == NULL) {
        log_error(logger_kernel, "Recurso %s no encontrado. Proceso %d enviado a EXIT.", nombre_recurso, pcb->pid);
        enviar_proceso_a_exit(pcb);
        return;
    }

    if (recurso->instancias > 0) {
        decrementar_recurso(recurso); // recurso->instancias--;
        // ----------------------------------------------------------------------------------------------
        asignar_proceso_a_recurso(nombre_recurso, pcb->pid);
        // ----------------------------------------------------------------------------------------------
        log_info(logger_kernel, "Recurso %s tenía instancias disponibles. Instancias restantes: %d", nombre_recurso, recurso->instancias);
        if (!from_signal) {
            send_pcb_cpu(pcb);
        } else {
            agregar_a_cola_ready(pcb);
        }
    } else {
        bloquear_proceso(recurso, pcb);
        log_info(logger_kernel, "Recurso %s no tenía instancias disponibles. Proceso %d bloqueado", nombre_recurso, pcb->pid);
        sem_post(&SEM_CPU);    
    }

    pthread_mutex_unlock(&MUTEX_RECURSOS);
}

void handle_signal(t_PCB *pcb, char *nombre_recurso) {

    pthread_mutex_lock(&MUTEX_RECURSOS);

    t_recurso *recurso = get_recurso(nombre_recurso);
    if (recurso == NULL) {
        log_error(logger_kernel, "Recurso %s no encontrado. Proceso %d enviado a EXIT.", nombre_recurso, pcb->pid);
        enviar_proceso_a_exit(pcb);
        return;
    }

    if (!remover_proceso_de_recurso(nombre_recurso, pcb->pid)) {
        log_error(logger_kernel, "Proceso %d no tiene asignado el recurso %s. Enviado a EXIT.", pcb->pid, nombre_recurso);
        enviar_proceso_a_exit(pcb);
        return;
    }

    incrementar_recurso(recurso);
    log_info(logger_kernel, "Recurso %s liberado por proceso %d. Instancias actuales: %d", nombre_recurso, pcb->pid, recurso->instancias);


    t_PCB *pcb_desbloqueado = NULL;
    bool desbloquear = !queue_is_empty(recurso->cola_bloqueados);
    if (desbloquear) {
        pcb_desbloqueado = desbloquear_proceso(recurso);
        log_info(logger_kernel, "Proceso %d desbloqueado por SIGNAL de recurso %s", pcb_desbloqueado->pid, nombre_recurso);
    }

    pthread_mutex_unlock(&MUTEX_RECURSOS);

    if (desbloquear) {
        handle_wait(pcb_desbloqueado, nombre_recurso, true);
    }

    send_pcb_cpu(pcb);
    
}



void enviar_proceso_a_exit(t_PCB* pcb){
        execute_to_null();
        cancelar_quantum_si_corresponde(pcb);
        agregar_a_cola_exit(pcb);
        sem_post(&SEM_CPU); 
} 