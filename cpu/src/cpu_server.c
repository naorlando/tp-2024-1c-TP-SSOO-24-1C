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

// TODO: 
    // headear de fin de quantum (case QUANTUM) --> con la accion de cambiar interrupcion_pendiente = TRUE. mandandole el PID para que el cpu
//  maneje(cargue) el pcb y se lo envie a kernel con su contexto actualizado.

void atender_cpu_kernel_interrupt()
{   
    bool control_key = 1;
    while (control_key)
    {
        int cod_op = recibir_operacion(fd_kernel_interrupt);

        switch (cod_op)
        {
            case MSG_QUANTUM:
                recibir_interrupcion();
                log_info(logger_cpu, "Se recibio un mensaje de interrupcion por QUANTUM del kernel");
                break;
            
            case MSG_KERNEL_CPU_EXIT:
                recibir_interrupcion();
                log_info(logger_cpu, "Se recibio un mensaje de interrupcion por EXIT del kernel");
                break;

            case -1:
                log_error(logger_cpu, "El KERNEL se desconecto. Terminando servidor");
                control_key = 0;
                break;
            default:
                log_warning(logger_cpu, "Operacion desconocida. No quieras meter la pata");
                break;
        }
    }
}

void levantar_servidor()
{
    server_port_dispatch = string_itoa(obtener_puerto_escucha_dispatch(cpu_config));
    fd_server_dispatch = iniciar_servidor(logger_cpu, SERVERNAME, NULL, server_port_dispatch);

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

    establecer_handshake_memoria();
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

void crear_hilos_conexiones() 
{
    pthread_t hilo_kernel_dispatch;
    pthread_t hilo_kernel_interrupt;
    pthread_t hilo_memoria;
    
    // Hilo para manejar mensajes del Kernel-Dispatch
    if (pthread_create(&hilo_kernel_dispatch, NULL, (void *)atender_cpu_kernel_dispatch, NULL) != 0)
    {
        log_error(logger_cpu, "Error al crear el hilo para atender al KERNEL-DISPATCH. ABORTANDO");
        exit(EXIT_FAILURE);
    }

    // Hilo para manejar mensajes del Kernel-Interrupt
    if (pthread_create(&hilo_kernel_interrupt, NULL, (void *)atender_cpu_kernel_interrupt, NULL) != 0)
    {
        log_error(logger_cpu, "Error al crear el hilo para atender al KERNEL-INTERRUPT. ABORTANDO");
        exit(EXIT_FAILURE);
    }

    // Hilo para manejar mensajes de Memoria
    if (pthread_create(&hilo_memoria, NULL, (void *)atender_cpu_memoria, NULL) != 0) 
    {
        log_error(logger_cpu, "Error al crear el hilo para atender a la MEMORIA. ABORTANDO");
        exit(EXIT_FAILURE);
    }

    pthread_detach(hilo_kernel_dispatch);
    pthread_detach(hilo_kernel_interrupt);
    pthread_join(hilo_memoria, NULL);
}

void cerrar_servidor()
{
    _cerrar_puertos();
    _cerrar_conexiones();
    log_info(logger_cpu, "SERVER CPU cerrado correctamente.");
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

void establecer_handshake_memoria()
{
    // Hacemos un handshake con memoria
    send_msg_cpu_memoria_init(fd_memoria);

    int cod_op = recibir_operacion(fd_memoria);

    if(cod_op != MSG_MEMORIA_CPU_INIT)
    {
        log_error(logger_cpu,"SE DEBE RECIBIR EL HANDSHAKE DESDE MEMORIA");
        exit(EXIT_FAILURE);
    }else
    {
        recv_msg_memoria_cpu_init(&page_size);
        log_debug(logger_cpu_debug, "HANDSHAKE CON MEMORIA RECIBIDO -- ENTRADAS_TABLA: %d -- PAGE_SIZE: %d", cpu_config->CANTIDAD_ENTRADAS_TLB, page_size);
    }
}