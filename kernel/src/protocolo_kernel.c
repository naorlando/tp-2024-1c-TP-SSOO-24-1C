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
  
    log_info(logger_kernel, "Se recibio un PCB a traves del CPU_DISPATCH, PID <%d>", pcb->pid);

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
    int bytes_enviados = send_io_generica(fd, io_generica);

    if(bytes_enviados > 0) {
         log_info(logger_kernel, "Solicitud enviada a la IO GENERICA %s con pid %d", nombre_interfaz, obtener_pid_generica(io_generica));
    }else {
        log_error(logger_kernel, "Hubo un error al enviar la solicitud a la IO GENERICA %s con pid %d", nombre_interfaz, obtener_pid_generica(io_generica));
    }
    
    return bytes_enviados;
}

// Agrego la función que envía la instrucción IO_STDIN al módulo de E/S
int send_kernel_io_stdin(int fd, char* nombre_interfaz, t_io_stdin* io_stdin) {
    
    //TODO: Modificar tipo de retorno para validar si la interfaz esta conectada
    int bytes_enviados = send_io_stdin(fd, io_stdin);

    if(bytes_enviados > 0) {
        log_info(logger_kernel, "Solicitud enviada a la IO STDIN %s con pid %d", nombre_interfaz, obtener_pid_stdin(io_stdin));
    }else {
        log_error(logger_kernel, "Hubo un error al enviar la solicitud a la IO STDIN %s con pid %d", nombre_interfaz, obtener_pid_stdin(io_stdin));
    }
    
    return bytes_enviados;
}

// Agrego la función que envía la instrucción IO_STDOUT al módulo de E/S
int send_kernel_io_stdout(int fd, char* nombre_interfaz, t_io_stdout* io_stdout) {
    
    //TODO: Modificar tipo de retorno para validar si la interfaz esta conectada
    int bytes_enviados = send_io_stdout(fd, io_stdout);
    
    if(bytes_enviados > 0) {
        log_info(logger_kernel, "Solicitud enviada a la IO STDOUT %s con pid %d", nombre_interfaz, obtener_pid_stdout(io_stdout));
    }else {
        log_error(logger_kernel, "Hubo un error al enviar la solicitud a la IO STDOUT %s con pid %d", nombre_interfaz, obtener_pid_stdout(io_stdout));
    }
    
    return bytes_enviados;
}

// Agrego la función que envía la instrucción IO_DIALFS al módulo de E/S
int send_kernel_io_dialfs(int fd, char* nombre_interfaz, t_io_dialfs* io_dialfs) {
    //TODO: Modificar tipo de retorno para validar si la interfaz esta conectada
    int bytes_enviados = send_io_dialfs(fd, io_dialfs);
    
    if(bytes_enviados > 0) {
        log_info(logger_kernel, "Solicitud enviada a la IO DIALFS %s para la operación %s con PID %d", 
                 nombre_interfaz, get_operation_name(io_dialfs->operacion), io_dialfs->pid);
    } else {
        log_error(logger_kernel, "Hubo un error al enviar la solicitud a la IO DIALFS %s para la operación %s con PID %d", 
                  nombre_interfaz, get_operation_name(io_dialfs->operacion), io_dialfs->pid);
    }
    
    return bytes_enviados;
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

t_solicitud_io_stdin* recv_solicitud_io_stdin_cpu()
{
    t_solicitud_io_stdin* io_stdin = recv_solicitud_io_stdin(fd_cpu_dispatch);

    return io_stdin;
}

void send_msg_kernel_memoria_end_process(uint32_t pid)
{
    t_package *package = package_create(MSG_KERNEL_END_PROCESS, sizeof(uint32_t));

    buffer_add_uint32(package->buffer, pid);

    package_send(package, fd_kernel_memoria);

    package_destroy(package);
}

t_solicitud_io_stdout* recv_solicitud_io_stdout_cpu()
{
    t_solicitud_io_stdout* io_stdout = recv_solicitud_io_stdout(fd_cpu_dispatch);

    return io_stdout;
}

t_solicitud_io_dialfs* recv_solicitud_io_dialfs_cpu()
{
    t_solicitud_io_dialfs* solicitud_dialfs = recv_solicitud_io_dialfs(fd_cpu_dispatch);

    return solicitud_dialfs;
}