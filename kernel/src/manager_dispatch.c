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
    t_solicitud_io_generica* io_gen = recv_solicitud_io_generica_cpu();

    t_PCB* pcb_io_gen = obtener_pcb_solicitud_generica(io_gen);

    log_info(logger_kernel, "Se recibio una solicitud de CPU a una IO GENERICA para el PCB de PID <%d>", pcb_io_gen->pid);
    
    if(strcmp(obtener_algoritmo_planificacion(kernel_config), "FIFO") != 0) {
        cancelar_hilo_quantum(pcb_io_gen->pid);
    }

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
    t_PCB* pcb_exit= recv_pcb_cpu();

    // limpio la variable global
    pthread_mutex_lock(&MUTEX_EXECUTE);
    EXECUTE = NULL;
    pthread_mutex_unlock(&MUTEX_EXECUTE);

    if(strcmp(obtener_algoritmo_planificacion(kernel_config), "FIFO") != 0) {
        cancelar_hilo_quantum(pcb_exit->pid);
    }

    log_info(logger_kernel, "Llego a EXIT el PCB de PID <%d>", pcb_exit->pid);

    // Actualizo el estado del pcb en la cola correspondiente:
    agregar_a_cola_exit(pcb_exit);
    
    sem_post(&SEM_CPU);

    log_info(logger_kernel, "La cola de Ready tiene %d elementos", queue_size(COLA_READY));
    log_info(logger_kernel, "La cola de Exit tiene %d elementos", queue_size(COLA_EXIT));
}

void procesar_interrupcion()
{
    //TODO: agregar PCB donde este:
    // 1-recibir pcb:
    t_PCB* pcb_interrupt = recv_pcb_interrupt();

    pthread_mutex_lock(&MUTEX_EXECUTE);
    EXECUTE = NULL; // SACO EL PCB DE EXECUTE
    pthread_mutex_unlock(&MUTEX_EXECUTE);

    log_info(logger_kernel, "Se recibio un PCB por interrupcion a traves del CPU_DISPATCH, PID: <%d>", pcb_interrupt->pid);

    // 2-actualizar el pcb en la tabla de pcb:
    // actualizar el pcb que ingresa en la tabla de pcbs macheando por pid:
    // hacemos un dictionary_remove_and_destroy() para liberar la memoria del pcb a actualizar...
    // dictionary_remove_and_destroy(table_pcb, string_itoa(pcb_interrupt->pid), (void *)pcb_destroy);
    
    // Elimino el PCB de la tabla de pcbs que gestiona el Kernel
    delete_pcb(pcb_interrupt->pid);

    // Agrego el PCB pero con su contexto nuevo
    add_pcb(pcb_interrupt);

    //dictionary_put(table_pcb, string_itoa(pcb_interrupt->pid), pcb_interrupt);
    log_info(logger_kernel, "Se actualizo el PCB de PID: <%d> en la table_pcb", pcb_interrupt->pid);

    // Actualizo el estado del pcb en la cola correspondiente:
    agregar_a_ready_fin_quantum(pcb_interrupt);

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
        // logica de mandar pcb a exit
        agregar_a_cola_exit(pcb);
    }

    if(recurso->instancias > 0){
        recurso->instancias--;
        // mandar el pcb devuelta para que se siga ejecutando en CPU
        log_info(logger_kernel, "el recurso tenia instancias disponibles y se pudo decrementar por un WAIT");
        send_pcb_cpu(pcb);
    }else{
        // mandar el pcb a la cola de bloqueados y agregar PCB a la cola de bloqueados del recurso en cuestion para cuando se libere.
        // TODO:
        //agregar_a_cola_bloqueados(pcb);
    }

    pthread_mutex_unlock(&MUTEX_RECURSOS);
}

// manejar instruccion WAIT
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
        // logica de mandar pcb a exit
        agregar_a_cola_exit(pcb);
    }

    if(recurso->instancias > 0){
        recurso->instancias++;
        // mandar el pcb devuelta para que se siga ejecutando en CPU
        log_info(logger_kernel, "el recurso tenia instancias disponibles y se pudo decrementar por un WAIT");
        send_pcb_cpu(pcb);
    }else{
        // mandar el pcb a la cola de bloqueados y agregar PCB a la cola de bloqueados del recurso en cuestion para cuando se libere.
        // TODO:
        //agregar_a_cola_bloqueados(pcb);
    }

    pthread_mutex_unlock(&MUTEX_RECURSOS);
}

// ############################################################################################################
// nueva forma planteada por el tipo: 
// ############################################################################################################

// Función para recibir y manejar el mensaje de WAIT o SIGNAL
void manejar_wait_signal(int cod_op) {
    t_PCB* pcb;
    char* resource_name;

    // Recibir el paquete y deserializar el PCB y el nombre del recurso
    recv_pcb_and_string(fd_cpu_dispatch, &pcb, &resource_name);

    bool resultado;
    if (cod_op == MSG_CPU_KERNEL_WAIT) {
        resultado = wait(resource_name, pcb);
    } else if (cod_op == MSG_CPU_KERNEL_SIGNAL) {
        signal(resource_name, pcb);
        resultado = true;
    }

    if (resultado) {
        // Si el resultado es true, reanudar el proceso
        log_info(logger_kernel, "Proceso %d reanudado después de WAIT/SIGNAL en el recurso %s", pcb->pid, resource_name);
        // Actualizar el PCB en la tabla
        dictionary_remove_and_destroy(table_pcb, string_itoa(pcb->pid), (void *)pcb_destroy);
        dictionary_put(table_pcb, string_itoa(pcb->pid), pcb);

        // Añadir el PCB a la cola READY
        pthread_mutex_lock(&MUTEX_READY);
        queue_push(COLA_READY, pcb);
        pthread_mutex_unlock(&MUTEX_READY);

        sem_post(&SEM_READY); // Despertar al planificador
    } else {
        // Si el resultado es false, bloquear el proceso
        log_info(logger_kernel, "Proceso %d bloqueado esperando el recurso %s", pcb->pid, resource_name);
        // Actualizar el PCB en la tabla
        dictionary_remove_and_destroy(table_pcb, string_itoa(pcb->pid), (void *)pcb_destroy);
        dictionary_put(table_pcb, string_itoa(pcb->pid), pcb);

        // Añadir el PCB a la cola de bloqueados del recurso
        pthread_mutex_lock(&MUTEX_BLOQUEADOS);
        // Aquí deberías tener una estructura para manejar los bloqueados por recurso
        queue_push(get_recurso(resource_name)->cola_bloqueados, pcb);
        pthread_mutex_unlock(&MUTEX_BLOQUEADOS);
    }

    free(resource_name); // Liberar la memoria del nombre del recurso
}
bool wait(char *resource_name, t_PCB *pcb) {
    t_recurso *recurso = get_recurso(resource_name);
    if (recurso == NULL) {
        log_error(logger_kernel, "Recurso %s no existe", resource_name);
        return false; // El recurso no existe
    }

    if (recurso->instancias > 0) {
        recurso->instancias--;
        return true; // El recurso está disponible
    } else {
        // El recurso no está disponible, el proceso debe ser bloqueado
        // Aquí puedes añadir el PCB a la cola de bloqueados del recurso
        return false;
    }
}

void signal(char *resource_name, t_PCB *pcb) {
    t_recurso *recurso = get_recurso(resource_name);
    if (recurso == NULL) {
        log_error(logger_kernel, "Recurso %s no existe", resource_name);
        // Aquí puedes manejar el caso en que el recurso no exista
        return;
    }

    recurso->instancias++;
    // Aquí puedes manejar el desbloqueo de procesos bloqueados por el recurso
}
