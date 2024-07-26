#include "protocolo_cpu.h"

int send_example_memoria()
{
    char *cadena = "CPU ENVIO MENSAJE A MEMORIA";
    uint8_t entero = 10;

    return send_example(cadena, entero, fd_memoria);
}

// CPU -> MEMORIA :: MSG_CPU_MEMORIA_INIT
int send_msg_cpu_memoria_init(int fd)
{

    t_package *package = package_create(MSG_CPU_MEMORIA_INIT, sizeof(uint32_t));
    serialize_uint32_t(package->buffer, 1, 0);
    package_send(package, fd);

    package_destroy(package);

    return EXIT_SUCCESS;
}

int recv_msg_memoria_cpu_init(uint32_t *page_size)
{
    t_buffer *buffer = recive_full_buffer(fd_memoria);

    deserialize_uint32_t(buffer, 1, page_size);

    buffer_destroy(buffer);

    return EXIT_SUCCESS;
}

int recv_example_msg_kernel()
{
    log_info(logger_cpu, "<<<<< EXAMPLE RECIVE MESSAGE FROM KERNEL >>>>");
    t_message_example *new_msg = recv_example(fd_kernel_dispatch);

    log_info(logger_cpu, "MENSAJE => %s", get_cadena(new_msg));
    log_info(logger_cpu, "ENTERO => %d", get_entero(new_msg));

    message_example_destroy(new_msg);

    return 0;
}

t_PCB *recv_pcb_kernel()
{
    t_PCB *pcb = recv_pcb(fd_kernel_dispatch);

    log_info(logger_cpu, "Se recibio un PCB del Kernel, PID => %d", pcb->pid);
    // log_info(logger_cpu, "PCB pc => %d", pcb->program_counter);
    // log_info(logger_cpu, "PCB Quantum => %d", pcb->quantum);
    // log_info(logger_cpu, "PCB cpu_registers AX => %d", pcb->cpu_registers->ax);

    // pcb_destroy(pcb);

    return pcb;
}

t_instruction *recv_instruction_memoria()
{
    t_instruction *instruction = recv_instruction(fd_memoria);

    log_info(logger_cpu, "Se recibio una instruccion de memoria");

    return instruction;
}

void send_get_next_instruction_memoria(uint32_t pid, uint32_t program_counter)
{
    send_get_next_instruction(fd_memoria, pid, program_counter);
}

void send_pcb_kernel(int header)
{
    send_pcb(header, fd_kernel_dispatch, pcb_execute);
}

void send_solicitud_io_generica_kernel(t_PCB *pcb, t_instruction *instruccion)
{
    t_list *parametros = obtener_parametros(instruccion);

    char *nombre = (char *)list_get(parametros, 0);
    uint32_t tiempo_sleep = atoi((char *)list_get(instruccion->params, 1));

    t_io_generica *generica = crear_io_generica(nombre, tiempo_sleep, pcb->pid);

    send_solicitud_io_generica(fd_kernel_dispatch, pcb, nombre, generica);
}

void send_solicitud_io_stdin_kernel(t_PCB *pcb, t_instruction *instruccion)
{
    t_list *parametros = obtener_parametros(instruccion);
    char *interface = (char *)list_get(parametros, 0);
    char *reg_direccion = (char *)list_get(parametros, 1);
    char *reg_tamano = (char *)list_get(parametros, 2);

    uint32_t direccion_logica = _obtener_valor_registro(cpu_registers, reg_direccion);
    uint32_t tamano = _obtener_valor_registro(cpu_registers, reg_tamano);

    t_io_frames* io_frames_stdin = exec_io_frames(pcb->pid,direccion_logica,tamano);

    t_io_stdin* io_stdin = crear_io_stdin(io_frames_stdin);
   
    send_solicitud_io_stdin(fd_kernel_dispatch, pcb, interface, io_stdin);
}

void send_pcb_kernel_interruption(t_name_interruption tipo_de_interrupcion)
{
    switch (tipo_de_interrupcion)
    {
    case QUANTUM_INTERRUPT:
        send_pcb(MSG_PCB_KERNEL_INTERRUPTION_QUANTUM, fd_kernel_dispatch, pcb_execute);
        break;

    case EXIT_INTERRUPT:
        send_pcb(MSG_PCB_KERNEL_INTERRUPTION_FINISH_PROCESS, fd_kernel_dispatch, pcb_execute);
        break;
    // TODO : mandarle el RECURSO.
    //  camino de la interrupcion a la negativa, ya sea  que no haya recursos disponibles, o el signal termino bien.
    //  case RECURSO_INTERRUPT:
    //          send_pcb(MSG_PCB_KERNEL_INTERRUPTION_RECURSO, fd_kernel_dispatch, pcb_execute);
    //      break;

    // case MSG_IO:
    //         send_pcb(MSG_PCB_KERNEL_INTERRUPTION_IO, fd_kernel_dispatch, pcb_execute);
    //     break;
    default:
        break;
    }
}


// CPU -> MEMORIA :: MSG_CPU_MEMORIA_PAGE
int send_msg_cpu_memoria_page(uint32_t pid, uint32_t page, int fd)
{

    t_package *package = package_create(MSG_CPU_MEMORIA_PAGE, sizeof(uint32_t) * 2);

    serialize_uint32_t(package->buffer, 2, pid, page);

    package_send(package, fd);

    package_destroy(package);

    return EXIT_SUCCESS;
}

// MEMORIA -> CPU :: MSG_MEMORIA_CPU_FRAME
int recv_msg_memoria_cpu_frame(t_buffer *buffer, uint32_t *frame)
{
    deserialize_uint32_t(buffer, 1, frame);

    return EXIT_SUCCESS;
}

// --   RESIZE PROCESS   --
int send_msg_cpu_memoria_resize(uint32_t pid, uint32_t new_size, int fd)
{

    t_package *package = package_create(MSG_CPU_MEMORIA_RESIZE, sizeof(uint32_t) * 2);

    serialize_uint32_t(package->buffer, 2, pid, new_size);

    package_send(package, fd);

    package_destroy(package);

    return EXIT_SUCCESS;
}

void send_solicitud_io_stdout_kernel(t_PCB *pcb, t_instruction *instruccion)
{
    t_list *parametros = obtener_parametros(instruccion);
    char *interface = (char *)list_get(parametros, 0);
    char *reg_direccion = (char *)list_get(parametros, 1);
    char *reg_tamano = (char *)list_get(parametros, 2);

    uint32_t direccion_logica = _obtener_valor_registro(cpu_registers, reg_direccion);
    uint32_t tamano = _obtener_valor_registro(cpu_registers, reg_tamano);

    t_io_frames* io_frames_stdout = exec_io_frames(pcb->pid,direccion_logica,tamano);

    t_io_stdout* io_stdout = crear_io_stdout(io_frames_stdout);
   
    send_solicitud_io_stdout(fd_kernel_dispatch, pcb, interface, io_stdout);
}
