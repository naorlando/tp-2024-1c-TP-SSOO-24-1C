#include "cpu_server.h"

void atender_cpu_kernel_dispatch()
{
    bool control_key = 1;
    while (control_key)
    {
        int cod_op = recibir_operacion(fd_kernel_dispatch);

        switch (cod_op)
        {

        case MSG_KERNEL_CPU_DISPATCH:
            // atender_cpu_kernel_dispatch();
            break;
        
        case MSG_PCB_CPU:
            recibir_pcb();
            break;

        case EXAMPLE:
            recv_example_msg_kernel();
            break;

        case -1:
            log_error(logger_cpu, "el KERNEL se desconecto. Terminando servidor");
            control_key = 0;
            break;
        default:
            log_warning(logger_cpu, "Operacion desconocida en dispatch. No quieras meter la pata");
            break;
        }
    }
}

void atender_cpu_memoria()
{
    bool control_key = 1;
    while (control_key)
    {
        int cod_op = recibir_operacion(fd_memoria);

        switch (cod_op)
        {
        case MSG_INSTRUCTION_MEMORIA:
            manejar_ciclo_de_instruccion();
            log_info(logger_cpu, "Se recibio una instruccion de memoria");
            break;
        
        case MSG_MEMORIA_CPU:
            log_info(logger_cpu, "Se recibio un mje del memoria");
            break;

        case -1:
            log_error(logger_cpu, "la memoria se desconecto. Terminando servidor");
            control_key = 0;
            break;
        default:
            log_warning(logger_cpu, "Operacion desconocida en dispatch. No quieras meter la pata");
            break;
        }
    }
}

void levantar_servidor()
{
    server_port = string_itoa(obtener_puerto_escucha_dispatch(cpu_config));
    fd_server = iiniciar_servidor(logger_cpu, NULL, NULL, server_port);

    if (fd_server != -1)
    {
        log_info(logger_cpu, "%s server listo escuchando en puerto %s", SERVERNAME, server_port);
    }
    else
    {
        log_error(logger_cpu, "Error al iniciar %s server en puerto %s", SERVERNAME, server_port);
        exit(EXIT_FAILURE);
    }
}