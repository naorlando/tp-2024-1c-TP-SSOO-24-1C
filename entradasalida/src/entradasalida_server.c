#include "entradasalida_server.h"


//=========================================================
// FUNCIONES PARA INICIALIZAR EL CLIENTE DE ENTRADA/SALIDA
//=========================================================

void inicializar_sockets() {
    // Inicializar socket de conexión con Kernel
    if (tiene_configuracion_kernel(entradasalida_config)) {
        kernel_port = string_itoa(obtener_puerto_kernel(entradasalida_config));
        fd_kernel = crear_conexion(logger_entradasalida, SERVER_KERNEL, obtener_ip_kernel(entradasalida_config), kernel_port);
        if (fd_kernel == -1) {
            log_error(logger_entradasalida, "Error al conectar con el Kernel. ABORTANDO");
            exit(EXIT_FAILURE);
        }
        
        enviar_io_interface();
        
    } else {
        log_error(logger_entradasalida, "No se encontró configuración para el Kernel. ABORTANDO");
        exit(EXIT_FAILURE);
    }

    // Consultar el tipo de interfaz
    char* tipo_interfaz = obtener_tipo_interfaz(entradasalida_config);

    // Inicializar socket de conexión con Memoria si no es una interfaz genérica
    if (strcmp(tipo_interfaz, "GENERICA") != 0) {
        if (tiene_configuracion_memoria(entradasalida_config)) {
            memoria_port = string_itoa(obtener_puerto_memoria(entradasalida_config));
            fd_memoria = crear_conexion(logger_entradasalida, SERVER_MEMORIA, obtener_ip_memoria(entradasalida_config), memoria_port);
            if (fd_memoria == -1) {
                log_error(logger_entradasalida, "Error al conectar con la Memoria. ABORTANDO");
                exit(EXIT_FAILURE);
            }
        } else {
            log_error(logger_entradasalida, "No se encontró configuración para la Memoria. ABORTANDO");
            exit(EXIT_FAILURE);
        }
    }
}

//==========================================================
// FUNCIONES PARA CREAR HILOS DE INTERFAZ DE ENTRADA/SALIDA
//==========================================================


// void crear_hilo_interfaz_generica() {
//     pthread_t hilo_generica;
//     if (pthread_create(&hilo_generica, NULL, atender_solicitudes_generica, NULL) != 0) {
//         log_error(logger_entradasalida, "Error al crear el hilo para atender la interfaz genérica. ABORTANDO");
//         exit(EXIT_FAILURE);
//     }
//     pthread_detach(hilo_generica);
// }

// void crear_hilo_interfaz_stdin() {
//     pthread_t hilo_stdin;
//     if (pthread_create(&hilo_stdin, NULL, atender_solicitudes_stdin, NULL) != 0) {
//         log_error(logger_entradasalida, "Error al crear el hilo para atender la interfaz STDIN. ABORTANDO");
//         exit(EXIT_FAILURE);
//     }
//     pthread_detach(hilo_stdin);
// }

// void crear_hilo_interfaz_stdout() {
//     pthread_t hilo_stdout;
//     if (pthread_create(&hilo_stdout, NULL, atender_solicitudes_stdout, NULL) != 0) {
//         log_error(logger_entradasalida, "Error al crear el hilo para atender la interfaz STDOUT. ABORTANDO");
//         exit(EXIT_FAILURE);
//     }
//     pthread_detach(hilo_stdout);
// }

// void crear_hilo_interfaz_dialfs() {
//     pthread_t hilo_dialfs;
//     if (pthread_create(&hilo_dialfs, NULL, atender_solicitudes_dialfs, NULL) != 0) {
//         log_error(logger_entradasalida, "Error al crear el hilo para atender la interfaz DialFS. ABORTANDO");
//         exit(EXIT_FAILURE);
//     }
//     pthread_detach(hilo_dialfs);
// }

void crear_hilos_conexiones() 
{
    pthread_t hilo_kernel;
    pthread_t hilo_memoria;

    // Hilo para manejar mensajes del Kernel
    if (pthread_create(&hilo_kernel, NULL, (void *)atender_solicitudes_io_kernel, NULL) != 0)
    {
        log_error(logger_entradasalida, "Error al crear el hilo para atender al KERNEL. ABORTANDO");
        exit(EXIT_FAILURE);
    }

    // Inicializar hilo para atender mensajes de Memoria si no es una interfaz genérica
    if (strcmp(obtener_tipo_interfaz(entradasalida_config), "GENERICA") != 0) {
        
        // Hilo para manejar mensajes de Memoria
        if (pthread_create(&hilo_memoria, NULL, (void *)atender_solicitudes_io_memoria, NULL) != 0) 
        {
            log_error(logger_entradasalida, "Error al crear el hilo para atender a la MEMORIA. ABORTANDO");
            exit(EXIT_FAILURE);
        }

        pthread_join(hilo_memoria, NULL);
    }

    pthread_join(hilo_kernel, NULL);
}

//====================================================
// FUNCIONES PARA ATENDER SOLICITUDES DE ENTRADA/SALIDA
//====================================================

void atender_solicitudes_io_memoria() {
    bool esperar = true;
    while (esperar) {
        int cod_operacion = recibir_operacion(fd_kernel);
        switch (cod_operacion) {
            // case MSG_IO_GENERICA:
            //     atender_instruccion_generica(fd_kernel);
            //     break;
            case -1:
                log_error(logger_entradasalida, "ERROR: Ha surgido un problema inesperado, se desconectó el Kernel.");
                esperar = false;
                break;
            default:
                log_warning(logger_entradasalida, "WARNING: El módulo de entrada/salida ha recibido una solicitud con una operación desconocida");
                break;
        }
    }
}

void atender_solicitudes_io_kernel() {
    bool esperar = true;
    while (esperar) {
        int cod_operacion = recibir_operacion(fd_kernel);
        switch (cod_operacion) {
            case MSG_KERNEL_IO_GENERICA:
                atender_instruccion_generica(fd_kernel);
                break;
            case MSG_KERNEL_IO_STDIN:
                atender_instruccion_stdin(fd_kernel);
                break;
            case MSG_KERNEL_IO_STDOUT:
                atender_instruccion_stdout(fd_kernel);
                break;
            case MSG_KERNEL_IO_DIALFS:
                atender_instruccion_dialfs(fd_kernel);
                break;
            case -1:
                log_error(logger_entradasalida, "ERROR: Ha surgido un problema inesperado, se desconectó el Kernel.");
                esperar = false;
                break;
            default:
                log_warning(logger_entradasalida, "WARNING: El módulo de entrada/salida ha recibido una solicitud con una operación desconocida");
                break;
        }
    }
}

// void* atender_solicitudes_generica(void* args) {
//     bool esperar = true;
//     while (esperar) {
//         int cod_operacion = recibir_operacion(fd_kernel);
//         switch (cod_operacion) {
//             case IO_GENERICA:
//                 atender_instruccion_generica(fd_kernel);
//                 break;
//             case -1:
//                 log_error(logger_entradasalida, "ERROR: Ha surgido un problema inesperado, se desconectó el Kernel.");
//                 esperar = false;
//                 break;
//             default:
//                 log_warning(logger_entradasalida, "WARNING: El módulo de entrada/salida ha recibido una solicitud con una operación desconocida");
//                 break;
//         }
//     }
//     return NULL;
// }

// void* atender_solicitudes_stdin(void* args) {
//     bool esperar = true;
//     while (esperar) {
//         int cod_operacion = recibir_operacion(fd_kernel);
//         switch (cod_operacion) {
//             case IO_STDIN:
//                 atender_instruccion_stdin(fd_kernel);
//                 break;
//             case -1:
//                 log_error(logger_entradasalida, "ERROR: Ha surgido un problema inesperado, se desconectó el Kernel.");
//                 esperar = false;
//                 break;
//             default:
//                 log_warning(logger_entradasalida, "WARNING: El módulo de entrada/salida ha recibido una solicitud con una operación desconocida");
//                 break;
//         }
//     }
//     return NULL;
// }

// void* atender_solicitudes_stdout(void* args) {
//     bool esperar = true;
//     while (esperar) {
//         int cod_operacion = recibir_operacion(fd_kernel);
//         switch (cod_operacion) {
//             case IO_STDOUT:
//                 atender_instruccion_stdout(fd_kernel);
//                 break;
//             case -1:
//                 log_error(logger_entradasalida, "ERROR: Ha surgido un problema inesperado, se desconectó el Kernel.");
//                 esperar = false;
//                 break;
//             default:
//                 log_warning(logger_entradasalida, "WARNING: El módulo de entrada/salida ha recibido una solicitud con una operación desconocida");
//                 break;
//         }
//     }
//     return NULL;
// }

// void* atender_solicitudes_dialfs(void* args) {
//     bool esperar = true;
//     while (esperar) {
//         int cod_operacion = recibir_operacion(fd_kernel);
//         switch (cod_operacion) {
//             // Agregar casos para las operaciones de DialFS
//             case -1:
//                 log_error(logger_entradasalida, "ERROR: Ha surgido un problema inesperado, se desconectó el Kernel.");
//                 esperar = false;
//                 break;
//             default:
//                 log_warning(logger_entradasalida, "WARNING: El módulo de entrada/salida ha recibido una solicitud con una operación desconocida");
//                 break;
//         }
//     }
//     return NULL;
// }

//====================================================
// FUNCIONES PARA CIERRE DE CLIENTE DE ENTRADA/SALIDA
//====================================================

void cerrar_cliente() {
    _cerrar_puertos();
    _cerrar_conexiones();
    log_info(logger_entradasalida, "Cliente de entrada/salida cerrado correctamente.");
}

void _cerrar_puertos() {
    // Liberar los puertos utilizados por el cliente de entrada/salida
    free(kernel_port);
    free(memoria_port);
    // Liberar otros puertos si es necesario
}

void _cerrar_conexiones() {
    // Liberar las conexiones utilizadas por el cliente de entrada/salida
    liberar_conexion(fd_kernel);
    liberar_conexion(fd_memoria);
    // Liberar otras conexiones si es necesario
}