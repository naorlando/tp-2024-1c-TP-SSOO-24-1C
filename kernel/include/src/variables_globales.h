#ifndef VARIABLES_GLOBALES_H
#define VARIABLES_GLOBALES_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> 
#include <commons/log.h>
#include <commons/collections/queue.h>
#include "kernel_config.h"
#include <readline/readline.h>
#include <commons/collections/dictionary.h>
#include <commons/collections/list.h>
#include <pthread.h>
#include <semaphore.h>
#include <utils/estructuras.h>
#include <datos_hilos.h>
#include <commons/temporal.h>

#define SERVERNAME "KERNEL"
#define SERVER_CPU "SERVER CPU"
#define SERVER_CPU_DISPATCH "SERVER CPU DISPATCH"
#define SERVER_CPU_INTERRUPT "SERVER CPU INTERRUPT"
#define SERVER_MEMORIA "SERVER MEMORIA"
#define CLIENTE_ENTRADASALIDA "CLIENTE ENTRADA SALIDA"

// VARIABLES GLOBALES
extern t_log* logger_kernel;
extern t_log* logger_kernel_debug;
extern t_dictionary* table_pcb;
extern int identificador_PID;
extern t_datos_hilo* datos_hilo_quantum;
extern bool interrupcion_enviada;
extern t_dictionary* io_connections;
extern t_dictionary* recursos_dictionary;
extern t_dictionary* recursos_asignados_por_pid;

//MUTEXS
extern pthread_mutex_t MUTEX_READY;
extern pthread_mutex_t mutex_pid;
extern pthread_mutex_t MUTEX_EXIT;
extern pthread_mutex_t MUTEX_NEW;
extern pthread_mutex_t MUTEX_EXECUTE;
extern pthread_mutex_t MUTEX_DICTIONARY;
extern pthread_mutex_t MUTEX_SOLICITUD;
extern pthread_mutex_t MUTEX_RECURSOS;
extern pthread_mutex_t MUTEX_AUX_READY;
extern pthread_mutex_t MUTEX_COLA_RETORNO_PCB;

//SEMAFOROS
extern sem_t SEM_READY;
extern sem_t SEM_BLOCKED;
extern sem_t SEM_EXIT;
extern sem_t SEM_NEW;
extern sem_t SEM_MULTIPROGRAMACION;
extern sem_t SEM_CPU;
extern sem_t SEM_SOLICITUDES;
extern sem_t SEM_AUX_READY;
extern sem_t SEM_PCB_RETURNS;

//CONFIG
extern t_config* config_kernel;
extern t_kernel_config* kernel_config;

//FD
extern int fd_kernel_memoria;
extern int fd_cpu_dispatch;
extern int fd_cpu_interrupt;
extern int fd_server;
extern int fd_kernel;

//PORTS
extern char* server_port;
extern char* cpu_dispatch_port;
extern char* cpu_interrupt_port;
extern char* memoria_port;


//List
extern t_queue* COLA_READY;
extern t_queue *COLA_AUX_READY;
extern t_queue* COLA_EXIT;
extern t_queue* COLA_NEW;
extern t_queue* COLA_RETORNO_PCB;
//una variable para que el kernel conozca el pcb que este ejecutando:
extern t_PCB* EXECUTE;
extern t_list* LISTA_COLAS_DISPOSITIVOS; 
extern t_queue* SOLICITUDES;


//ESTADO PLANIFICADOR
extern bool planificador_status;


#endif