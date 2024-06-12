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
    server_port_dispatch = string_itoa(obtener_puerto_escucha_dispatch(cpu_config));
    fd_server_dispatch = iniciar_servidor(logger_cpu, NULL, NULL, server_port_dispatch);

    if (fd_server_dispatch != -1)
    {
        log_info(logger_cpu, "%s server listo escuchando en puerto %s", SERVERNAME, server_port_dispatch);
    }
    else
    {
        log_error(logger_cpu, "Error al iniciar %s server en puerto %s", SERVERNAME, server_port_dispatch);
        exit(EXIT_FAILURE);
    }

    server_port_interrupt = string_itoa(obtener_puerto_escucha_interrupt(cpu_config));
    fd_server_interrupt = iniciar_servidor(logger_cpu, NULL, NULL, server_port_interrupt);

    if (fd_server_interrupt != -1)
    {
        log_info(logger_cpu, "%s server listo escuchando en puerto %s", SERVERNAME, server_port_interrupt);
    }
    else
    {
        log_error(logger_cpu, "Error al iniciar %s server en puerto %s", SERVERNAME, server_port_interrupt);
        exit(EXIT_FAILURE);
    }
}

void inicializar_sockets()
{
    // Conexion con memoria
    memoria_port = string_itoa(obtener_puerto_memoria(cpu_config));
    fd_memoria = crear_conexion(logger_cpu, SERVER_MEMORIA, obtener_ip_memoria(cpu_config), memoria_port);

    if (fd_memoria == -1)
    {
        log_error(logger_cpu, "Error al conectar con la MEMORIA. ABORTANDO");
        exit(EXIT_FAILURE);
    }
}

void esperar_clientes()
{
    // Esperamos al Kernel-Dispatch
    log_info(logger_cpu, "Esperando a que se conecte %s - Dispatch",CLIENTE_KERNEL);
    fd_kernel_dispatch = esperar_cliente(logger_cpu, CLIENTE_KERNEL, fd_server_dispatch);

    // Esperamos al Kernel-Interrupt
    log_info(logger_cpu, "Esperando a que se conecte %s - Interrupt",CLIENTE_KERNEL);
    fd_kernel_interrupt = esperar_cliente(logger_cpu, CLIENTE_KERNEL, fd_server_interrupt);
}

void cerrar_servidor()
{
    _cerrar_puertos();
    _cerrar_conexiones();
    log_info(logger_kernel, "SERVER CPU cerrado correctamente.");
}

void _cerrar_puertos()
{
    free(server_port_dispatch);
    free(server_port_interrupt);
    free(memoria_port);
}

void _cerrar_conexiones()
{
    liberar_conexion(fd_server_dispatch);
    liberar_conexion(fd_server_interrupt);
    liberar_conexion(fd_kernel_dispatch);
    liberar_conexion(fd_kernel_interrupt);
    liberar_conexion(fd_memoria);
}