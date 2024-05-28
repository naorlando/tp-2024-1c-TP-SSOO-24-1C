#include <entradasalida.h>

int main(int argc, char *argv[])
{

    init();

    // Comento bloque de conexión con memoria (checkpoint 1)
    fd_memoria = crear_conexion(logger_entradasalida, SERVER_MEMORIA, IP_MEMORIA, PUERTO_MEMORIA);
    fd_memoria > 0 ? send_example_memoria() : log_error(logger_entradasalida, "Error al intentar enviar mensaje a %s", SERVER_MEMORIA);

    // Comento bloque de conexión con kernel (checkpoint 1)
    // fd_kernel = crear_conexion(logger_entradasalida, SERVER_KERNEL, IP_KERNEL, PUERTO_KERNEL);
    // fd_kernel > 0 ? send_example_kernel() : log_error(logger_entradasalida, "Error al intentar enviar mensaje a %s", SERVER_KERNEL);

    // Reemplazo el código anterior quitando la conexión con Memoria, ya que la Interfaz Genérica no la necesita
    
    fd_kernel = crear_conexion(logger_entradasalida, SERVER_KERNEL, IP_KERNEL, PUERTO_KERNEL);
    fd_kernel > 0 ? log_info(logger_entradasalida, "Conexión exitosa con kernel")
                  : log_error(logger_entradasalida, "Error al conectar con kernel");

    // Atendemos mensaje del Kernel

    pthread_t hilo_kernel;
    pthread_create(&hilo_kernel, NULL, (void *)requests_kernel, NULL);
    pthread_detach(hilo_kernel);
    //pthread_join(hilo_kernel, NULL);

    //Atender los mensajes de la memoria (comentado), no lo necesitamos para la interfaz genérica
    pthread_t hilo_memoria;
    pthread_create(&hilo_memoria, NULL, (void *)requests_memoria,NULL);
    pthread_join(hilo_memoria,NULL);

    // Agrego un hilo para esperar indefinidamente a recibir solicitudes del kernel (no es necesario un hilo para atender memoria)
    
    //pthread_join(hilo_kernel, NULL);

    // Liberamos Conexiones
    
    // comento la liberación de la conexión con memoria
    liberar_conexion(fd_memoria);
    liberar_conexion(fd_kernel);
    return EXIT_SUCCESS;
}
