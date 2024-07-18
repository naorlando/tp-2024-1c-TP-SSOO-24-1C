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
    agregar_a_cola_new(pcb);

    log_info(logger_kernel, "Se crea el proceso <%d> en NEW", pcb->pid);
    log_info(logger_kernel, "PID: %d - Estado Anterior: %s - Estado Actual: %s", pcb->pid, "-", "NEW");
    // log_info(logger_kernel, "Cola NEW tiene un total de %d elementos", queue_size(COLA_NEW));
    sem_post(&SEM_NEW);
}

void send_new_to_ready()
{
    while (1)
    {
        sem_wait(&SEM_NEW);
        sem_wait(&SEM_MULTIPROGRAMACION);

        // descolar pcb de NEW
        t_PCB *pcb = siguiente_pcb_cola_new();

        log_info(logger_kernel, "Se paso el PCB <%d> de NEW -> READY ",pcb->pid);
        log_info(logger_kernel, "Cola NEW tiene un total de %d elementos", queue_size(COLA_NEW));

        // encolar en ready
        agregar_de_new_a_ready(pcb);
        log_info(logger_kernel, "Cola READY tiene un total de %d elementos", queue_size(COLA_READY));
    }
}

void end_process()
{
    while (1)
    {
        sem_wait(&SEM_EXIT);
        t_PCB *pcb_exit;  

        log_info(logger_kernel, "empieza a terminar el proceso");
        //TODO: Hacer una abstraccion para obtener el siguiente PCB de la cola de EXIT
        pthread_mutex_lock(&MUTEX_EXIT);
        pcb_exit = queue_pop(COLA_EXIT);
        pthread_mutex_unlock(&MUTEX_EXIT);
        sem_post(&SEM_MULTIPROGRAMACION);
        // -------------------------------------------------------------------
        // agregar logica de liberar recursos de prcesos de ser necesario AQUI...
        // chequeo de recursos no liberados:
        liberar_recursos_de_proceso(pcb_exit->pid);
        dictionary_remove_and_destroy(recursos_asignados_por_pid, uint32_to_string(pcb_exit->pid), (void *)list_destroy);

        // -------------------------------------------------------------------

        // TODO eliminar otros contextos en tablas

        // liberar memoria de pcb:
        pcb_destroy(pcb_exit);
    }
}