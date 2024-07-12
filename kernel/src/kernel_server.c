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

void atender_kernel_IO(void* io_connection)
{
    t_IO_connection* cliente_io = (t_IO_connection*)io_connection;
    bool control_key = 1;

    while (control_key)
    {
        sem_wait(obtener_semaforo_cola_bloqueados(cliente_io));

        procesar_solicitud_func procesar_func = obtener_procesador_solicitud(obtener_tipo_conexion(cliente_io));
        void* solicitud = obtener_siguiente_proceso(cliente_io);

        if (procesar_func != NULL) {
            procesar_solicitud_IO(obtener_file_descriptor(cliente_io), solicitud, procesar_func);
        } else {
            log_warning(logger_kernel, "Tipo de IO desconocida. No quieras meter la pata");
        }

        int cod_op = recibir_operacion(obtener_file_descriptor(cliente_io));
        char* tipo_interfaz = tipo_interfaz_to_string(obtener_tipo_conexion(cliente_io));

        switch (cod_op)
        {
            case EXAMPLE:
                // Se procesa el request
                recv_example_msg_entradasalida(obtener_file_descriptor(cliente_io));
                break;
            case MSG_IO_KERNEL_GENERICA:
            case MSG_IO_KERNEL_STDIN:
            case MSG_IO_KERNEL_STDOUT:
                log_info(logger_kernel, "Se recibio un mensaje de IO %s", tipo_interfaz);
                procesar_respuesta_io(obtener_file_descriptor(cliente_io), obtener_nombre_conexion(cliente_io));
                break;
            case MSG_IO_KERNEL_DIALFS: 

                log_info(logger_kernel, "Se recibio un mensaje de IO DIALFS");
                break;
            case -1:
                log_error(logger_kernel, "la IO se desconecto. Terminando servidor");
                control_key = 0;
                break;
            default:
                log_warning(logger_kernel, "Operacion desconocida en IO. No quieras meter la pata");
                break;
        }

        // Obtener el PCB de la solicitud y moverlo a ready
        t_PCB* pcb = obtener_pcb_de_solicitud(solicitud, tipo_interfaz);
        if (pcb != NULL) {
            agregar_de_blocked_a_ready(pcb);
            destruir_solicitud_io(solicitud, tipo_interfaz);
        } else {
            log_warning(logger_kernel, "No se pudo obtener el PCB de la solicitud");
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
            case MSG_CPU_IO_GEN_SLEEP: //CPU -> KERNEL (Se solicita interactuar con IO GENENRICA)
                // t_solicitud_io_generica* io_gen = recv_solicitud_io_generica_cpu();

                // t_PCB* pcb_io_gen = obtener_pcb_solicitud_generica(io_gen);

                // log_info(logger_kernel, "Se recibio una solicitud de CPU a una IO GENERICA para el PCB de PID <%d>", pcb_io_gen->pid);
                
                // cancelar_hilo_quantum(pcb_io_gen->pid);
                // sem_post(&SEM_CPU);
    
                //1. Validar que la IO GENERICA este conectada
                //2. 
                //  a) Si esta conectada, encolar el PCB a la cola de bloqueo correspondiente (Sigue en 3 el flujo)
                //  b) Si no esta conectada, pasar a EXIT el PCB. (Termina el flujo aca)
                //3. Enviar el struct t_io_generica a la IO GENERICA si esta libre
                //4. Bloquear el envio de otra solicitud de IO GENERICA, hasta que la IO responda luego de su procesamiento

                procesar_ios_genericas();

                break;
            case MSG_PCB_IO_KERNEL: // TODO: NO se esta usando
                //Recibimos el t_interface

                // SI recibo el PCB antes de que se mande la interrupcion tengo que matar el hilo de interrupcion
                // t_PCB* pcb_io = recv_pcb_cpu(); 
                // cancelar_hilo_quantum(pcb_io->pid);

                // log_info(logger_kernel, "El PCB de PID <%d> solicita una IO ",pcb_io->pid);
                
                break;
            case MSG_PCB_KERNEL_EXIT: // CPU -> KERNEL (El PCB llego a la instruccion EXIT) 
                //TODO: ARMAR UNA FUNCION QUE SE ENCARGUE DE LA GESTION DE LIBERAR EL PCB QUE LLEGO A EXIT
                // YA QUE PUEDE TENER RECURSOS ASIGNADOS Y MEMORIA

                // t_PCB* pcb_exit= recv_pcb_cpu();

                // // limpio la variable global
                // pthread_mutex_lock(&MUTEX_EXECUTE);
                // EXECUTE = NULL;
                // pthread_mutex_unlock(&MUTEX_EXECUTE);

                // cancelar_hilo_quantum(pcb_exit->pid);

                // log_info(logger_kernel, "Llego a EXIT el PCB de PID <%d>", pcb_exit->pid);

                // // Elimino el PCB
                // pcb_destroy(pcb_exit);
                
                // sem_post(&SEM_CPU);
                // log_info(logger_kernel, "La cola de Ready tiene %d elementos", queue_size(COLA_READY));

                procesar_pcb_exit();

                break;
            case MSG_PCB_KERNEL_INTERRUPTION_QUANTUM:
                //TODO: agregar PCB donde este:
                // 1-recibir pcb:
                // t_PCB* pcb_interrupt = recv_pcb_interrupt();

                // pthread_mutex_lock(&MUTEX_EXECUTE);
                // EXECUTE = NULL; // SACO EL PCB DE EXECUTE
                // pthread_mutex_unlock(&MUTEX_EXECUTE);

                // log_info(logger_kernel, "Se recibio un PCB por interrupcion a traves del CPU_DISPATCH, PID: <%d>", pcb_interrupt->pid);

                // // 2-actualizar el pcb en la tabla de pcb:
                // // actualizar el pcb que ingresa en la tabla de pcbs macheando por pid:
                // // hacemos un dictionary_remove_and_destroy() para liberar la memoria del pcb a actualizar...
                // dictionary_remove_and_destroy(table_pcb, string_itoa(pcb_interrupt->pid), (void *)pcb_destroy);
                // dictionary_put(table_pcb, string_itoa(pcb_interrupt->pid), pcb_interrupt);
                // log_info(logger_kernel, "Se actualizo el PCB de PID: <%d> en la table_pcb", pcb_interrupt->pid);

                

                // // 3-actualizar el estado del pcb en la cola correspondiente:
                // pthread_mutex_lock(&MUTEX_READY);
                // queue_push(COLA_READY, pcb_interrupt);
                // pthread_mutex_unlock(&MUTEX_READY);
                // log_info(logger_kernel, "Se actualizo el estado del PCB de PID: <%d> en la cola READY", pcb_interrupt->pid);
                // log_info(logger_kernel, "La cola de Ready tiene %d elementos", queue_size(COLA_READY));
                // sem_post(&SEM_READY);
                // sem_post(&SEM_CPU);
                
                //sem_post(&SEM_CPU);

                procesar_interrupcion();

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
    // Conexion con cpu-dispatch
    cpu_dispatch_port = string_itoa(obtener_puerto_cpu_dispatch(kernel_config));
    fd_cpu_dispatch = crear_conexion(logger_kernel, SERVER_CPU, obtener_ip_cpu(kernel_config), cpu_dispatch_port);

    if (fd_cpu_dispatch == -1)
    {
        log_error(logger_kernel, "Error al conectar con la CPU-DISPATCH. ABORTANDO");
        exit(EXIT_FAILURE);
    }

    // Conexion con cpu-interrupt
    cpu_interrupt_port = string_itoa(obtener_puerto_cpu_interrupt(kernel_config));
    fd_cpu_interrupt = crear_conexion(logger_kernel, SERVER_CPU, obtener_ip_cpu(kernel_config), cpu_interrupt_port);

    if (fd_cpu_interrupt == -1)
    {
        log_error(logger_kernel, "Error al conectar con la CPU-INTERRUPT. ABORTANDO");
        exit(EXIT_FAILURE);
    }

    // Conexion con memoria
    memoria_port = string_itoa(obtener_puerto_memoria(kernel_config));
    fd_kernel_memoria = crear_conexion(logger_kernel, SERVER_MEMORIA, obtener_ip_memoria(kernel_config), memoria_port);

    if (fd_kernel_memoria == -1)
    {
        log_error(logger_kernel, "Error al conectar con la MEMORIA. ABORTANDO");
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
            char* nombre_interfaz = recibir_io_connection(cliente_io);
            t_IO_connection* io_connection = get_IO_connection(nombre_interfaz);

            pthread_t hilo_io;
            if (pthread_create(&hilo_io, NULL, (void*)atender_kernel_IO, io_connection) != 0) {
                log_error(logger_kernel, "Error al crear el hilo para atender el cliente de IO. ABORTANDO");
                exit(EXIT_FAILURE);
            }
            log_info(logger_kernel, "Interfaz %s conectada con éxito", nombre_interfaz);
            pthread_detach(hilo_io);
        } else {
            log_error(logger_kernel, "Error al esperar cliente de IO");
        }
    }
    return NULL;
}

char* recibir_io_connection(int cliente_io) 
{
    int cod_op = recibir_operacion(cliente_io);

    if(cod_op == MSG_IO_KERNEL) {
        return nuevo_IO_cliente_conectado(cliente_io);
    } else {
        log_info(logger_kernel, "Error al recibir un cliente IO. ABORTANDO");
        exit(EXIT_FAILURE);
    }
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
