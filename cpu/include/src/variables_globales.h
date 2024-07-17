#ifndef VARIABLES_GLOBALES_H
#define VARIABLES_GLOBALES_H

#include <stdio.h>
#include <stdlib.h>
#include <commons/log.h>
#include "utils/estructuras.h"
#include "utils/interruption.h"
#include "cpu_config.h"
#include <semaphore.h>
#include <pthread.h>


#define SERVERNAME "CPU"
#define SERVER_MEMORIA "SERVER MEMORIA"
#define CLIENTE_KERNEL "CLIENTE KERNEL"

// VARIABLES GLOBALES
extern t_log *logger_cpu;
extern t_log* logger_cpu_debug;
extern t_cpu_registers* cpu_registers;
extern t_PCB* pcb_execute;

extern bool interrupcion_pendiente;
extern t_name_interruption tipo_de_interrupcion;
extern bool llego_a_exit;
extern bool solicitud_io;
extern bool solicitud_recurso;


// Variables globales


//PAGES
extern uint32_t page_size;
extern uint32_t page_table_entries_qty; 

//Semaforos
extern sem_t SEM_INTERRUPT; // BINARIO -> Revisar si sirve!
extern sem_t SEM_SOCKET_KERNEL_DISPATCH;

// MUTEXS
extern pthread_mutex_t MUTEX_INTERRUPT;

// CONFIG
extern t_config *config_cpu;
extern t_cpu_config *cpu_config;

// FD
extern int fd_server_dispatch;
extern int fd_server_interrupt;
extern int fd_kernel_dispatch;
extern int fd_kernel_interrupt;
extern int fd_memoria;

//PORTS
extern char* server_port_dispatch;
extern char* server_port_interrupt;
extern char* memoria_port;

// SEMAFOROS
//static pthread_mutex_t mutex_cpu_registers = PTHREAD_MUTEX_INITIALIZER;

#endif