#include <inicializar_entradasalida.h>

void init(){
    _iniciar_logger();
    _iniciar_config();
    _imprimir_config();

}

//================================================================//

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

void _iniciar_config() {
    config_entradasalida = config_create("/home/utnso/tp-2024-1c-TP-SSOO-24-1C/entradasalida/cfg/entradasalida.config");

    if (config_entradasalida == NULL)
    {
        perror("No se pudo crear el archivo de configuracion para el modulo de entrada/salida");
        exit(EXIT_FAILURE);
    }

        // mover a modulo que corresponda:
    TIPO_INTERFAZ = config_get_string_value(config_entradasalida, "TIPO_INTERFAZ");
    TIEMPO_UNIDAD_TRABAJO = config_get_int_value(config_entradasalida, "TIEMPO_UNIDAD_TRABAJO");
    IP_KERNEL = config_get_string_value(config_entradasalida, "IP_KERNEL");
    PUERTO_KERNEL = config_get_string_value(config_entradasalida, "PUERTO_KERNEL");
    IP_MEMORIA = config_get_string_value(config_entradasalida, "IP_MEMORIA");
    PUERTO_MEMORIA = config_get_string_value(config_entradasalida, "PUERTO_MEMORIA");
    PATH_BASE_DIALFS = config_get_string_value(config_entradasalida, "PATH_BASE_DIALFS");
    BLOCK_SIZE = config_get_int_value(config_entradasalida, "BLOCK_SIZE");
    BLOCK_COUNT = config_get_int_value(config_entradasalida, "BLOCK_COUNT");
    // ------------------------- para que? --------------------------------- //

    // Creo la estructura para guardar los datos del archivo de configuracion
    // entradasalida_config = crear_entradasalida_config();

    // if (entradasalida_config == NULL)
    // {
    //     log_error(logger_entradasalida, "Error al crear la estructura t_entradasalida_config");
    // }

    // cargar_entradasalida_config(entradasalida_config, config_entradasalida);

    // ------------------------- para que? ---------------------------------//
}

void _imprimir_config() {    
    log_info(logger_entradasalida, "TIPO_INTERFAZ: %s", TIPO_INTERFAZ);
    log_debug(logger_entradasalida_debug, "tiempo de unidad de trabajo: %d", TIEMPO_UNIDAD_TRABAJO);
    log_trace(logger_entradasalida_debug, "PUERTO_MEMORIA : %s", PUERTO_MEMORIA);
}

