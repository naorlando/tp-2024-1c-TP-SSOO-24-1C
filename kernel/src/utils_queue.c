#include "utils_queue.h"

void agregar_de_execute_a_ready(t_PCB* pcb)
{
    if(strcmp(obtener_algoritmo_planificacion(kernel_config), "VRR") == 0) {
        agregar_a_cola_ready_VRR(pcb);
    } else {
        agregar_a_cola_ready(pcb);
    }
}

void agregar_a_cola_ready_VRR(t_PCB* pcb){
    if (cronometro_obtener_tiempo() < pcb->quantum) {
            pcb->quantum -= cronometro_obtener_tiempo() ;
            agregar_a_cola_aux_ready(pcb);
    } else {
            pcb->quantum = obtener_quantum(kernel_config);
            agregar_a_cola_ready(pcb);
    }
}

void agregar_a_cola_ready(t_PCB* pcb) 
{
    pthread_mutex_lock(&MUTEX_READY);
        pcb->state = READY;
        queue_push(COLA_READY, pcb);
    pthread_mutex_unlock(&MUTEX_READY);

    sem_post(&SEM_READY);
}

void agregar_a_cola_aux_ready(t_PCB* pcb) 
{
    pthread_mutex_lock(&MUTEX_AUX_READY);
        pcb->state = READY;
        queue_push(COLA_AUX_READY, pcb);
    pthread_mutex_unlock(&MUTEX_AUX_READY);

    sem_post(&SEM_AUX_READY);
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

