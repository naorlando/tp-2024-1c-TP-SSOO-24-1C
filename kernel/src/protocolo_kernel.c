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
int send_kernel_io_gen_sleep(int fd, char* nombre_interfaz, t_io_generica* io_generica) {
    
    //TODO: Modificar tipo de retorno para validar si la interfaz esta conectada
    send_io_generica(fd, io_generica);
    
    log_info(logger_kernel, "Solicitud enviada a la IO GENERICA %s con pid %d", nombre_interfaz, obtener_pid_generica(io_generica));
    
    return 0;
}

// Agrego la función que envía la instrucción IO_STDIN al módulo de E/S
int send_kernel_io_stdin(int fd, char* nombre_interfaz, t_io_stdin* io_stdin) {
    
    //TODO: Modificar tipo de retorno para validar si la interfaz esta conectada
    send_io_stdin(fd, io_stdin);
    
    log_info(logger_kernel, "Solicitud enviada a la IO STDIN %s con pid %d", nombre_interfaz, obtener_pid_stdout(io_stdin));
    
    return 0;
}

// Agrego la función que envía la instrucción IO_STDOUT al módulo de E/S
int send_kernel_io_stdout(int fd, char* nombre_interfaz, t_io_stdout* io_stdout) {
    
    //TODO: Modificar tipo de retorno para validar si la interfaz esta conectada
    send_io_stdout(fd, io_stdout);
    
    log_info(logger_kernel, "Solicitud enviada a la IO STDOUT %s con pid %d", nombre_interfaz, obtener_pid_stdout(io_stdout));
    
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