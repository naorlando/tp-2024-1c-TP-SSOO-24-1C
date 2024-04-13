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
    int fd_cliente = esperar_cliente(logger_memoria, NULL, fd_server);

    liberar_conexion(fd_cliente);

    free(server_port);

    return EXIT_SUCCESS;
}