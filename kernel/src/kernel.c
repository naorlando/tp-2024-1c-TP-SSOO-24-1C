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

    char *puerto_cpu_dispatch = string_itoa(kernel_config->PUERTO_CPU_DISPATCH);

    char *puerto_memoria = string_itoa(kernel_config->PUERTO_MEMORIA);

    fd_cpu_dispatch = crear_conexion(logger_kernel, SERVER_CPU, kernel_config->IP_CPU, puerto_cpu_dispatch);
    fd_cpu_dispatch > 0 ? send_example_cpu() : log_error(logger_kernel, "Error al intentar enviar mensaje a %s", SERVER_CPU);

    fd_kernel_memoria = crear_conexion(logger_kernel, SERVER_MEMORIA, kernel_config->IP_MEMORIA, puerto_memoria);
    fd_kernel_memoria > 0 ? send_example_memoria() : log_error(logger_kernel, "Error al intentar enviar mensaje a %s", SERVER_MEMORIA);

    fd_kernel_IO = esperar_cliente(logger_kernel, CLIENTE_ENTRADASALIDA, fd_server);

    pthread_t hilo_cpu_dispatch;
    pthread_create(&hilo_cpu_dispatch, NULL, (void *)atender_kernel_IO, NULL);
    // pthread_detach(hilo_kernel_dispatch);
    pthread_join(hilo_cpu_dispatch, NULL);
}
