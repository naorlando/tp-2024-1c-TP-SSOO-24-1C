#include <kernel.h>

int main(int argc, char *argv[])
{

    init();

    // ############## Socket ###################

    char *server_port = string_itoa(kernel_config->PUERTO_ESCUCHA);
    int fd_server = iniciar_servidor(logger_kernel, NULL, NULL, server_port);

    if (fd_server != -1)
    {
        log_info(logger_kernel, "%s server listo escuchando en puerto %s", SERVERNAME, server_port);
    }
    else
    {
        log_error(logger_kernel, "Error al iniciar %s server en puerto %s", SERVERNAME, server_port);
    }

    free(server_port);

    return EXIT_SUCCESS;
    // char *server_port = string_itoa(kernel_config->PUERTO_ESCUCHA);
    // int fd_server;

    // fd_server = iniciar_servidor(
    //     kernel_logger,
    //     SERVERNAME,
    //     "0.0.0.0",
    //     server_port);

    // if (fd_server != -1)
    //     log_info(kernel_logger, "%s server listo escuchando en puerto %s", SERVERNAME, server_port);
    // else
    //     log_error(kernel_logger, "Error al iniciar %s server en puerto %s", SERVERNAME, server_port);

    // free(server_port);

    // config_destroy(cfg_aux);
}
