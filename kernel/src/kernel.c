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

    // ############## Espera de I/O ###################
    
    log_info(logger_kernel, "esperando a que se conecte %s",CLIENTE_ENTRADASALIDA);
    fd_kernel_IO = esperar_cliente(logger_kernel, CLIENTE_ENTRADASALIDA, fd_server);

// ????????????????    
    // fd_kernel_IO > 0 ? requests_entradasalida() : log_error(logger_kernel, "Error al intentar recibir mensaje a %s", CLIENTE_ENTRADASALIDA);
// ????????? es responsabilidad del main este chequeo?

   
    // Atendemos mensaje CPU DISPATCH
    pthread_t hilo_cpu_dispatch;
    pthread_create(&hilo_cpu_dispatch, NULL,(void *)atender_kernel_cpu_dispatch, NULL);
    pthread_detach(hilo_cpu_dispatch);

    // Atendemos mensaje del Entradasalida
    pthread_t hilo_entradasalida;
    pthread_create(&hilo_entradasalida, NULL,(void *)atender_kernel_IO, NULL);
    pthread_detach(hilo_entradasalida);

    //Atender los mensajes de la memoria

    pthread_t hilo_memoria;
    pthread_create(&hilo_memoria,NULL,(void *)atender_kernel_memoria,NULL);
    pthread_join(hilo_memoria,NULL);

    free(server_port);
    liberar_conexion(fd_server);
    liberar_conexion(fd_kernel_memoria);
    liberar_conexion(fd_cpu_dispatch);
    liberar_conexion(fd_kernel_IO);

    return EXIT_SUCCESS;
}
