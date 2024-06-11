#include <entradasalida.h>

int main(int argc, char *argv[])
{
    if (argc != 3) {
        fprintf(stderr, "Uso correcto: %s <nombre_interfaz> <path_config>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char* nombre_interfaz = argv[1];
    char* path_config = argv[2];

    init(nombre_interfaz, path_config);

    inicializar_sockets();

    crear_hilos_conexiones();

    cerrar_cliente();

    return EXIT_SUCCESS;
}
