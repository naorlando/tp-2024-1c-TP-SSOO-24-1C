#include "kernel_server.h"

void atender_kernel_memoria()
{
    bool control_key = 1;
    while (control_key)
    {
        int cod_op = recibir_operacion(fd_kernel_memoria);

        switch (cod_op)
        {
            case MSG_MEMORIA_KERNEL:

                log_info(logger_kernel, "Se recibio un mje del memoria");
                break;

            case -1:
                log_error(logger_kernel, "la memoria se desconecto. Terminando servidor");
                control_key = 0;
                break;
            default:
                log_warning(logger_kernel, "Operacion desconocida en memoria. No quieras meter la pata");
                break;
        }
    }
}

void atender_kernel_IO(void* cliente_socket)
{
    int cliente_io = *(int*)cliente_socket;
    bool control_key = 1;

    while (control_key)
    {
        int cod_op = recibir_operacion(cliente_io);

        switch (cod_op)
        {
            case EXAMPLE:
                // Se procesa el request
                recv_example_msg_entradasalida(cliente_io);
                control_key = false; // Cortamos la espera de solicitudes
                break;
            case MSG_IO_KERNEL:

                log_info(logger_kernel, "Se recibio un mje de IO");
                break;

            case -1:
                log_error(logger_kernel, "la IO se desconecto. Terminando servidor");
                control_key = 0;
                break;
            default:
                log_warning(logger_kernel, "Operacion desconocida en IO. No quieras meter la pata");
                break;
        }
    }
}

void atender_kernel_cpu_dispatch()
{
    bool control_key = 1;
    while (control_key)
    {
        int cod_op = recibir_operacion(fd_cpu_dispatch);

        switch (cod_op)
        {
            case MSG_CPU_DISPATCH_KERNEL:

                log_info(logger_kernel, "Se recibio un mje de CPU DISPATCH");
                break;
            case MSG_PCB_IO_KERNEL:
                //Recibimos el t_interface
                //recibir_pcb_io(); // TODO: hacer la funcion para recibir la interface
                break;

            case -1:
                log_error(logger_kernel, "CPU DISPATCH se desconecto. Terminando servidor");
                control_key = 0;
                break;
            default:
                log_warning(logger_kernel, "Operacion desconocida en dispatch. No quieras meter la pata");
                break;
        }
    }
}

void levantar_servidor()
{
    server_port = string_itoa(kernel_config->PUERTO_ESCUCHA);
    fd_server = iniciar_servidor(logger_kernel, NULL, NULL, server_port);

    if (fd_server != -1)
    {
        log_info(logger_kernel, "%s server listo escuchando en puerto %s", SERVERNAME, server_port);
    }
    else
    {
        log_error(logger_kernel, "Error al iniciar %s server en puerto %s", SERVERNAME, server_port);
        exit(EXIT_FAILURE);
    }
}

void inicializar_sockets()
{
    // Conexion con cpu_dispatch
    cpu_dispatch_port = string_itoa(kernel_config->PUERTO_CPU_DISPATCH);
    fd_cpu_dispatch = crear_conexion(logger_kernel, SERVER_CPU, kernel_config->IP_CPU, cpu_dispatch_port);

    if (fd_cpu_dispatch == -1)
    {
        log_error(logger_kernel, "Error al conectar con la memoria. ABORTANDO");
        exit(EXIT_FAILURE);
    }

    // Conexion con memoria
    memoria_port = string_itoa(kernel_config->PUERTO_MEMORIA);
    fd_kernel_memoria = crear_conexion(logger_kernel, SERVER_MEMORIA, kernel_config->IP_MEMORIA, memoria_port);

    if (fd_kernel_memoria == -1)
    {
        log_error(logger_kernel, "Error al conectar con la CPU. ABORTANDO");
        exit(EXIT_FAILURE);
    }
}

void crear_hilos_conexiones() 
{
    pthread_t hilo_cpu_dispatch;
    pthread_t hilo_memoria;
    pthread_t hilo_aceptar_io;

    // Hilo para manejar mensajes de CPU Dispatch
    if (pthread_create(&hilo_cpu_dispatch, NULL, (void *)atender_kernel_cpu_dispatch, NULL) != 0)
    {
        log_error(logger_kernel, "Error al crear el hilo para atender la CPU dispatch. ABORTANDO");
        exit(EXIT_FAILURE);
    }

    // Hilo para manejar mensajes de Memoria
    if (pthread_create(&hilo_memoria, NULL, (void *)atender_kernel_memoria, NULL) != 0)
    {
        log_error(logger_kernel, "Error al crear el hilo para atender la MEMORIA. ABORTANDO");
        exit(EXIT_FAILURE);
    }

    // Creo hilo para aceptar conexiones de IO de forma dinámica
    if (pthread_create(&hilo_aceptar_io, NULL, esperar_conexiones_IO, &fd_server) != 0) {
        log_error(logger_kernel, "Error al crear el hilo para aceptar conexiones de IO. ABORTANDO");
        exit(EXIT_FAILURE);
    }

    pthread_detach(hilo_cpu_dispatch);
    pthread_detach(hilo_memoria);
    pthread_detach(hilo_aceptar_io);
}

void* esperar_conexiones_IO(void* arg) 
{
    int server_fd = *(int*)arg;
    bool control_key = 1;

    while (control_key) {
        int cliente_io = esperar_cliente(logger_kernel, CLIENTE_ENTRADASALIDA, server_fd);
        
        if (cliente_io != -1) {
            //add_io_client(cliente_io, "IOClient");
            
            pthread_t hilo_io;
            if (pthread_create(&hilo_io, NULL, (void*)atender_kernel_IO, &cliente_io) != 0) {
                log_error(logger_kernel, "Error al crear el hilo para atender el cliente de IO. ABORTANDO");
                exit(EXIT_FAILURE);
            }
            pthread_detach(hilo_io);
        } else {
            log_error(logger_kernel, "Error al esperar cliente de IO");
        }
    }
    return NULL;
}

void cerrar_servidor()
{
    _cerrar_puertos();
    _cerrar_conexiones();
    log_info(logger_kernel, "SERVER KERNEL cerrado correctamente.");
}

void _cerrar_puertos()
{
    free(server_port);
    free(cpu_dispatch_port);
    free(memoria_port);
}

void _cerrar_conexiones()
{
    liberar_conexion(fd_server);
    liberar_conexion(fd_kernel_memoria);
    liberar_conexion(fd_cpu_dispatch);
    //TODO: Implementar funcion para liberar las conexiones de todas las IOs
    // que se conectaron al kernel
}