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

void send_to_exit(t_PCB *pcb)
{
    pthread_mutex_lock(&MUTEX_EXIT);
    // -------------------------------------------------------------------
    // agregar logica de liberar recursos de prcesos de ser necesario AQUI...
    u_int32_t pid = pcb->pid;
    liberar_recursos_de_proceso(pid);
    // -------------------------------------------------------------------
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

        // TODO eliminar otros contextos en tablas

        pcb_destroy(pcb_exit);
    }
}

// manejo de chequeo de recursos al finalizar un proceso:
void liberar_recursos_de_proceso(u_int32_t pid){
// chequeo que el proceso es valido y se encuentra en el diccionario de pcbs
// chequeo que el proceso tiene recursos asignados
// si tiene les hago signal a los recursos en cuestion que no se habian liberado.


}