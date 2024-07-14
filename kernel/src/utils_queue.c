#include "utils_queue.h"

void agregar_a_cola_ready(t_PCB* pcb) 
{
    pthread_mutex_lock(&MUTEX_READY);
    queue_push(COLA_READY, pcb);
    pthread_mutex_unlock(&MUTEX_READY);

    sem_post(&SEM_READY);
}

void agregar_a_cola_new(t_PCB* pcb)
{
    pthread_mutex_lock(&MUTEX_NEW);
    queue_push(COLA_NEW, pcb);
    pthread_mutex_unlock(&MUTEX_NEW);
}

t_PCB* siguiente_pcb_cola_new()
{
    pthread_mutex_lock(&MUTEX_NEW);
    t_PCB* pcb = queue_pop(COLA_NEW);
    pthread_mutex_unlock(&MUTEX_NEW);

    return pcb;
}

t_PCB* siguiente_pcb_cola_ready()
{
    pthread_mutex_lock(&MUTEX_READY);
    t_PCB* pcb = queue_pop(COLA_READY);
    pthread_mutex_unlock(&MUTEX_READY);

    return pcb;
}

void agregar_de_new_a_ready(t_PCB* pcb)
{
    agregar_a_cola_ready(pcb);
}

void agregar_a_ready_fin_quantum(t_PCB* pcb)
{
    char* algoritmo_plani = obtener_algoritmo_planificacion(kernel_config);

    if(strcmp(algoritmo_plani, "RR") == 0){
        agregar_a_cola_ready(pcb);
    }
    else if(strcmp(algoritmo_plani, "VRR") == 0){
        //TODO: Se debe agregar a la cola auxiliar?
    }
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
    queue_push(COLA_EXIT, pcb);
    pthread_mutex_unlock(&MUTEX_EXIT);
    sem_post(&SEM_EXIT);
}