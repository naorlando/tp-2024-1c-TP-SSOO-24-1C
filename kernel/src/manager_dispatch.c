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

    log_info(logger_kernel, "Se recibio una solicitud de CPU a una IO GENERICA para el PCB de PID <%d>", pcb_io_gen->pid);
    
    if(strcmp(obtener_algoritmo_planificacion(kernel_config), "FIFO") != 0) {
        cancelar_hilo_quantum(pcb_io_gen->pid);
    }

    //set_solicitud(SOLICITUD_GENERICA, solicitud_gen);
    // add_new_solicitud(SOLICITUD_GENERICA, solicitud_gen);
    // sem_post(&SEM_BLOCKED);
    sem_post(&SEM_CPU);
    
    //1. Validar que la IO GENERICA este conectada
    //2. 
    //  a) Si esta conectada, encolar el PCB a la cola de bloqueo correspondiente (Sigue en 3 el flujo)
    //  b) Si no esta conectada, pasar a EXIT el PCB. (Termina el flujo aca)
    //3. Enviar el struct t_io_generica a la IO GENERICA si esta libre
    //4. Bloquear el envio de otra solicitud de IO GENERICA, hasta que la IO responda luego de su procesamiento
}

void procesar_pcb_exit()
{
    //TODO: ARMAR UNA FUNCION QUE SE ENCARGUE DE LA GESTION DE LIBERAR EL PCB QUE LLEGO A EXIT
    // YA QUE PUEDE TENER RECURSOS ASIGNADOS Y MEMORIA
    t_PCB* pcb_exit= recv_pcb_cpu(); // DATO: aca dentro se crea otro malloc de pcb(se usa pcb_create())..

    execute_to_null();
    cancelar_quantum_si_corresponde(pcb_exit);

    log_info(logger_kernel, "Llego a EXIT el PCB de PID <%d>", pcb_exit->pid);

    // Actualizo el estado del pcb en la cola correspondiente:
    agregar_a_cola_exit(pcb_exit);
    
    cronometro_reiniciar(); // funciona en caso de VRR
    
    // actualizar el pcb en la tabla de pcb:
    update_pcb(pcb_exit);
    log_info(logger_kernel, "Se actualizo el PCB de PID: <%d> en la table_pcb", pcb_exit->pid);
    
    sem_post(&SEM_CPU);

    log_info(logger_kernel, "La cola de Ready tiene %d elementos", queue_size(COLA_READY));
    log_info(logger_kernel, "La cola de Exit tiene %d elementos", queue_size(COLA_EXIT));
}

void procesar_interrupcion_quantum()
{
    //TODO: agregar PCB donde este:
    // 1-recibir pcb:
    t_PCB* pcb_interrupt = recv_pcb_interrupt();

    log_info(logger_kernel, "Se recibio un PCB por interrupcion de QUANTUM a traves del CPU_DISPATCH, PID: <%d>", pcb_interrupt->pid);

    cronometro_detener(); // funciona en caso de VRR

    // 2-actualizar el estado del pcb en la cola correspondiente:
    agregar_de_execute_a_ready(pcb_interrupt);

    // 3-actualizar el pcb en la tabla de pcb:
    update_pcb(pcb_interrupt);
    log_info(logger_kernel, "Se actualizo el PCB de PID: <%d> en la table_pcb", pcb_interrupt->pid);


    log_info(logger_kernel, "Se actualizo el estado del PCB de PID: <%d> en la cola READY", pcb_interrupt->pid);
    log_info(logger_kernel, "La cola de Ready tiene %d elementos", queue_size(COLA_READY));
    sem_post(&SEM_CPU);
}

// manejar instruccion WAIT
void handle_wait_request(){
    //recibo el paquete con el recurso a manejar
    pthread_mutex_lock(&MUTEX_RECURSOS);

    t_manejo_recurso * manejo_recurso_recv = recv_wait_or_signal_request();

    t_PCB *pcb = manejo_recurso_recv->pcb;
    char *nombre_recurso = manejo_recurso_recv->nombre_recurso;

    log_info(logger_kernel, "AX: %u", pcb->cpu_registers->ax);
    log_info(logger_kernel, "BX: %u", pcb->cpu_registers->bx);
    log_info(logger_kernel, "CX: %u", pcb->cpu_registers->cx);
    log_info(logger_kernel, "Nombre de recurso: %s", nombre_recurso);

    t_recurso *recurso = get_recurso(nombre_recurso);
    if(recurso == NULL) {
        execute_to_null();
        cancelar_quantum_si_corresponde(pcb);
        agregar_a_cola_exit(pcb);
        sem_post(&SEM_CPU);    
        return; 
    }

    if (recurso->instancias > 0) {
        decrementar_recurso(recurso); // recurso->instancias--;
        // ----------------------------------------------------------------------------------------------
        asignar_proceso_a_recurso(nombre_recurso, pcb->pid);
        // ----------------------------------------------------------------------------------------------
        log_info(logger_kernel, "Recurso %s tenía instancias disponibles. Instancias restantes: %d", nombre_recurso, recurso->instancias);
        send_pcb_cpu(pcb);
    } else {
        bloquear_proceso(recurso, pcb);
        log_info(logger_kernel, "Recurso %s no tenía instancias disponibles. Proceso %d bloqueado", nombre_recurso, pcb->pid);
        sem_post(&SEM_CPU);    
    }

    pthread_mutex_unlock(&MUTEX_RECURSOS);
}

// manejar instruccion SIGNAL
void handle_signal_request()
{
    //recibo el paquete con el recurso a manejar
    pthread_mutex_lock(&MUTEX_RECURSOS);

    t_manejo_recurso * manejo_recurso_recv = recv_wait_or_signal_request();

    t_PCB *pcb = manejo_recurso_recv->pcb;
    char *nombre_recurso = manejo_recurso_recv->nombre_recurso;

    log_info(logger_kernel, "AX: %u", pcb->cpu_registers->ax);
    log_info(logger_kernel, "BX: %u", pcb->cpu_registers->bx);
    log_info(logger_kernel, "CX: %u", pcb->cpu_registers->cx);
    log_info(logger_kernel, "Nombre de recurso: %s", nombre_recurso);

    t_recurso *recurso = get_recurso(nombre_recurso);
    if(recurso == NULL) {
        execute_to_null();
        cancelar_quantum_si_corresponde(pcb);
        agregar_a_cola_exit(pcb);
        sem_post(&SEM_CPU);    
        return;
    }

    // Verificar si el proceso que hace SIGNAL está en la lista de procesos asignados
    // si el proceso NO esta en la lista de procesos asignados, lo mando a EXIT...
    if (!remover_proceso_de_recurso(nombre_recurso, pcb->pid)) {
        log_info(logger_kernel, "Proceso %d no tiene asignado el recurso %s. Proceso enviado a EXIT", pcb->pid, nombre_recurso);
        execute_to_null();
        cancelar_quantum_si_corresponde(pcb);
        agregar_a_cola_exit(pcb);
        sem_post(&SEM_CPU);    
        return;
    }
    // si el proceso esta en la lista de procesos asignados,se ELEMININA EL PID en la funcion remove_asignado_a_recurso() e 
    // incremento las instancias del recurso...
    incrementar_recurso(recurso); // recurso->instancias++;
    log_info(logger_kernel, "Recurso %s incrementado. Instancias actuales: %d", nombre_recurso, recurso->instancias);

 

    if (!queue_is_empty(recurso->cola_bloqueados)) {
        t_PCB *pcb_desbloqueado = desbloquear_proceso(recurso);
        //asignar_proceso_a_recurso(nombre_recurso, pcb_desbloqueado->pid);
        log_info(logger_kernel, "Proceso %d desbloqueado por SIGNAL de recurso %s", pcb_desbloqueado->pid, nombre_recurso);
        agregar_a_cola_ready(pcb_desbloqueado);
    }

    send_pcb_cpu(pcb);

    pthread_mutex_unlock(&MUTEX_RECURSOS);
}

void execute_to_null() {
    pthread_mutex_lock(&MUTEX_EXECUTE);
    EXECUTE = NULL;
    pthread_mutex_unlock(&MUTEX_EXECUTE);
}

void cancelar_quantum_si_corresponde(t_PCB *pcb_exit) {
    if (strcmp(obtener_algoritmo_planificacion(kernel_config), "FIFO") != 0) {
        cancelar_hilo_quantum(pcb_exit->pid);
    }
}

void logica_pcb_retorno_vrr(t_PCB *pcb) {
    pthread_mutex_lock(&MUTEX_COLA_RETORNO_PCB);
        queue_push(COLA_RETORNO_PCB, pcb);
    pthread_mutex_unlock(&MUTEX_COLA_RETORNO_PCB);
    sem_post(&SEM_PCB_RETURNS);  // Signal que el PCB ha retornado
}