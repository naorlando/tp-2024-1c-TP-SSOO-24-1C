#include <entradasalida.h>


int main(int argc, char *argv[]) {
    
    // CONEXIONES: nos vamos a conectar a Memoria y a Kernel.
    int conexion_io_memoria;
    
    //Conexion como cliente a MEMORIA:
    

    init();

    conexion_io_memoria = crear_conexion(logger_entradasalida,"MEMORIA",IP_MEMORIA, PUERTO_MEMORIA);
    conexion_io_kernel = crear_conexion(logger_entradasalida,"KERNEL",IP_KERNEL, PUERTO_KERNEL);
    
    while(1){}

    liberar_conexion(conexion_io_memoria);
    liberar_conexion(conexion_io_kernel);
    return EXIT_SUCCESS;
}


