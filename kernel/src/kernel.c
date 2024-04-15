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

    char *ip_cpu = string_itoa(kernel_config->IP_CPU);
    char *puerto_cpu_dispatch = string_itoa(kernel_config->PUERTO_CPU_DISPATCH);
    crear_conexion(logger_kernel, "CPU", ip_cpu, puerto_cpu_dispatch)

        return EXIT_SUCCESS;
}
