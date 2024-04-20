#include <entradasalida.h>

int main(int argc, char *argv[])
{

    init();

    fd_memoria = crear_conexion(logger_entradasalida, SERVER_MEMORIA, IP_MEMORIA, PUERTO_MEMORIA);
    fd_memoria > 0 ? send_example_memoria() : log_error(logger_entradasalida, "Error al intentar enviar mensaje a %s", SERVER_MEMORIA);
    fd_kernel = crear_conexion(logger_entradasalida, SERVER_KERNEL, IP_KERNEL, PUERTO_KERNEL);
    fd_kernel > 0 ? send_example_kernel() : log_error(logger_entradasalida, "Error al intentar enviar mensaje a %s", SERVER_KERNEL);



    liberar_conexion(fd_memoria);
    liberar_conexion(fd_kernel);
    return EXIT_SUCCESS;
}
