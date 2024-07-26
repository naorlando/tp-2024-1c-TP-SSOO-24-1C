#include "manager_dispatch.h"

void cancelar_hilo_quantum(uint32_t pid) 
{
    if (get_pid_datos_hilo(datos_hilo_quantum) == pid && !interrupcion_enviada) {
        pthread_cancel(get_hilo(datos_hilo_quantum));
        log_info(logger_kernel, "Se cancela el hilo de quantum para desalojar el PCB de PID: <%d>", pid);
    }
    // Elimino las datos guardados para enviar la interrupcion
    datos_hilo_destroy(datos_hilo_quantum);
}

void procesar_ios_genericas()
{
    t_solicitud_io_generica* solicitud_gen = recv_solicitud_io_generica_cpu();

    t_PCB* pcb_io_gen = obtener_pcb_solicitud_generica(solicitud_gen);
    cancelar_quantum_si_corresponde(pcb_io_gen);
    actualizar_quantum(pcb_io_gen);
    log_info(logger_kernel, "Se recibio una solicitud de CPU a una IO GENERICA para el PCB de PID <%d>", pcb_io_gen->pid);

    proceso_solicita_io(0, solicitud_gen);
    sem_post(&SEM_CPU);
}


void procesar_pcb_exit(t_motivo_exit motivo)
{
    t_PCB* pcb_exit= recv_pcb_cpu(); // DATO: aca dentro se crea otro malloc de pcb(se usa pcb_create())..
    execute_to_null();
    cancelar_quantum_si_corresponde(pcb_exit);

    //log_info(logger_kernel, "Llego a EXIT el PCB de PID <%d>", pcb_exit->pid);

    // Actualizo el estado del pcb en la cola correspondiente:
    agregar_a_cola_exit(pcb_exit);
    
    // LOG OBLIGATORIO:
    log_info(logger_kernel, "Finaliza el proceso <%d> - Motivo: %s", pcb_exit->pid, obtener_motivo_exit(motivo));

    cronometro_reiniciar(); // funciona en caso de VRR
    // actualizar el pcb en la tabla de pcb:
    update_pcb(pcb_exit);
    log_info(logger_kernel, "Se actualizo el PCB de PID: <%d> en la table_pcb", pcb_exit->pid);
    
    sem_post(&SEM_CPU);

    //log_info(logger_kernel, "La cola de Ready tiene %d elementos", queue_size(COLA_READY));
    //log_info(logger_kernel, "La cola de Exit tiene %d elementos", queue_size(COLA_EXIT));

    log_info(logger_kernel, "La cola de Ready tiene %d elementos", list_size(COLA_READY));
    log_info(logger_kernel, "La cola de Exit tiene %d elementos", list_size(COLA_EXIT));
}

void procesar_interrupcion_quantum()
{
    //TODO: agregar PCB donde este:
    // 1-recibir pcb:
    t_PCB* pcb_interrupt = recv_pcb_interrupt();

    log_info(logger_kernel, "Se recibio un PCB por interrupcion de QUANTUM a traves del CPU_DISPATCH, PID: <%d>", pcb_interrupt->pid);

    // cronometro_detener(); // funciona en caso de VRR
 
    // 2-actualizar el estado del pcb en la cola correspondiente:
    actualizar_quantum(pcb_interrupt);
    agregar_de_execute_a_ready(pcb_interrupt);

    // 3-actualizar el pcb en la tabla de pcb:
    update_pcb(pcb_interrupt);
    log_info(logger_kernel, "Se actualizo el PCB de PID: <%d> en la table_pcb", pcb_interrupt->pid);


    log_info(logger_kernel, "Se actualizo el estado del PCB de PID: <%d> en la cola READY", pcb_interrupt->pid);
    //log_info(logger_kernel, "La cola de Ready tiene %d elementos", queue_size(COLA_READY));
    log_info(logger_kernel, "La cola de Ready tiene %d elementos", list_size(COLA_READY));
    sem_post(&SEM_CPU);
}

void handle_wait_request() {
    //recibo el paquete con el recurso a manejar    

    t_manejo_recurso *manejo_recurso_recv = recv_wait_or_signal_request();

    t_PCB *pcb = manejo_recurso_recv->pcb;
    char *nombre_recurso = manejo_recurso_recv->nombre_recurso;

    log_info(logger_kernel, "AX: %u", pcb->cpu_registers->ax);
    log_info(logger_kernel, "BX: %u", pcb->cpu_registers->bx);
    log_info(logger_kernel, "CX: %u", pcb->cpu_registers->cx);
    log_info(logger_kernel, "Nombre de recurso: %s", nombre_recurso);

    handle_wait(pcb, nombre_recurso,false);

}

void handle_signal_request() {

    t_manejo_recurso *manejo_recurso_recv = recv_wait_or_signal_request();
    t_PCB *pcb = manejo_recurso_recv->pcb;
    char *nombre_recurso = manejo_recurso_recv->nombre_recurso;

    log_info(logger_kernel, "AX: %u", pcb->cpu_registers->ax);
    log_info(logger_kernel, "BX: %u", pcb->cpu_registers->bx);
    log_info(logger_kernel, "CX: %u", pcb->cpu_registers->cx);
    log_info(logger_kernel, "Nombre de recurso: %s", nombre_recurso);

    handle_signal(pcb, nombre_recurso);

}

// // manejar instruccion WAIT
// void handle_wait_request(){
//     //recibo el paquete con el recurso a manejar
//     pthread_mutex_lock(&MUTEX_RECURSOS);

//     t_manejo_recurso * manejo_recurso_recv = recv_wait_or_signal_request();

//     t_PCB *pcb = manejo_recurso_recv->pcb;
//     char *nombre_recurso = manejo_recurso_recv->nombre_recurso;

//     log_info(logger_kernel, "AX: %u", pcb->cpu_registers->ax);
//     log_info(logger_kernel, "BX: %u", pcb->cpu_registers->bx);
//     log_info(logger_kernel, "CX: %u", pcb->cpu_registers->cx);
//     log_info(logger_kernel, "Nombre de recurso: %s", nombre_recurso);

//     t_recurso *recurso = get_recurso(nombre_recurso);
//     if(recurso == NULL) {
//         execute_to_null();
//         cancelar_quantum_si_corresponde(pcb);
//         agregar_a_cola_exit(pcb);
//         sem_post(&SEM_CPU);    
//         return; 
//     }

//     if (recurso->instancias > 0) {
//         decrementar_recurso(recurso); // recurso->instancias--;
//         // ----------------------------------------------------------------------------------------------
//         asignar_proceso_a_recurso(nombre_recurso, pcb->pid);
//         // ----------------------------------------------------------------------------------------------
//         log_info(logger_kernel, "Recurso %s tenía instancias disponibles. Instancias restantes: %d", nombre_recurso, recurso->instancias);
//         send_pcb_cpu(pcb);
//     } else {
//         bloquear_proceso(recurso, pcb);
//         log_info(logger_kernel, "Recurso %s no tenía instancias disponibles. Proceso %d bloqueado", nombre_recurso, pcb->pid);
//         sem_post(&SEM_CPU);    
//     }

//     pthread_mutex_unlock(&MUTEX_RECURSOS);
// }


// manejar instruccion SIGNAL
// void handle_signal_request()
// {
//     //recibo el paquete con el recurso a manejar
//     pthread_mutex_lock(&MUTEX_RECURSOS);

//     t_manejo_recurso * manejo_recurso_recv = recv_wait_or_signal_request();

//     t_PCB *pcb = manejo_recurso_recv->pcb;
//     char *nombre_recurso = manejo_recurso_recv->nombre_recurso;

//     log_info(logger_kernel, "AX: %u", pcb->cpu_registers->ax);
//     log_info(logger_kernel, "BX: %u", pcb->cpu_registers->bx);
//     log_info(logger_kernel, "CX: %u", pcb->cpu_registers->cx);
//     log_info(logger_kernel, "Nombre de recurso: %s", nombre_recurso);

//     t_recurso *recurso = get_recurso(nombre_recurso);
//     if(recurso == NULL) {
//         execute_to_null();
//         cancelar_quantum_si_corresponde(pcb);
//         agregar_a_cola_exit(pcb);
//         sem_post(&SEM_CPU);    
//         return;
//     }

//     // Verificar si el proceso que hace SIGNAL está en la lista de procesos asignados
//     // si el proceso NO esta en la lista de procesos asignados, lo mando a EXIT...
//     if (!remover_proceso_de_recurso(nombre_recurso, pcb->pid)) {
//         log_info(logger_kernel, "Proceso %d no tiene asignado el recurso %s. Proceso enviado a EXIT", pcb->pid, nombre_recurso);
//         execute_to_null();
//         cancelar_quantum_si_corresponde(pcb);
//         agregar_a_cola_exit(pcb);
//         sem_post(&SEM_CPU);    
//         return;
//     }
//     // si el proceso esta en la lista de procesos asignados,se ELEMININA EL PID en la funcion remove_asignado_a_recurso() e 
//     // incremento las instancias del recurso...
//     incrementar_recurso(recurso); // recurso->instancias++;
//     log_info(logger_kernel, "Recurso %s incrementado. Instancias actuales: %d", nombre_recurso, recurso->instancias);

 

//     if (!queue_is_empty(recurso->cola_bloqueados)) {
//         t_PCB *pcb_desbloqueado = desbloquear_proceso(recurso);
//         //asignar_proceso_a_recurso(nombre_recurso, pcb_desbloqueado->pid);
//         log_info(logger_kernel, "Proceso %d desbloqueado por SIGNAL de recurso %s", pcb_desbloqueado->pid, nombre_recurso);
//         agregar_a_cola_ready(pcb_desbloqueado);
//     }

//     send_pcb_cpu(pcb);

//     pthread_mutex_unlock(&MUTEX_RECURSOS);
// }

void cancelar_quantum_si_corresponde(t_PCB *pcb_exit) {
    if (strcmp(obtener_algoritmo_planificacion(kernel_config), "FIFO") != 0) {
        cancelar_hilo_quantum(pcb_exit->pid);
    }
}

void logica_pcb_retorno_vrr(t_PCB *pcb) {
    pthread_mutex_lock(&MUTEX_COLA_RETORNO_PCB);
        //queue_push(COLA_RETORNO_PCB, pcb);
        list_add(COLA_RETORNO_PCB, pcb);
    pthread_mutex_unlock(&MUTEX_COLA_RETORNO_PCB);
    sem_post(&SEM_PCB_RETURNS);  // Signal que el PCB ha retornado
}

//TODO terminar de implementar
void procesar_ios_stdin()
{
    t_solicitud_io_stdin* solicitud_stdin = recv_solicitud_io_stdin_cpu();

    t_PCB* pcb_io_stdin = obtener_pcb_solicitud_stdin(solicitud_stdin);
    cancelar_quantum_si_corresponde(pcb_io_stdin);
    actualizar_quantum(pcb_io_stdin);
    log_info(logger_kernel, "Se recibio una solicitud de CPU a una IO STDIN para el PCB de PID <%d>", pcb_io_stdin->pid);

    proceso_solicita_io(1, solicitud_stdin);
    sem_post(&SEM_CPU);
}

//TODO terminar de implementar
void procesar_ios_stdout(){
    t_solicitud_io_stdout* solicitud_stdout = recv_solicitud_io_stdout_cpu();

    t_PCB* pcb_io_stdout = obtener_pcb_solicitud_stdout(solicitud_stdout);
    cancelar_quantum_si_corresponde(pcb_io_stdout);
    actualizar_quantum(pcb_io_stdout);
    log_info(logger_kernel, "Se recibio una solicitud de CPU a una IO STDOUT para el PCB de PID <%d>", pcb_io_stdout->pid);

    proceso_solicita_io(2, solicitud_stdout);
    sem_post(&SEM_CPU);

}


