#include "memoria_server.h"

void requests_cpu()
{
    bool esperar = true;

    while (esperar)
    {
        int cod_operacion = recibir_operacion(fd_cpu);

        switch (cod_operacion)
        {
        case EXAMPLE:
            // Se procesa el request
            recv_example_msg_cpu();
            // esperar = false; //Cortamos la espera de solicitudes
            break;
        case MSG_NEXT_INSTRUCTION_CPU:
            retornar_siguiente_instruccion();
            break;

        case MSG_CPU_MEMORIA_INIT:
            process_message_cpu_handshake();
            break;

        case MSG_CPU_MEMORIA_PAGE:
            process_message_cpu_page();
            break;

        case MSG_GENERIC_MEMORIA_DATA_READ:
            process_message_data_read(fd_cpu);
            break;

        case MSG_GENERIC_MEMORIA_DATA_WRITE:
            process_message_data_write();
            break;

        case MSG_CPU_MEMORIA_RESIZE:
            process_message_cpu_resize(fd_cpu);
            break;
        case -1:
            log_error(logger_memoria, "ERROR: Ha surgido un problema inesperado, se desconecto el modulo de memoria.");
            esperar = false; // Cortamos la espera de solicitudes
            break;
        default:
            log_warning(logger_memoria, "WARNING: El modulo de memoria ha recibido una solicitud con una operacion desconocida");
            break;
        }
    }
}

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
            // esperar = false; //Cortamos la espera de solicitudes
            break;

        // TODO:
        /*
            Agregar operaciones a las que dara servicio el modulo
        */
        case MSG_KERNEL_MEMORIA:

        case MSG_KERNEL_CREATE_PROCESS:
            crear_imagen_proceso();
            break;
        case -1:
            log_error(logger_memoria, "ERROR: Ha surgido un problema inesperado, se desconecto el modulo de memoria.");
            esperar = false; // Cortamos la espera de solicitudes
            break;
        default:
            log_warning(logger_memoria, "WARNING: El modulo de memoria ha recibido una solicitud con una operacion desconocida");
            break;
        }
    }
}

void levantar_servidor()
{
    server_port = string_itoa(obtener_puerto_escucha(memoria_config));
    fd_server = iniciar_servidor(logger_memoria, SERVERNAME, NULL, server_port);

    if (fd_server != -1)
    {
        log_info(logger_memoria, "%s server listo escuchando en puerto %s", SERVERNAME, server_port);
    }
    else
    {
        log_error(logger_memoria, "Error al iniciar %s server en puerto %s", SERVERNAME, server_port);
    }
}

void esperar_clientes()
{
    // Espera conexion de la CPU
    log_info(logger_memoria, "Esperando a que se conecte CPU");
    fd_cpu = esperar_cliente(logger_memoria, CLIENTE_CPU, fd_server);

    // Espera conexion del Kernel
    log_info(logger_memoria, "Esperando a que se conecte KERNEL");
    fd_kernel = esperar_cliente(logger_memoria, CLIENTE_KERNEL, fd_server);
}

void crear_hilos_conexiones()
{
    pthread_t hilo_kernel;
    pthread_t hilo_cpu;
    pthread_t hilo_aceptar_io;

    // Hilo para manejar mensajes del Kernel
    if (pthread_create(&hilo_kernel, NULL, (void *)requests_kernel, NULL) != 0)
    {
        log_error(logger_memoria, "Error al crear el hilo para atender al KERNEL. ABORTANDO");
        exit(EXIT_FAILURE);
    }

    // Hilo para manejar mensajes de CPU
    if (pthread_create(&hilo_cpu, NULL, (void *)requests_cpu, NULL) != 0)
    {
        log_error(logger_memoria, "Error al crear el hilo para atender al CPU. ABORTANDO");
        exit(EXIT_FAILURE);
    }

    // Creo hilo para aceptar conexiones de IO de forma dinámica
    if (pthread_create(&hilo_aceptar_io, NULL, esperar_conexiones_IO, &fd_server) != 0)
    {
        log_error(logger_memoria, "Error al crear el hilo para aceptar conexiones de IO. ABORTANDO");
        exit(EXIT_FAILURE);
    }

    pthread_detach(hilo_kernel);
    pthread_detach(hilo_cpu);
    pthread_join(hilo_aceptar_io, NULL);
}

void *esperar_conexiones_IO(void *arg)
{
    int server_fd = *(int *)arg;
    bool control_key = 1;

    while (control_key)
    {
        int cliente_io = esperar_cliente(logger_memoria, CLIENTE_ENTRADASALIDA, server_fd);

        if (cliente_io != -1)
        {
            t_IO_connection* io_connection = recibir_io_connection(cliente_io, logger_memoria, MSG_IO_MEMORIA);

            if (io_connection != NULL) {
                agregar_IO_connection(io_connection, ios_conectadas, &MUTEX_DICTIONARY_IOS);

                pthread_t hilo_entradasalida;
                if (pthread_create(&hilo_entradasalida, NULL, (void *)atender_memoria_IO, io_connection) != 0)
                {
                    log_error(logger_memoria, "Error al crear el hilo para atender el cliente de IO. ABORTANDO");
                    exit(EXIT_FAILURE);
                }
                char* nombre_interfaz = obtener_nombre_conexion(io_connection);
                log_info(logger_memoria, "Nueva conexión de I/O establecida: %s de tipo %s", nombre_interfaz, tipo_interfaz_to_string(obtener_tipo_conexion(io_connection)));
                pthread_detach(hilo_entradasalida);
            }
        }
        else
        {
            log_error(logger_memoria, "Error al esperar cliente de IO");
        }
    }
    return NULL;
}

void atender_memoria_IO(void* io_connection)
{
    t_IO_connection* cliente_io = (t_IO_connection*)io_connection;
    bool control_key = 1;

    while (control_key)
    {
        int cod_op = recibir_operacion(obtener_file_descriptor(cliente_io));

        switch (cod_op)
        {
            case EXAMPLE:
                recv_example_msg_entradasalida();
                // esperar = false; //Cortamos la espera de solicitudes
                break;
            case -1:
                log_error(logger_memoria, "ERROR: Ha surgido un problema inesperado, se desconecto el modulo de memoria.");
                control_key = false; // Cortamos la espera de solicitudes
                break;
            default:
                log_warning(logger_memoria, "WARNING: El modulo de memoria ha recibido una solicitud con una operacion desconocida");
                break;
        }
    }
}

void cerrar_servidor()
{
    _cerrar_puertos();
    _cerrar_conexiones();
    log_info(logger_memoria, "SERVER MEMORIA cerrado correctamente.");
}

void _cerrar_puertos()
{
    free(server_port);
}

void _cerrar_conexiones()
{
    liberar_conexion(fd_server);
    liberar_conexion(fd_cpu);
    liberar_conexion(fd_kernel);
    // TODO: Implementar funcion para liberar las conexiones de todas las IOs
    //  que se conectaron a la memoria
}