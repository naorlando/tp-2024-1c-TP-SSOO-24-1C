#include <entradasalida.h>

int main(int argc, char *argv[])
{

    init();

    fd_memoria = crear_conexion(logger_entradasalida, SERVER_MEMORIA, IP_MEMORIA, PUERTO_MEMORIA);
    fd_memoria > 0 ? send_example_memoria() : log_error(logger_entradasalida, "Error al intentar enviar mensaje a %s", SERVER_MEMORIA);
    fd_kernel = crear_conexion(logger_entradasalida, SERVER_KERNEL, IP_KERNEL, PUERTO_KERNEL);
    fd_kernel > 0 ? send_example_kernel() : log_error(logger_entradasalida, "Error al intentar enviar mensaje a %s", SERVER_KERNEL);

    // Atendemos mensaje del Kernel
    pthread_t hilo_kernel;
    pthread_create(&hilo_kernel, NULL, (void *)atender_io_kernel, NULL);
    pthread_detach(hilo_kernel);

    //Atender los mensajes de la memoria

    pthread_t hilo_memoria;
    pthread_create(&hilo_memoria,NULL,(void *)atender_io_memoria,NULL);
    pthread_join(hilo_memoria,NULL);



    liberar_conexion(fd_memoria);
    liberar_conexion(fd_kernel);
    return EXIT_SUCCESS;
}
