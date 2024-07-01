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

void manejar_instruccion_wait(){
    //recibo el paquete con el recurso a manejar
    pthread_mutex_lock(&MUTEX_RECURSOS);
    char * nombre_recurso = receive_message_with_string(fd_cpu_dispatch);
    t_recurso *recurso = dictionary_get(recursos_dictionary, nombre_recurso);
    char * respuesta;
    log_info(logger_kernel, "Se recibio una instruccion WAIT para el recurso: %s", nombre_recurso);
    //verificar si el recurso esta disponible
    if(recurso->instancias > 0){
        recurso->instancias--;
        respuesta = "OK";
        log_info(logger_kernel, "Se decremento una instancia del recurso: %s", nombre_recurso);
    }else{
        //si no esta disponible el recurso, se manda mensaje a cpu de esto, cpu copia el contexto, y manda el pcb al kernel para que este proceso pase a bloqueado.
    //CHAN, que pija hacemos, ?mandamos todo el pcb en cada parte, o solo mensajes?
    
}
}

void manejar_instruccion_signal(){
    //recibo el paquete con el recurso a manejar
}