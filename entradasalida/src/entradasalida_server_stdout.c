#include "entradasalida_server_stdout.h"

void crear_hilos_conexiones_stdout() {
    pthread_t hilo_stdout;

    if (pthread_create(&hilo_stdout, NULL, (void *)atender_solicitudes_stdout, NULL) != 0) {
        log_error(logger_entradasalida, "Error al crear el hilo para atender la interfaz STDOUT. ABORTANDO");
        exit(EXIT_FAILURE);
    }

    pthread_detach(hilo_stdout);
}

void *atender_solicitudes_stdout(void *args) {
    bool esperar = true;

    while (esperar) {
        int cod_operacion = recibir_operacion(fd_kernel);

        switch (cod_operacion) {
            case IO_STDOUT:
                atender_instruccion_stdout(fd_kernel);
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

    return NULL;
}

void inicializar_sockets_stdout() {
    kernel_port = string_itoa(obtener_puerto_kernel(entradasalida_config));
    fd_kernel = crear_conexion(logger_entradasalida, SERVER_KERNEL, obtener_ip_kernel(entradasalida_config), kernel_port);

    if (fd_kernel == -1) {
        log_error(logger_entradasalida, "Error al conectar con el Kernel. ABORTANDO");
        exit(EXIT_FAILURE);
    }

    memoria_port = string_itoa(obtener_puerto_memoria(entradasalida_config));
    fd_memoria = crear_conexion(logger_entradasalida, SERVER_MEMORIA, obtener_ip_memoria(entradasalida_config), memoria_port);

    if (fd_memoria == -1) {
        log_error(logger_entradasalida, "Error al conectar con la Memoria. ABORTANDO");
        exit(EXIT_FAILURE);
    }
}