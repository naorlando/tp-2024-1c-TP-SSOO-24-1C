#include <inicializar_entradasalida.h>

int fd_memoria;
int fd_kernel;

t_log* logger_entradasalida;
t_log* logger_entradasalida_debug;
char* nombre_interfaz;

t_config* config_entradasalida;
t_IO_config* entradasalida_config;

t_dialfs* DIALFS;

char* kernel_port;
char* memoria_port;

void init(char* config_path){
    _iniciar_logger();
    _iniciar_config(config_path);
    _inicializar_dialfs();
    _imprimir_config();
}

void _iniciar_logger(){

    // LOGGER OFICIAL:
    logger_entradasalida = log_create("entradasalida.log", "ENTRADASALIDA_LOG", 1, LOG_LEVEL_INFO);

    if (logger_entradasalida == NULL)
    {
        perror("No se pudo crear el archivo log para el modulo de entrada/salida");
        exit(EXIT_FAILURE);
    }

    // LOGGER DEBUG:
    logger_entradasalida_debug = log_create("entradasalida.log", "ENTRADASALIDA_LOG", 1, LOG_LEVEL_TRACE);

    if (logger_entradasalida_debug == NULL)
    {
        perror("No se pudo crear el archivo log de debug para el modulo de entrada/salida");
        exit(EXIT_FAILURE);
    }
}

void _iniciar_config(char *config_path) {
    config_entradasalida = config_create(config_path);

    if (config_entradasalida == NULL)
    {
        perror("No se pudo crear el archivo de configuracion para el modulo de entrada/salida");
        exit(EXIT_FAILURE);
    }

    //Creo la estructura para guardar los datos del archivo de configuracion
    entradasalida_config = crear_IO_config();

    if(entradasalida_config == NULL){
        log_error(logger_entradasalida, "Error al crear la estructura t_IO_config");
    }

    bool cargo = cargar_IO_config(entradasalida_config, config_entradasalida);

    if(!cargo){
        log_error(logger_entradasalida, "Error al cargar entradasalida config");
    }else{
        log_info(logger_entradasalida, "Se cargo correctamente ENTRADASALIDA.CONFIG");
    }
}

void _imprimir_config() {    
    // Comento código anterior
    // log_info(logger_entradasalida, "Entrada/salida inicializado");
    
    // Reemplazo el código anterior mostrando un mensaje de inicialización de Interfaz Genérica
    log_info(logger_entradasalida, "Interfaz %s de tipo %s inicializada", nombre_interfaz, obtener_tipo_interfaz(entradasalida_config));
}

void _inicializar_dialfs() {
    if (strcmp(obtener_tipo_interfaz(entradasalida_config), "dialfs") != 0) {
        return;
    }
    // Obtengo los datos necesarios para crear el sistema de archivos DialFS desde archivo de configuración
    char* path_base_dialfs = obtener_path_base_dialfs(entradasalida_config);
    uint32_t block_size = obtener_block_size(entradasalida_config);
    uint32_t block_count = obtener_block_count(entradasalida_config);
    uint32_t retraso_compactacion = obtener_retraso_compactacion(entradasalida_config);
    
    // Creo el sistema de archivos DialFS con los datos obtenidos
    dialfs = crear_dialfs(path_base_dialfs, block_size, block_count, retraso_compactacion);
    if (dialfs == NULL) {
        log_error(logger_entradasalida, "Error al crear el sistema de archivos DialFS.");
        exit(EXIT_FAILURE);
    }
}