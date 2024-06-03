#include <kernel.h>

int main(int argc, char *argv[])
{

    init();

    levantar_servidor();

    inicializar_sockets();

    crear_hilos_conexiones();

    // CONSOLA INTERACTIVA:
    // ejecuto la consola en el hilo principal del programa.
    iniciar_consola_interactiva();

    cerrar_servidor();
    
    return EXIT_SUCCESS;
}
