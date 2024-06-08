#include "largo_plazo.h"

void planificador_largo_plazo()
{
    pthread_t hilo_newAReady;
    // pthread_t hilo_exit;

    pthread_create(&hilo_newAReady, NULL, (void *)send_new_to_ready, NULL);
    // pthread_create(&hilo_exit,NULL,(void*) terminarProcesos,NULL);

    pthread_detach(hilo_newAReady);
    // pthread_detach(hilo_exit);
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

        // int sem_value;
        // if (sem_getvalue(&SEM_MULTIPROGRAMACION, &sem_value) == 0)
        // {
        //     log_info(logger_kernel, "Valor del semáforo %s: %d", "SEM_MULTIPROGRAMACION", sem_value);
        // }

        // log_trace(logger_kernel, "Esperando SEM_NEW");
        sem_wait(&SEM_NEW);
        sem_wait(&SEM_MULTIPROGRAMACION);

        // descolar pcb de NEW
        t_PCB *pcb;
        pthread_mutex_lock(&MUTEX_NEW);
        pcb = queue_pop(COLA_NEW);
        pthread_mutex_unlock(&MUTEX_NEW);

        // encolar en ready
        pthread_mutex_lock(&MUTEX_READY);
        queue_push(COLA_READY, pcb);
        pthread_mutex_unlock(&MUTEX_READY);
        sem_post(&SEM_READY);
        log_info(logger_kernel, "Cola READY tiene un total de %d elementos", queue_size(COLA_READY));
    }
}