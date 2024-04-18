#include "inicializar_kernel.h"

t_log* logger_kernel;
t_log* logger_kernel_debug;
t_config* config_kernel;
t_kernel_config* kernel_config;

void init(){
    _iniciar_logger();
    _iniciar_config();
    imprimir_config();
}

void _iniciar_logger(){
    logger_kernel = log_create("kernel.log", "KERNEL_LOG", 1, LOG_LEVEL_INFO);

    if(logger_kernel == NULL){
        perror("No se pudo crear el archivo log para el modulo de kernel");
        //exit(EXIT_FAILURE);
    }

    logger_kernel_debug = log_create("kernel.log", "KERNEL_LOG", 1, LOG_LEVEL_TRACE);

    if(logger_kernel_debug == NULL){
        perror("No se pudo crear el archivo log de debug para el modulo de kernel");
        //exit(EXIT_FAILURE);
    }
}

void _iniciar_config(){
    config_kernel = config_create("/home/utnso/tp-2024-1c-TP-SSOO-24-1C/kernel/cfg/kernel.config");

    if(config_kernel == NULL){
        perror("No se pudo crear el archivo de configuracion para el modulo de kernel");
        //exit(EXIT_FAILURE);
    }

    //Creo la estructura para guardar los datos del archivo de configuracion
    kernel_config = crear_kernel_config();

    if(kernel_config == NULL){
        log_error(logger_kernel, "Error al crear la estructura t_kernel_config");
    }

    cargar_kernel_config(kernel_config, config_kernel);
}

void imprimir_config() {
    log_trace(logger_kernel_debug, "PUERTO DE ESCUCHA DE LA KERNEL: %d", kernel_config->PUERTO_ESCUCHA);
}