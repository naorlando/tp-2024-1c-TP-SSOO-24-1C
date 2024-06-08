#ifndef VARIABLES_GLOBALES_H
#define VARIABLES_GLOBALES_H

#include <stdio.h>
#include <stdlib.h> 
#include <commons/log.h>
#include <commons/collections/queue.h>
#include "kernel_config.h"
#include <readline/readline.h>
#include <commons/collections/dictionary.h>
#include <commons/collections/list.h>
#include <pthread.h>
#include <semaphore.h>

#define SERVERNAME "KERNEL"
#define SERVER_CPU "SERVER CPU"
#define SERVER_MEMORIA "SERVER MEMORIA"
#define CLIENTE_ENTRADASALIDA "CLIENTE ENTRADA SALIDA"

// VARIABLES GLOBALES
extern t_log* logger_kernel;
extern t_log* logger_kernel_debug;
extern t_dictionary* table_pcb;
extern pthread_mutex_t  mutex_pid;
extern int identificador_PID;
extern pthread_mutex_t MUTEX_READY;
extern pthread_mutex_t MUTEX_EXIT;
extern pthread_mutex_t MUTEX_NEW;
extern sem_t SEM_READY;
extern sem_t BLOQUEADOR;
extern sem_t SEM_EXIT;
extern sem_t SEM_NEW;
extern sem_t SEM_MULTIPROGRAMACION;

//CONFIG
extern t_config* config_kernel;
extern t_kernel_config* kernel_config;

//FD
extern int fd_kernel_memoria;
extern int fd_kernel_IO;
extern int fd_cpu_dispatch;
extern int fd_server;

//PORTS
extern char* server_port;
extern char* cpu_dispatch_port;
extern char* memoria_port;


//List
extern t_queue* COLA_READY;
extern t_queue* COLA_EXIT;
extern t_queue* COLA_NEW;
extern t_list* LISTA_COLAS_DISPOSITIVOS; 


//ESTADO PLANIFICADOR
extern bool planificador_status;


#endif