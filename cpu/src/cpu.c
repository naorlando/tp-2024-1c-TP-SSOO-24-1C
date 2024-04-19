
#include "cpu.h"

int main(int argc, char *argv[])
{

    init();

    log_info(logger_cpu, "Archivo de configuracion cargado correctamente");

    // ############## Socket ###################

    char *server_port = string_itoa(cpu_config->PUERTO_ESCUCHA_DISPATCH);

    int fd_server = iniciar_servidor(logger_cpu, NULL, NULL, server_port);

    if (fd_server != -1)
    {
        log_info(logger_cpu, "%s server listo escuchando en puerto %s", SERVERNAME, server_port);
    }
    else
    {
        log_error(logger_cpu, "Error al iniciar %s server en puerto %s", SERVERNAME, server_port);
    }

    // Conectamos CPU -> Memoria
    char *memoria_client = "MEMORIA";
    char *puerto_memoria = string_itoa(cpu_config->PUERTO_MEMORIA);
    int conexion_memoria = crear_conexion(logger_cpu, memoria_client, cpu_config->IP_MEMORIA, puerto_memoria);

    // Esperamos al Kernel
    fd_kernel_dispatch = esperar_cliente(logger_cpu, "KERNEL", fd_server);

    // Atendemos mensaje del Kernel
    pthread_t hilo_kernel_dispatch;
    pthread_create(&hilo_kernel_dispatch, NULL, (void *)atender_cpu_kernel_dispatch, NULL);
    // pthread_detach(hilo_kernel_dispatch);
    pthread_join(hilo_kernel_dispatch, NULL);
    // atender_cpu_kernel_interrupt(logger_cpu);

    // free(server_port);

    // return EXIT_SUCCESS;
}
