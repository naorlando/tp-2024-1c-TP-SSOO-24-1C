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

void atender_kernel_cpu_interrupt()
{
    bool control_key = 1;
    while (control_key)
    {
        int cod_op = recibir_operacion(fd_cpu_interrupt);

        switch (cod_op)
        {

        case MSG_PCB_KERNEL_INTERRUPTION_QUANTUM:
            //TODO: agregar PCB donde este:
            // 1-recibir pcb:
            t_PCB* pcb_interrupt = recv_pcb_interrupt();
            log_info(logger_kernel, "Se recibio un pcb de CPU-INTERRUPT");

            // 2-actualizar el pcb en la tabla de pcb:
            // actualizar el pcb que ingresa en la tabla de pcbs macheando por pid:
            // hacemos un dictionary_remove_and_destroy() para liberar la memoria del pcb a actualizar...
            dictionary_remove_and_destroy(table_pcb, string_itoa(pcb_interrupt->pid), pcb_destroy);
            dictionary_put(table_pcb, string_itoa(pcb_interrupt->pid), pcb_interrupt);
            log_info(logger_kernel, "Se actualizo un pcb-INTERRUPT en la table_pcb");

            

            // 3-actualizar el estado del pcb en la cola correspondiente:
            queue_push(COLA_READY, pcb_interrupt);
            log_info(logger_kernel, "Se actualizo el estado del pcb-INTERRUPT en la cola correspondiente");

            break;

        case -1:
            log_error(logger_kernel, "CPU INTERRUPT se desconecto. Terminando servidor");
            control_key = 0;
            break;
        default:
            log_warning(logger_kernel, "Operacion desconocida en INTERRUPT. No quieras meter la pata");
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
    }
}

void inicializar_sockets()
{
    // Conexion con cpu_dispatch
    cpu_dispatch_port = string_itoa(kernel_config->PUERTO_CPU_DISPATCH);
    fd_cpu_dispatch = crear_conexion(logger_kernel, SERVER_CPU, kernel_config->IP_CPU, cpu_dispatch_port);

    // Conexion con memoria
    memoria_port = string_itoa(kernel_config->PUERTO_MEMORIA);
    fd_kernel_memoria = crear_conexion(logger_kernel, SERVER_MEMORIA, kernel_config->IP_MEMORIA, memoria_port);

    //fd_kernel_memoria > 0 ? send_example_memoria() : log_error(logger_kernel, "Error al intentar enviar mensaje a %s", SERVER_MEMORIA);

    log_info(logger_kernel, "Esperando a que se conecte %s...", CLIENTE_ENTRADASALIDA);
    fd_kernel_IO = esperar_cliente(logger_kernel, CLIENTE_ENTRADASALIDA, fd_server);
}

void crear_hilos_conexiones() 
{
    // Hilo para manejar mensajes de CPU Dispatch
    pthread_t hilo_cpu_dispatch;
    pthread_create(&hilo_cpu_dispatch, NULL, (void *)atender_kernel_cpu_dispatch, NULL);
    pthread_detach(hilo_cpu_dispatch);

    //hilo para manejar mensajes de CPU Interrupt
    pthread_t hilo_cpu_interrupt;
    pthread_create(&hilo_cpu_interrupt, NULL, (void *)atender_kernel_cpu_interrupt, NULL);
    pthread_detach(hilo_cpu_interrupt);

    // Hilo para manejar mensajes de IO
    pthread_t hilo_entradasalida;
    pthread_create(&hilo_entradasalida, NULL, (void *)atender_kernel_IO, NULL);
    pthread_detach(hilo_entradasalida);

    // Hilo para manejar mensajes de Memoria
    pthread_t hilo_memoria;
    pthread_create(&hilo_memoria, NULL, (void *)atender_kernel_memoria, NULL);
    pthread_detach(hilo_memoria);
}

void cerrar_servidor()
{
    free(server_port);
    free(cpu_dispatch_port);
    free(memoria_port);
    liberar_conexion(fd_server);
    liberar_conexion(fd_kernel_memoria);
    liberar_conexion(fd_cpu_dispatch);
    liberar_conexion(fd_kernel_IO);
}