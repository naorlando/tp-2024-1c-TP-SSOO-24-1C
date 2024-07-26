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

// ############################################################################################################
//  Manejo de colas de bloqueados:
// ############################################################################################################
void bloquear_proceso(t_recurso *recurso, t_PCB *pcb) 
{
    // semaforo: de DETENER_PLANIFICACION.
    pthread_mutex_lock(&recurso->mutex_cola_bloqueados);
        //queue_push(recurso->cola_bloqueados, pcb);
        pcb->state = BLOCKED;
        list_add(recurso->cola_bloqueados, pcb);
    pthread_mutex_unlock(&recurso->mutex_cola_bloqueados);
    log_info(logger_kernel, "Proceso %d bloqueado por recurso %s", pcb->pid, recurso->nombre);
}

t_PCB* desbloquear_proceso(t_recurso *recurso) 
{
    pthread_mutex_lock(&recurso->mutex_cola_bloqueados);
        //t_PCB* pcb = queue_pop(recurso->cola_bloqueados);
        t_PCB* pcb = list_remove(recurso->cola_bloqueados, 0);
    pthread_mutex_unlock(&recurso->mutex_cola_bloqueados);

    log_info(logger_kernel, "Proceso %d bloqueado por recurso %s", pcb->pid, recurso->nombre);
    return pcb;
}
// ----------------------------------------------------------------------------------------------

void asignar_recurso_a_proceso(char *nombre_recurso, uint32_t pid) 
{
    t_list *recursos = dictionary_get(recursos_asignados_por_pid, uint32_to_string(pid));
    if (recursos == NULL) {
        recursos = list_create();
        dictionary_put(recursos_asignados_por_pid, uint32_to_string(pid), recursos);
    }
    list_add(recursos, (void *) nombre_recurso);
    print_dictionary();
}

void free_resource(t_PCB *pcb) 
{
    log_info(logger_kernel, "Liberando recursos del proceso %d", pcb->pid);

    // Liberar recursos asociados al proceso
    liberar_recursos_de_proceso(pcb->pid);

    // Esta logica se usa para cunado se manda un pcb a exit y se quiere sacar completamenteel pid del diccionario: recursos_asignados_por_pid
    // Verificar si el proceso está en el diccionario antes de eliminarlo: 
    char* pid_str = uint32_to_string(pcb->pid);
    if (dictionary_has_key(recursos_asignados_por_pid, pid_str)) {
        dictionary_remove_and_destroy(recursos_asignados_por_pid, pid_str, (void *)list_destroy);
        log_info(logger_kernel, "Proceso %d eliminado del diccionario de recursos asignados.", pcb->pid);
    }
    free(pid_str);
}



// manejo de chequeo de recursos al finalizar un proceso:
void liberar_recursos_de_proceso(u_int32_t pid) {
    // Verificar si el proceso es válido y se encuentra en el diccionario de PCBs
    if (get_pcb(pid) == NULL) {
        log_error(logger_kernel, "No se encontró el proceso (con PID: %d) en el diccionario de PCBs.", pid);
        return;
    }

    // Verificar si el proceso tiene recursos asignados
    char* pid_str = uint32_to_string(pid);
    t_list *recursos = dictionary_get(recursos_asignados_por_pid, pid_str);
    if (recursos == NULL) {
        log_info(logger_kernel, "El proceso %d no tiene recursos asignados, se puede eliminar tranquilamente.", pid);
        free(pid_str);
        return;
    }

    // Si tiene recursos, liberar los recursos no previamente liberados
    while (!list_is_empty(recursos)) {
        char *nombre_recurso = (char *)list_remove(recursos, 0);
        t_recurso *recurso = get_recurso(nombre_recurso);
        if (recurso != NULL) {
            incrementar_recurso(recurso);
            log_info(logger_kernel, "Se libera recurso %s (SIN PREVIAMENTE SER LIBERADO) del PID = %d", nombre_recurso, pid);
            remover_recurso_de_proceso(nombre_recurso, pid);
            if (!list_is_empty(recurso->cola_bloqueados)) {
                t_PCB *pcb_desbloqueado = desbloquear_proceso(recurso);
                log_info(logger_kernel, "Proceso %d desbloqueado por SIGNAL de recurso %s", pcb_desbloqueado->pid, nombre_recurso);
                handle_wait(pcb_desbloqueado, nombre_recurso, true);
            }
        }
    }

    list_destroy(recursos);  // Destruir la lista de recursos
    free(pid_str);
}

bool remover_recurso_de_proceso(char *nombre_recurso, uint32_t pid) 
{
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

void remover_proceso_de_colas_bloqueados(uint32_t pid) {
    void remover_proceso(char *key, void *value) {
        t_recurso *recurso = (t_recurso *)value;
        bool pid_match(void *pcb_ptr) {
            return ((t_PCB *)pcb_ptr)->pid == pid;
        }

        pthread_mutex_lock(&recurso->mutex_cola_bloqueados); // Asegúrate de tener un mutex para cada cola de bloqueados
        t_PCB *pcb_removido = (t_PCB *)list_remove_by_condition(recurso->cola_bloqueados, pid_match);
        pthread_mutex_unlock(&recurso->mutex_cola_bloqueados);

        if (pcb_removido != NULL) {
            log_info(logger_kernel, "Proceso %d removido de la cola de bloqueados del recurso %s", pcb_removido->pid, recurso->nombre);
        } else {
            log_info(logger_kernel, "Proceso %d no se encontró en la cola de bloqueados del recurso %s", pcb_removido->pid, recurso->nombre);
        }
    }
    dictionary_iterator(recursos_dictionary, remover_proceso);
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
        asignar_recurso_a_proceso(nombre_recurso, pcb->pid);
        // ----------------------------------------------------------------------------------------------
        log_info(logger_kernel, "Recurso %s tenía instancias disponibles. Instancias restantes: %d", nombre_recurso, recurso->instancias);
        // from_signal = true   -> viene de desbloquear un proceso de la cola de bloqueados del recurso.
        // from_signal = false  -> viene de cpu.
        if (!from_signal) {
            send_pcb_cpu(pcb);
        } else {
            agregar_de_blocked_a_ready(pcb);
            update_pcb(pcb);
        }
    } else {
        enviar_proceso_a_cola_bloqueados(recurso, pcb);
        execute_to_null();
        log_info(logger_kernel, "Recurso %s no tenía instancias disponibles. Proceso %d bloqueado", nombre_recurso, pcb->pid);
        sem_post(&SEM_CPU);    
    }

    pthread_mutex_unlock(&MUTEX_RECURSOS);
}

void enviar_proceso_a_cola_bloqueados(t_recurso* recurso,t_PCB* pcb)
{
    sem_wait(&SEM_PLANIFICACION_BLOCKED_INICIADA);
    bloquear_proceso(recurso, pcb);
    cancelar_quantum_si_corresponde(pcb);
    actualizar_quantum(pcb);
    update_pcb(pcb);
    sem_post(&SEM_PLANIFICACION_BLOCKED_INICIADA);
}

void handle_signal(t_PCB *pcb, char *nombre_recurso) {

    pthread_mutex_lock(&MUTEX_RECURSOS);

    t_recurso *recurso = get_recurso(nombre_recurso);
    if (recurso == NULL) {
        log_error(logger_kernel, "Recurso %s no encontrado. Proceso %d enviado a EXIT.", nombre_recurso, pcb->pid);
        enviar_proceso_a_exit(pcb);
        return;
    }

    if (!remover_recurso_de_proceso(nombre_recurso, pcb->pid)) {
        log_error(logger_kernel, "Proceso %d no tiene asignado el recurso %s. Enviado a EXIT.", pcb->pid, nombre_recurso);
        enviar_proceso_a_exit(pcb);
        return;
    }

    incrementar_recurso(recurso);
    log_info(logger_kernel, "Recurso %s liberado por proceso %d. Instancias actuales: %d", nombre_recurso, pcb->pid, recurso->instancias);


    t_PCB *pcb_desbloqueado = NULL;
    bool desbloquear = !list_is_empty(recurso->cola_bloqueados);
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

void enviar_proceso_a_exit(t_PCB* pcb)
{
    execute_to_null();
    cancelar_quantum_si_corresponde(pcb);
    agregar_a_cola_exit(pcb);
    cronometro_reiniciar();
    update_pcb(pcb);
    sem_post(&SEM_CPU); 
} 