#include <memoria.h>

int main(void)
{
    init();
   
    levantar_servidor();

    esperar_clientes();

    crear_hilos_conexiones();

    // Libero recursos
    cerrar_servidor();

    return EXIT_SUCCESS;
}