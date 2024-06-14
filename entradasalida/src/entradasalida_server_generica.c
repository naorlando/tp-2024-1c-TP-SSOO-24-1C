#include "entradasalida_server_generica.h"

void crear_hilos_conexiones_generica() {
    pthread_t hilo_generica;

    if (pthread_create(&hilo_generica, NULL, (void *)atender_solicitudes_generica, NULL) != 0) {
        log_error(logger_entradasalida, "Error al crear el hilo para atender la interfaz genérica. ABORTANDO");
        exit(EXIT_FAILURE);
    }

    pthread_detach(hilo_generica);
}

void *atender_solicitudes_generica(void *args) {
    bool esperar = true;

    while (esperar) {
        int cod_operacion = recibir_operacion(fd_kernel);

        switch (cod_operacion) {
            case IO_GENERICA:
                atender_instruccion_generica(fd_kernel);
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

void inicializar_sockets_generica() {
    kernel_port = string_itoa(obtener_puerto_kernel(entradasalida_config));
    fd_kernel = crear_conexion(logger_entradasalida, SERVER_KERNEL, obtener_ip_kernel(entradasalida_config), kernel_port);

    if (fd_kernel == -1) {
        log_error(logger_entradasalida, "Error al conectar con el Kernel. ABORTANDO");
        exit(EXIT_FAILURE);
    }
}