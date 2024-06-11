#ifndef VARIABLES_GLOBALES_H
#define VARIABLES_GLOBALES_H

#include <stdio.h>
#include <stdlib.h>
#include <commons/log.h>
#include "utils/estructuras.h"
#include "cpu_config.h"

#define SERVERNAME "CPU"
#define SERVER_MEMORIA "SERVER MEMORIA"
#define CLIENTE_KERNEL "CLIENTE KERNEL"

// VARIABLES GLOBALES
extern t_log *logger_cpu;
extern t_log* logger_cpu_debug;
extern t_cpu_registers* cpu_registers;
extern t_PCB* pcb_execute;
extern bool interrupcion_pendiente;

// CONFIG
extern t_config *config_cpu;
extern t_cpu_config *cpu_config;

// FD
extern int fd_kernel_dispatch;
extern int fd_kernel_interrupt;
extern int fd_memoria;

// SEMAFOROS
//static pthread_mutex_t mutex_cpu_registers = PTHREAD_MUTEX_INITIALIZER;

#endif