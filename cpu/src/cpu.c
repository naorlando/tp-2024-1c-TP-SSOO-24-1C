
#include <cpu.h>

int main(int argc, char *argv[])
{

    init();

    levantar_servidor();

    inicializar_sockets();

    esperar_clientes();

    crear_hilos_conexiones();

    cerrar_servidor(); 

    return EXIT_SUCCESS;
}
