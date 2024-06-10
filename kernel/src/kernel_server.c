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

void atender_kernel_IO()
{
    bool control_key = 1;
    while (control_key)
    {
        int cod_op = recibir_operacion(fd_kernel_IO);

        switch (cod_op)
        {
        case EXAMPLE:
            // Se procesa el request
            recv_example_msg_entradasalida();
            control_key = false; // Cortamos la espera de solicitudes
            break;
        // TODO:
        /*
            Agregar operaciones a las que dara servicio el modulo
        */
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

    //fd_kernel_memoria > 0 ? send_example_memoria() : log_error(logger_kernel, "Error al intentar enviar mensaje a %s", SERVER_MEMORIA);

    log_info(logger_kernel, "Esperando a que se conecte %s...", CLIENTE_ENTRADASALIDA);
    fd_kernel_IO = esperar_cliente(logger_kernel, CLIENTE_ENTRADASALIDA, fd_server);
}

void crear_hilos_conexiones() 
{
    pthread_t hilo_cpu_dispatch;
    pthread_t hilo_entradasalida;
    pthread_t hilo_memoria;

    // Hilo para manejar mensajes de CPU Dispatch
    if (pthread_create(&hilo_cpu_dispatch, NULL, (void *)atender_kernel_cpu_dispatch, NULL) != 0)
    {
        log_error(logger_kernel, "Error al crear el hilo para atender la CPU dispatch. ABORTANDO");
        exit(EXIT_FAILURE);
    }

    // Hilo para manejar mensajes de IO
    if (pthread_create(&hilo_entradasalida, NULL, (void *)atender_kernel_IO, NULL) != 0)
    {
        log_error(logger_kernel, "Error al crear el hilo para atender las IOs. ABORTANDO");
        exit(EXIT_FAILURE);
    }

    // Hilo para manejar mensajes de Memoria
    if (pthread_create(&hilo_memoria, NULL, (void *)atender_kernel_memoria, NULL) != 0)
    {
        log_error(logger_kernel, "Error al crear el hilo para atender la MEMORIA. ABORTANDO");
        exit(EXIT_FAILURE);
    }

    pthread_detach(hilo_cpu_dispatch);
    pthread_detach(hilo_entradasalida);
    pthread_detach(hilo_memoria);
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
    liberar_conexion(fd_kernel_IO);
}