int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso correcto: %s <nombre_interfaz> <path_config>\\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Obtener los argumentos, nombre de interfaz y path del archivo de configuración
    char* nombre_interfaz = argv[1];
    char* path_config = argv[2];

    // Inicializar el módulo de entradasalida
    init(nombre_interfaz, path_config);

    // Inicializar los sockets
    inicializar_sockets();

    // Consultar el tipo de interfaz
    char* tipo_interfaz = obtener_tipo_interfaz(entradasalida_config);

    // Crear hilos según el tipo de interfaz
    if (strcmp(tipo_interfaz, "GENERICA") == 0) {
        crear_hilo_interfaz_generica();
    } else if (strcmp(tipo_interfaz, "STDIN") == 0) {
        crear_hilo_interfaz_stdin();
    } else if (strcmp(tipo_interfaz, "STDOUT") == 0) {
        crear_hilo_interfaz_stdout();
    } else if (strcmp(tipo_interfaz, "DIALFS") == 0) {
        crear_hilo_interfaz_dialfs();
    } else {
        fprintf(stderr, "Tipo de interfaz desconocido: %s\\n", tipo_interfaz);
        exit(EXIT_FAILURE);
    }

    cerrar_cliente();

    return EXIT_SUCCESS;
}