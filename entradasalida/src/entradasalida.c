#include <entradasalida.h>

int main(int argc, char *argv[]) {
    if (argc != 3) { //Valido la cantidad de argumentos
        fprintf(stderr, "Uso correcto: %s <nombre_interfaz> <path_config>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char* nombre_interfaz = argv[1]; //Obtengo el nombre de la interfaz
    char* path_config = argv[2]; //Obtengo el path del archivo de configuración

    init(nombre_interfaz, path_config);

    //Inicializo la interfaz I/O, según el tipo de interfaz que se haya configurado
    if (strcmp(obtener_tipo_interfaz(entradasalida_config), "GENERICA") == 0) {
        inicializar_sockets_generica();
        crear_hilos_conexiones_generica();
    } else if (strcmp(obtener_tipo_interfaz(entradasalida_config), "STDIN") == 0) {
        inicializar_sockets_stdin();
        crear_hilos_conexiones_stdin();
    } else if (strcmp(obtener_tipo_interfaz(entradasalida_config), "STDOUT") == 0) {
        inicializar_sockets_stdout();
        crear_hilos_conexiones_stdout();
    } else if (strcmp(obtener_tipo_interfaz(entradasalida_config), "DIALFS") == 0) {
        inicializar_sockets_dialfs();
        crear_hilos_conexiones_dialfs();
    } else {
        fprintf(stderr, "Tipo de interfaz desconocido: %s\n", obtener_tipo_interfaz(entradasalida_config));
        exit(EXIT_FAILURE);
    }

    cerrar_cliente();
    return EXIT_SUCCESS;
}
