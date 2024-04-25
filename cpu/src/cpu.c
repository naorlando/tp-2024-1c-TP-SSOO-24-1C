
#include <cpu.h>

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

    char *puerto_memoria = string_itoa(cpu_config->PUERTO_MEMORIA);
    fd_memoria = crear_conexion(logger_cpu, SERVER_MEMORIA, cpu_config->IP_MEMORIA, puerto_memoria);
    fd_memoria > 0 ? send_example_memoria() : log_error(logger_cpu, "Error al intentar enviar mensaje a %s", SERVER_MEMORIA);

    // Esperamos al Kernel
    log_info(logger_cpu, "esperando a que se conecte %s",CLIENTE_KERNEL);
    fd_kernel_dispatch = esperar_cliente(logger_cpu, CLIENTE_KERNEL, fd_server);


    // Atendemos mensaje del Kernel
    pthread_t hilo_kernel_dispatch;
    pthread_create(&hilo_kernel_dispatch, NULL, (void *)atender_cpu_kernel_dispatch, NULL);
    pthread_join(hilo_kernel_dispatch, NULL);

    free(server_port);
    liberar_conexion(fd_server);
    liberar_conexion(fd_memoria);
    liberar_conexion(fd_kernel_dispatch);    

    return EXIT_SUCCESS;
}
