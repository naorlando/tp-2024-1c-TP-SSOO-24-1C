#include "largo_plazo.h"

void planificador_largo_plazo()
{
    pthread_t hilo_newAReady;
    pthread_t hilo_exit;

    pthread_create(&hilo_newAReady, NULL, (void *)send_new_to_ready, NULL);
    pthread_create(&hilo_exit, NULL, (void *)end_process, NULL);

    pthread_detach(hilo_newAReady);
    pthread_detach(hilo_exit);
}

void process_to_new(t_PCB *pcb)
{

    pthread_mutex_lock(&MUTEX_NEW);
    queue_push(COLA_NEW, pcb);
    pthread_mutex_unlock(&MUTEX_NEW);
    log_info(logger_kernel, "PID: %d - Estado Anterior: %s - Estado Actual: %s", pcb->pid, "-", "NEW");
    log_info(logger_kernel, "Cola NEW tiene un total de %d elementos", queue_size(COLA_NEW));
    sem_post(&SEM_NEW);
}

void send_new_to_ready()
{
    while (1)
    {
        sem_wait(&SEM_NEW);
        sem_wait(&SEM_MULTIPROGRAMACION);

        // descolar pcb de NEW
        t_PCB *pcb;
        pthread_mutex_lock(&MUTEX_NEW);
        pcb = queue_pop(COLA_NEW);
        pthread_mutex_unlock(&MUTEX_NEW);
        log_info(logger_kernel, "Se paso un PCB de NEW -> READY \nCola NEW tiene un total de %d elementos", queue_size(COLA_NEW));

        // encolar en ready
    // antes:
        // pthread_mutex_lock(&MUTEX_READY);
        // queue_push(COLA_READY, pcb);
        // pthread_mutex_unlock(&MUTEX_READY);
        // log_info(logger_kernel, "Cola READY tiene un total de %d elementos", queue_size(COLA_READY));
        // sem_post(&SEM_READY); //POST READY
    //despues:
        agregar_a_cola_ready(pcb);
    }
}

// deprecada
void send_to_exit(t_PCB *pcb)
{
    pthread_mutex_lock(&MUTEX_EXIT);
    queue_push(COLA_EXIT, pcb);
    pthread_mutex_unlock(&MUTEX_EXIT);
    sem_post(&SEM_EXIT);
}

void end_process()
{
    while (1)
    {
        sem_wait(&SEM_EXIT);
        t_PCB *pcb_exit;  

        log_info(logger_kernel, "empieza a terminar el proceso");
        pthread_mutex_lock(&MUTEX_EXIT);
        pcb_exit = queue_pop(COLA_EXIT);
        pthread_mutex_unlock(&MUTEX_EXIT);
        sem_post(&SEM_MULTIPROGRAMACION);
        // -------------------------------------------------------------------
        // agregar logica de liberar recursos de prcesos de ser necesario AQUI...
        // chequeo de recursos no liberados:
        liberar_recursos_de_proceso(pcb_exit->pid);
        // -------------------------------------------------------------------

        // TODO eliminar otros contextos en tablas

        // liberar memoria de pcb:
        pcb_destroy(pcb_exit);
    }
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