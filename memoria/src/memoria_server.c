#include "memoria_server.h"

void requests_cpu() {
    bool esperar = true;

    while(esperar) {
        int cod_operacion = recibir_operacion(fd_cpu);

        switch(cod_operacion){
            case EXAMPLE:
                // Se procesa el request
                recv_example_msg_cpu();
                //esperar = false; //Cortamos la espera de solicitudes
            break;
            case MSG_NEXT_INSTRUCTION_CPU:
                t_next_instruction* next = recv_next_instruction();

                if(next == NULL) {
                    log_error(logger_memoria, "ERROR: Ha surgido un problema al recibir la solicitud de siguiente instruccion.");
                }

                t_proceso* proceso = obtener_proceso(obtener_pid_process(next));

                if(proceso == NULL) {
                    log_error(logger_memoria, "ERROR: Ha surgido un problema al buscar el proceso en la memoria.");
                }else{
                    t_instruction* instruction = obtener_siguiente_instruccion(proceso, obtener_pc_process(next));

                    if(instruction != NULL){
                        send_instrution(instruction);
                    }
                }
            break;                     
            case MSG_CPU_MEMORIA:

                log_info(logger_memoria, "Se recibio un mje del cpu");
            break;
            case -1:
                log_error(logger_memoria, "ERROR: Ha surgido un problema inesperado, se desconecto el modulo de memoria.");
                esperar = false; //Cortamos la espera de solicitudes
            break;
            default:
                log_warning(logger_memoria, "WARNING: El modulo de memoria ha recibido una solicitud con una operacion desconocida");
            break;
        }
    }
}

void requests_kernel() {
    bool esperar = true;

    while(esperar) {
        int cod_operacion = recibir_operacion(fd_kernel);

        switch(cod_operacion){
            case EXAMPLE:
                // Se procesa el request
                recv_example_msg_kernel();
                //esperar = false; //Cortamos la espera de solicitudes
            break;

            //TODO:
            /*
                Agregar operaciones a las que dara servicio el modulo
            */             
            case MSG_KERNEL_MEMORIA:

            case MSG_KERNEL_CREATE_PROCESS:

                t_new_process* new_process= recv_process_kernel();

                if(new_process == NULL) {
                    log_error(logger_memoria, "ERROR: Ha surgido un problema al recibir el nuevo proceso.");
                }
                
                if(add_process_to_memory(new_process)) {
                    log_info(logger_memoria, "Se ha creado correctamente en memoria el proceso con pid %d", new_process->pid);
                }else {
                    log_warning(logger_memoria, "WARNING CREATE PROCESS: No se ha podido crear el proceso en memoria");
                }

                //TODO: Hacer funcion para eliminar un t_new_process
                destroy_new_process(new_process);
            break;
            case -1:
                log_error(logger_memoria, "ERROR: Ha surgido un problema inesperado, se desconecto el modulo de memoria.");
                esperar = false; //Cortamos la espera de solicitudes
            break;
            default:
                log_warning(logger_memoria, "WARNING: El modulo de memoria ha recibido una solicitud con una operacion desconocida");
            break;
        }
    }
}

void requests_entradasalida() {
    bool esperar = true;

    while(esperar) {
        int cod_operacion = recibir_operacion(fd_entradasalida);

        switch(cod_operacion){
            case EXAMPLE:
                // Se procesa el request
                recv_example_msg_entradasalida();
                //esperar = false; //Cortamos la espera de solicitudes
            break;
            //TODO:
            /*
                Agregar operaciones a las que dara servicio el modulo
            */
            case MSG_IO_MEMORIA:

                log_info(logger_memoria, "Se recibio un mje del IO");
            break;
 
            case -1:
                log_error(logger_memoria, "ERROR: Ha surgido un problema inesperado, se desconecto el modulo de memoria.");
                esperar = false; //Cortamos la espera de solicitudes
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
    log_info(logger_memoria, "esperando a que se conecte CPU");
    fd_cpu = esperar_cliente(logger_memoria, CLIENTE_CPU, fd_server);

    // Espera conexion del Kernel
    log_info(logger_memoria, "esperando a que se conecte KERNEL");
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
    if (pthread_create(&hilo_aceptar_io, NULL, esperar_conexiones_IO, &fd_server) != 0) {
        log_error(logger_memoria, "Error al crear el hilo para aceptar conexiones de IO. ABORTANDO");
        exit(EXIT_FAILURE);
    }

    pthread_detach(hilo_kernel);
    pthread_detach(hilo_cpu);
    pthread_join(hilo_aceptar_io,NULL);
}

void* esperar_conexiones_IO(void* arg) 
{
    int server_fd = *(int*)arg;
    bool control_key = 1;

    while (control_key) {
        int cliente_io = esperar_cliente(logger_memoria, CLIENTE_ENTRADASALIDA, server_fd);
        
        if (cliente_io != -1) {
            //add_io_client(cliente_io, "IOClient");
            
            pthread_t hilo_entradasalida;
            if (pthread_create(&hilo_entradasalida, NULL, (void*)requests_entradasalida, &cliente_io) != 0) {
                log_error(logger_memoria, "Error al crear el hilo para atender el cliente de IO. ABORTANDO");
                exit(EXIT_FAILURE);
            }
            pthread_detach(hilo_entradasalida);
        } else {
            log_error(logger_memoria, "Error al esperar cliente de IO");
        }
    }
    return NULL;
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
    //TODO: Implementar funcion para liberar las conexiones de todas las IOs
    // que se conectaron a la memoria
}