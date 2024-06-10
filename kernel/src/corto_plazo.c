#include "corto_plazo.h"

t_PCB *get_next_pcb_to_exec()
{
    t_PCB *pcb_a_tomar;
    switch (_obtener_planificador(kernel_config->ALGORITMO_PLANIFICACION))
    {
    case FIFO:
        break;
    case RR:
        /* ordenar */
        break;
    case VRR:
        /* ordenar */
        break;

    default:
        log_error(logger_kernel, "algoritmo_planificación invalido");
        exit(EXIT_FAILURE);
        break;
    }

    pthread_mutex_lock(&MUTEX_READY);
    pcb_a_tomar = queue_pop(COLA_READY);
    pthread_mutex_unlock(&MUTEX_READY);
    return pcb_a_tomar;
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

t_planificador _obtener_planificador(char *str)
{

    if (strcmp(str, "FIFO") == 0)
        return FIFO;
    if (strcmp(str, "RR") == 0)
        return RR;
    if (strcmp(str, "VRR") == 0)
        return VRR;
    return -1;
}