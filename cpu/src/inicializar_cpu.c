#include "inicializar_cpu.h"
t_log* logger_cpu;
t_log* logger_cpu_debug;
t_config* config_cpu;
t_cpu_config* cpu_config;
int fd_kernel_dispatch;
int fd_memoria;
t_cpu_registers* cpu_registers;
bool interrupcion_pendiente= false;
t_PCB* pcb_execute;

void init(){
    _iniciar_logger();
    _iniciar_config();
    imprimir_config();
    _init_cpu_registers();
}

void _init_cpu_registers() {
    cpu_registers = malloc(sizeof(t_cpu_registers));
    
    cpu_registers->pc = 0;
    cpu_registers->ax = 0;
    cpu_registers->bx = 0;
    cpu_registers->cx = 0;
    cpu_registers->dx = 0;
    cpu_registers->eax = 0;
    cpu_registers->ebx = 0;
    cpu_registers->ecx = 0;
    cpu_registers->edx = 0;
    cpu_registers->si = 0;
    cpu_registers->di = 0;

   
    log_info(logger_cpu, "Se inicializaron los registros de la CPU");
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