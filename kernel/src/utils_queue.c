#include "utils_queue.h"

void agregar_a_cola_ready(t_PCB* pcb) 
{
    pthread_mutex_lock(&MUTEX_READY);
        pcb->state = READY;
        queue_push(COLA_READY, pcb);
    pthread_mutex_unlock(&MUTEX_READY);

    sem_post(&SEM_READY);
}

void agregar_de_new_a_ready(t_PCB* pcb)
{
    agregar_a_cola_ready(pcb);
}

void agregar_de_blocked_a_ready(t_PCB* pcb)
{
    char* algoritmo_plani = obtener_algoritmo_planificacion(kernel_config);

    if(strcmp(algoritmo_plani, "FIFO") == 0){
        agregar_a_cola_ready(pcb);
    }
    else if(strcmp(algoritmo_plani, "RR") == 0){
        agregar_a_cola_ready(pcb);
    }
    else if(strcmp(algoritmo_plani, "VRR") == 0){
        //TODO: Se debe agregar a la cola auxiliar?

    }
}

void agregar_a_cola_exit(t_PCB* pcb){
    pthread_mutex_lock(&MUTEX_EXIT);
        pcb->state = FINISHED;
        queue_push(COLA_EXIT, pcb);
        //log_debug(logger_kernel, "Se agregó el proceso %d a la cola EXIT", pcb->pid);
    pthread_mutex_unlock(&MUTEX_EXIT);
    sem_post(&SEM_EXIT);
}

void agregar_a_cola_aux_ready(t_PCB* pcb) 
{
    pthread_mutex_lock(&MUTEX_AUX_READY);
        pcb->state = READY;
        queue_push(COLA_AUX_READY, pcb);
    pthread_mutex_unlock(&MUTEX_AUX_READY);

    sem_post(&SEM_AUX_READY);
}