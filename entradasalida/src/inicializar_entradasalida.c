#include <inicializar_entradasalida.h>

int fd_memoria;
int fd_kernel;

t_log* logger_entradasalida;
t_log* logger_entradasalida_debug;

t_config* config_entradasalida;
t_IO_config* entradasalida_config;

char* kernel_port;
char* memoria_port;

void init(char* nombre_interfaz, char* config_path){
    _iniciar_logger();
    _iniciar_config(config_path);
    _imprimir_config(nombre_interfaz);
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

void _imprimir_config(char* nombre_interfaz) {    
    // Comento código anterior
    // log_info(logger_entradasalida, "Entrada/salida inicializado");
    
    // Reemplazo el código anterior mostrando un mensaje de inicialización de Interfaz Genérica
    log_info(logger_entradasalida, "Interfaz %s de tipo %s inicializada", nombre_interfaz, obtener_tipo_interfaz(entradasalida_config));
}

