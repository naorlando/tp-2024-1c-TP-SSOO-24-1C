#include "corto_plazo.h"

t_PCB* get_next_pcb_to_exec()
{
    t_PCB *pcb_a_tomar;
    if (strcasecmp(kernel_config->ALGORITMO_PLANIFICACION, "FIFO") == 0)
    {
        pthread_mutex_lock(&MUTEX_READY);
        pcb_a_tomar = queue_pop(COLA_READY);
        pthread_mutex_unlock(&MUTEX_READY);
        return pcb_a_tomar;
    }
    log_error(logger_kernel, "algoritmo_planificación invalido");
    exit(EXIT_FAILURE);
}

void planificador_corto_plazo()
{
    while (1)
    {
        sem_wait(&SEM_READY);

        t_PCB *pcb = get_next_pcb_to_exec();

        log_info(logger_kernel, "PID: %d - Estado Anterior: %s - Estado Actual: %s", pcb->pid, "READY", "EXEC");
        sem_wait(&SEM_CPU);
        send_pcb_cpu(pcb);
        sem_post(&SEM_CPU);
        
    }
}