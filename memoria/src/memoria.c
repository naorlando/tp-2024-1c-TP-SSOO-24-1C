#include "memoria.h"

int main(void){
    init();

    // ############## Socket ###################

    char* server_port = string_itoa(memoria_config->PUERTO_ESCUCHA);
    int fd_server = iniciar_servidor(logger_memoria, NULL, NULL, server_port);

    if (fd_server != -1) {
        log_info(logger_memoria, "%s server listo escuchando en puerto %s", SERVERNAME, server_port);
    } else {
        log_error(logger_memoria, "Error al iniciar %s server en puerto %s", SERVERNAME, server_port);
    }

    // ############## Espera de Clientes ###################
    // Espera conexion del Kernel
    int fd_kernel = esperar_cliente(logger_memoria, NULL, fd_server);
    
    // Espera conexion de la CPU
    int fd_cpu = esperar_cliente(logger_memoria, NULL, fd_server);

    // Espera conexion de la Interfaz I/O
    int fd_entradasalida = esperar_cliente(logger_memoria, NULL, fd_server);

    // ############## Escucha de Mensajes ###################
    // Escucha de mensaje Kernel
    solicitudes_kernel(fd_kernel, logger_memoria);

    // Escucha de mensaje CPU
    solicitudes_cpu(fd_cpu, logger_memoria);

    // Escucha de mensaje EntradaSalida
    solicitudes_entradasalida(fd_entradasalida, logger_memoria);

    //Libero recursos
    liberar_conexion(fd_kernel);
    liberar_conexion(fd_cpu);
    liberar_conexion(fd_entradasalida);
    free(server_port);

    return EXIT_SUCCESS;
}