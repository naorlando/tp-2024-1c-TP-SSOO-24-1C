#include "entradasalida_server.h"

void requests_kernel()
{
    bool esperar = true;

    while (esperar)
    {
        int cod_operacion = recibir_operacion(fd_kernel);

        switch (cod_operacion)
        {
            case EXAMPLE:
                // Se procesa el request
                recv_example_msg_kernel();
                esperar = false; // Cortamos la espera de solicitudes
                break;

            case MSG_KERNEL_IO:

                log_info(logger_entradasalida, "Se recibio un mje del KERNEL");
                break;
            // Agrego la operacion para el caso de que el kernel solicite un sleep
            case MSG_IO_GEN_SLEEP:
                log_info(logger_entradasalida, "Se recibio una instruccion MSG_IO_GEN_SLEEP");
                atender_instruccion_sleep();
                break;
            case -1:
                log_error(logger_entradasalida, "ERROR: Ha surgido un problema inesperado, se desconecto el modulo de entradaSalida.");
                esperar = false; // Cortamos la espera de solicitudes
                break;
            default:
                log_warning(logger_entradasalida, "WARNING: El modulo de entradaSalida ha recibido una solicitud con una operacion desconocida");
                break;
        }
    }
}

void requests_memoria()
{
    bool control_key = 1;
    while (control_key)
    {
        int cod_op = recibir_operacion(fd_memoria);

        switch (cod_op)
        {
            case -1:
                log_error(logger_entradasalida, "la memoria se desconecto. Terminando servidor");
                control_key = 0;
                break;
            default:
                log_warning(logger_entradasalida, "Operacion desconocida en dispatch. No quieras meter la pata");
                break;
        }
    }
}

void inicializar_sockets()
{
    // Conexion con Kernel
    kernel_port = string_itoa(obtener_puerto_kernel(entradasalida_config));
    fd_kernel = crear_conexion(logger_entradasalida, SERVER_KERNEL, obtener_ip_kernel(entradasalida_config), kernel_port);
    //fd_kernel > 0 ? log_info(logger_entradasalida, "Conexión exitosa con kernel") : log_error(logger_entradasalida, "Error al conectar con kernel");

    if (fd_kernel == -1)
    {
        log_error(logger_entradasalida, "Error al conectar con el kernel. ABORTANDO");
        exit(EXIT_FAILURE);
    }

    // Conexion con Memoria
    memoria_port = string_itoa(obtener_puerto_memoria(entradasalida_config));
    fd_memoria = crear_conexion(logger_entradasalida, SERVER_MEMORIA, obtener_ip_memoria(entradasalida_config), memoria_port);
    //fd_memoria > 0 ? send_example_memoria() : log_error(logger_entradasalida, "Error al intentar enviar mensaje a %s", SERVER_MEMORIA);

    if (fd_memoria == -1)
    {
        log_error(logger_entradasalida, "Error al conectar con la memoria. ABORTANDO");
        exit(EXIT_FAILURE);
    }
}

void crear_hilos_conexiones() 
{
    pthread_t hilo_kernel;
    pthread_t hilo_memoria;

    // Hilo para manejar mensajes de Kernel
    if (pthread_create(&hilo_kernel, NULL, (void *)requests_kernel, NULL) != 0) {
        log_error(logger_entradasalida, "Error al crear el hilo para atender el KERNEL. ABORTANDO");
        exit(EXIT_FAILURE);
    }

    // Hilo para manejar mensajes de Memoria
    if (pthread_create(&hilo_memoria, NULL, (void *)requests_memoria, NULL) != 0)
    {
        log_error(logger_entradasalida, "Error al crear el hilo para atender la MEMORIA. ABORTANDO");
        exit(EXIT_FAILURE);
    }

    pthread_detach(hilo_kernel);
    pthread_join(hilo_memoria,NULL);
}

void cerrar_cliente()
{
    _cerrar_puertos();
    _cerrar_conexiones();
    log_info(logger_entradasalida, "SERVER ENTRADASALIDA cerrado correctamente.");
}

void _cerrar_puertos()
{
    free(kernel_port);
    free(memoria_port);
}

void _cerrar_conexiones()
{
    liberar_conexion(fd_kernel);
    liberar_conexion(fd_memoria);
}