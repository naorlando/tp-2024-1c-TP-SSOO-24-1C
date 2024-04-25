#include <entradasalida.h>

int main(int argc, char *argv[])
{

    init();

    fd_memoria = crear_conexion(logger_entradasalida, SERVER_MEMORIA, IP_MEMORIA, PUERTO_MEMORIA);
    fd_memoria > 0 ? send_example_memoria() : log_error(logger_entradasalida, "Error al intentar enviar mensaje a %s", SERVER_MEMORIA);

    fd_kernel = crear_conexion(logger_entradasalida, SERVER_KERNEL, IP_KERNEL, PUERTO_KERNEL);
    fd_kernel > 0 ? send_example_kernel() : log_error(logger_entradasalida, "Error al intentar enviar mensaje a %s", SERVER_KERNEL);

    while(1){}

//CHEQUEAR: 
       // ############## Escucha de Mensajes ###################

        // ENTIENDO QUE AL SER BIDIRECCIONAL LA COMUNICACION DE ESTE MODULO, este NECESITA de REQUES
        // aunque no sea un modulo con socket servidor. (problema disparador, que se terminaba la ejecucion sin el while(1){}).
        // -->


    // Escucha de mensaje Kernel
    
    // pthread_t hilo_kernel;
    // pthread_create(&hilo_kernel, NULL, (void *)requests_kernel, NULL);
    // pthread_join(hilo_kernel,NULL); //hace que no se cierre el modulo en ejecucion

    // Liberamos Conexiones

    liberar_conexion(fd_memoria);
    liberar_conexion(fd_kernel);
    return EXIT_SUCCESS;
}
