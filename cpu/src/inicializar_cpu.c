#include "inicializar_cpu.h"
t_log* logger_cpu;
t_log* logger_cpu_debug;
t_config* config_cpu;
t_cpu_config* cpu_config;
int fd_kernel_dispatch;
int fd_memoria;

void init(){
    _iniciar_logger();
    _iniciar_config();
    imprimir_config();
}

void _iniciar_logger(){
    logger_cpu = log_create("cpu.log", "CPU_LOG", 1, LOG_LEVEL_INFO);

    if(logger_cpu == NULL){
        perror("No se pudo crear el archivo log para el modulo de cpu");
        //exit(EXIT_FAILURE);
    }

    logger_cpu_debug = log_create("cpu.log", "CPU_LOG", 1, LOG_LEVEL_TRACE);

    if(logger_cpu_debug == NULL){
        perror("No se pudo crear el archivo log de debug para el modulo de cpu");
        //exit(EXIT_FAILURE);
    }
}

void _iniciar_config(){
    config_cpu = config_create("/home/utnso/tp-2024-1c-TP-SSOO-24-1C/cpu/cfg/cpu.config");

    if(config_cpu == NULL){
        perror("No se pudo crear el archivo de configuracion para el modulo de cpu");
        //exit(EXIT_FAILURE);
    }

    //Creo la estructura para guardar los datos del archivo de configuracion
    cpu_config = crear_cpu_config();

    if(cpu_config == NULL){
        log_error(logger_cpu, "Error al crear la estructura t_cpu_config");
    }

    cargar_cpu_config(cpu_config, config_cpu);
}

void imprimir_config() {
    log_trace(logger_cpu_debug, "PUERTO DE ESCUCHA DE LA CPU: %d", cpu_config->PUERTO_ESCUCHA_DISPATCH);
}