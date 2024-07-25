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
        if(list_is_empty(COLA_NEW)){
            continue;
        }
        
        sem_wait(&SEM_MULTIPROGRAMACION);
        // Control de PLANIFICACION:
        sem_wait(&SEM_PLANIFICACION_NEW_READY_INICIADA);
        
        // descolar pcb de NEW
        t_PCB *pcb = siguiente_pcb_cola_new();
        // if(pcb->state == FINISHED){
        //     sem_post(&SEM_MULTIPROGRAMACION);
        //     continue;
        // }


        log_info(logger_kernel, "Se paso el PCB <%d> de NEW -> READY ",pcb->pid);
        //log_info(logger_kernel, "Cola NEW tiene un total de %d elementos", queue_size(COLA_NEW));
        log_info(logger_kernel, "Cola NEW tiene un total de %d elementos", list_size(COLA_NEW));

        // encolar en ready
        agregar_de_new_a_ready(pcb);

        sem_post(&SEM_PLANIFICACION_NEW_READY_INICIADA);
        //log_info(logger_kernel, "Cola READY tiene un total de %d elementos", queue_size(COLA_READY));
        log_info(logger_kernel, "Cola READY tiene un total de %d elementos", list_size(COLA_READY));
    }
}

void end_process()
{
    while (1)
    {
        sem_wait(&SEM_EXIT);
        t_PCB *pcb_exit;  

        // log_info(logger_kernel, "empieza a terminar el proceso");
        // tomar pcb de exit
        pcb_exit = get_next_pcb_exit();
        sem_post(&SEM_MULTIPROGRAMACION);
        // -------------------------------------------------------------------
        // chequeo de recursos no liberados:
        free_resource(pcb_exit);
        // -------------------------------------------------------------------
        // Eliminamos bloque de memoria asignada
        send_msg_kernel_memoria_end_process(pcb_exit->pid);
        // liberar memoria de pcb: TODO: chequear donde liberar PCBs
        // pcb_destroy(pcb_exit);
    }
}