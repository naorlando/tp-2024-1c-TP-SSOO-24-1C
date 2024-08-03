#include <memoria.h>

int main(int argc, char *argv[])
{
    init(argv[1]);
   
    levantar_servidor();

    esperar_clientes();

    crear_hilos_conexiones();

    // Libero recursos
    cerrar_servidor();

    return EXIT_SUCCESS;
}