#include <protocolo_kernel.h>

int send_example_cpu()
{
    char* cadena = "KERNEL ENVIO MENSAJE A CPU";
    uint8_t entero = 5;

    return send_example(cadena, entero, fd_cpu_dispatch);
}

void send_pcb_cpu(t_PCB* pcb)
{
    send_pcb(MSG_PCB_CPU, fd_cpu_dispatch, pcb);
}

t_PCB* recv_pcb_cpu() 
{
    t_PCB* pcb = recv_pcb(fd_cpu_dispatch);
    log_info(logger_kernel, "Se recibio un PCB del CPU_DISPATCH, PID <%d>", pcb->pid);

    return pcb;
}

int send_example_memoria()
{
    char *cadena = "KERNEL ENVIO MENSAJE A MEMORIA";
    uint8_t entero = 5;

    return send_example(cadena, entero, fd_kernel_memoria);
}

int recv_example_msg_entradasalida(int cliente_io)
{
    log_info(logger_kernel, "<<<<< EXAMPLE RECIVE MESSAGE FROM ENTRADASALIDA >>>>");

    t_message_example* new_msg = recv_example(cliente_io);

    log_info(logger_kernel, "MENSAJE => %s", get_cadena(new_msg));
    log_info(logger_kernel, "ENTERO => %d", get_entero(new_msg));
    
    message_example_destroy(new_msg);

    return 0;
}

t_PCB* recv_pcb_interrupt()
{
    t_buffer* buffer = recive_full_buffer(fd_cpu_dispatch);
    t_PCB* pcb = deserialize_pcb(buffer);
    //pcb_destroy(pcb);
    buffer_destroy(buffer);
    return pcb;
}

// adapter:
void send_interruption_cpu(t_interruption* interrupcion)
{
    // Seteo que se envio una interrupcion a CPU
    interrupcion_enviada = true;
    send_interruption(interrupcion, fd_cpu_interrupt);
}

// Agrego la función que envía la instrucción IO_GEN_SLEEP al módulo de E/S
int enviar_io_gen_sleep(int fd, int pid, int unidades_trabajo) {
    t_package *paquete = package_create(MSG_IO_KERNEL_GEN_SLEEP, sizeof(t_buffer));
    
    // Crear la instrucción
    t_instruction *instruccion = malloc(sizeof(t_instruction));
    instruccion->name = IO_GEN_SLEEP;
    instruccion->params = list_create();

    // Agregar los parámetros a la lista
    int *param_pid = malloc(sizeof(int));
    *param_pid = pid;
    list_add(instruccion->params, param_pid);
    
    int *param_unidades_trabajo = malloc(sizeof(int));
    *param_unidades_trabajo = unidades_trabajo;
    list_add(instruccion->params, param_unidades_trabajo);
    
    // Serializar la instrucción
    t_buffer *buffer = paquete->buffer;
    buffer->size = sizeof(t_name_instruction) + sizeof(uint32_t) * 2 + sizeof(int) * 2;
    buffer->stream = malloc(buffer->size);
    void *stream = buffer->stream;
    
    memcpy(stream, &(instruccion->name), sizeof(t_name_instruction));
    stream += sizeof(t_name_instruction);
    
    uint32_t cant_params = list_size(instruccion->params);
    memcpy(stream, &cant_params, sizeof(uint32_t));
    stream += sizeof(uint32_t);
    
    for (int i = 0; i < cant_params; i++) {
        int *param = list_get(instruccion->params, i);
        memcpy(stream, param, sizeof(int));
        stream += sizeof(int);
    }
    
    if (package_send(paquete, fd) != EXIT_SUCCESS) {
        log_error(logger_kernel, "Error al enviar la instrucción IO_GEN_SLEEP");
        list_destroy_and_destroy_elements(instruccion->params, free);
        free(instruccion);
        package_destroy(paquete);
        return -1;
    }
    
    log_info(logger_kernel, "Instrucción IO_GEN_SLEEP enviada con pid %d y %d unidades de trabajo", pid, unidades_trabajo);
    
    list_destroy_and_destroy_elements(instruccion->params, free);
    free(instruccion);
    package_destroy(paquete);
    
    return 0;
}

// Agrego la función que recibe la confirmación de que la instrucción IO_GEN_SLEEP fue recibida y procesada
int recibir_confirmacion_io(int fd_kernel) {
    t_package *package = package_create(NULL_HEADER, 0);

    if (package_recv(package, fd_kernel) != EXIT_SUCCESS) {
        log_error(logger_kernel, "Error al recibir confirmación IO_GEN_SLEEP");
        package_destroy(package);
        return -1;
    }

    t_buffer *buffer = package->buffer;
    void *stream = buffer->stream;
    uint32_t mensaje_id;
    memcpy(&mensaje_id, stream, sizeof(uint32_t));

    if (mensaje_id == MSG_KERNEL_IO) {
        log_info(logger_kernel, "Confirmación de IO_GEN_SLEEP recibida");
    } else {
        log_warning(logger_kernel, "Se recibió un mensaje desconocido en confirmación IO_GEN_SLEEP");
    }

    package_destroy(package);
    return 0;
}

t_solicitud_io_generica* recv_solicitud_io_generica_cpu()
{
    t_solicitud_io_generica* io_gen = recv_solicitud_io_generica(fd_cpu_dispatch);

    return io_gen;
}

t_manejo_recurso*  recv_wait_or_signal_request()
{
    t_buffer* buffer = recive_full_buffer(fd_cpu_dispatch);
    t_manejo_recurso* manejo_recurso_recibido = deserialize_manejo_recurso(buffer);
    log_info(logger_kernel, "Se recibio una solicitud de WAIT o SIGNAL del CPU_DISPATCH, PID <%d> y  recurso: %s", manejo_recurso_recibido->pcb->pid, manejo_recurso_recibido->nombre_recurso);
    buffer_destroy(buffer);
    return manejo_recurso_recibido;
}