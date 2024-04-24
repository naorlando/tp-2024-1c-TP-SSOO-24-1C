#include <memoria.h>

int main(void)
{
    init();

    // ############## Socket ###################

    char *server_port = string_itoa(memoria_config->PUERTO_ESCUCHA);
    fd_server = iniciar_servidor(logger_memoria, SERVERNAME, NULL, server_port);

    if (fd_server != -1)
    {
        log_info(logger_memoria, "%s server listo escuchando en puerto %s", SERVERNAME, server_port);
    }
    else
    {
        log_error(logger_memoria, "Error al iniciar %s server en puerto %s", SERVERNAME, server_port);
    }

    // ############## Espera de Clientes ###################

    // Espera conexion de la CPU
    log_info(logger_memoria, "esperando a que se conecte CPU");
    fd_cpu = esperar_cliente(logger_memoria, CLIENTE_CPU, fd_server);
        //requests_cpu();

    // Espera conexion del Kernel
    log_info(logger_memoria, "esperando a que se conecte KERNEL");
    fd_kernel = esperar_cliente(logger_memoria, CLIENTE_KERNEL, fd_server);
        // requests_kernel();

    // Espera conexion de la Interfaz I/O
    log_info(logger_memoria, "esperando a que se conecte ENTRADASALIDA");
    fd_entradasalida = esperar_cliente(logger_memoria, CLIENTE_ENTRADASALIDA, fd_server);
        // requests_entradasalida();

    // ############## Escucha de Mensajes ###################
    // Escucha de mensaje Kernel
    
    pthread_t hilo_kernel;
    pthread_create(&hilo_kernel, NULL, (void *)requests_kernel, NULL);
    pthread_detach(hilo_kernel);

    // Escucha de mensaje CPU

    pthread_t hilo_cpu;
    pthread_create(&hilo_cpu, NULL, (void *)requests_cpu, NULL);
    pthread_detach(hilo_cpu);

    // Escucha de mensaje EntradaSalida

    pthread_t hilo_entradaSalida;
    pthread_create(&hilo_entradaSalida, NULL, (void *)requests_entradasalida, NULL);
    pthread_join(hilo_entradaSalida, NULL);

    // pthread_t hilo_keep_alive;
    // pthread_create(&hilo_keep_alive, NULL, (void *)solicitudes_cpu, NULL);
    //  pthread_detach(hilo_kernel_dispatch);
    // pthread_join(hilo_keep_alive, NULL);

    // Libero recursos
    liberar_conexion(fd_kernel);
    liberar_conexion(fd_cpu);
    liberar_conexion(fd_entradasalida);

    free(server_port);
    log_info(logger_memoria,"server liberado");

    return EXIT_SUCCESS;
}