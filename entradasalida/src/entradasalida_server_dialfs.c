#include "entradasalida_server_dialfs.h"

void crear_hilos_conexiones_dialfs() {
    pthread_t hilo_dialfs;

    if (pthread_create(&hilo_dialfs, NULL, (void *)atender_solicitudes_dialfs, NULL) != 0) {
        log_error(logger_entradasalida, "Error al crear el hilo para atender la interfaz DialFS. ABORTANDO");
        exit(EXIT_FAILURE);
    }

    pthread_detach(hilo_dialfs);
}

void *atender_solicitudes_dialfs(void *args) {
    bool esperar = true;

    while (esperar) {
        int cod_operacion = recibir_operacion(fd_kernel);

        switch (cod_operacion) {
            // Agregar casos para las operaciones de DialFS
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

void inicializar_sockets_dialfs() {
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