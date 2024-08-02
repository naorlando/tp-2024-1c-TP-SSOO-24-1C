#include "inicializar_cpu.h"

t_log* logger_cpu;
t_log* logger_cpu_debug;
t_config* config_cpu;
t_cpu_config* cpu_config;

int fd_server_dispatch;
int fd_server_interrupt;
int fd_kernel_interrupt;
int fd_kernel_dispatch;
int fd_kernel_interrupt;
int fd_memoria;

t_list* TLB_list; 

t_cpu_registers* cpu_registers;
bool interrupcion_pendiente= false;
t_name_interruption tipo_de_interrupcion;
t_PCB* pcb_execute;
bool llego_a_exit = false;
bool solicitud_io = false; 
bool solicitud_recurso = false;
bool cambio_pc = false;
bool out_of_memory = false;

sem_t SEM_INTERRUPT;
sem_t SEM_SOCKET_KERNEL_DISPATCH;
pthread_mutex_t MUTEX_INTERRUPT;

char* server_port_interrupt;
char* server_port_dispatch;
char* memoria_port;



uint32_t page_size;


void init(){
    _iniciar_logger();
    _iniciar_config();
    _init_cpu_registers();
    inicializar_TLB(cpu_config->CANTIDAD_ENTRADAS_TLB, &TLB_list);
    imprimir_config();
    initializeSemaphores(); // TODO: Revisar si sirve
    initialize_mutexes();
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

    if(cargar_cpu_config(cpu_config, config_cpu)) {
        log_info(logger_cpu, "La estructura t_cpu_config fue cargado correctamente");
    } else {
        log_error(logger_cpu, "Error al cargar la estructura t_cpu_config");
    }

}

void imprimir_config() {
    log_trace(logger_cpu_debug, "PUERTO DE ESCUCHA DE LA CPU: %d", obtener_puerto_escucha_dispatch(cpu_config));
}


