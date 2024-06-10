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