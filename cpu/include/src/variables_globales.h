#ifndef VARIABLES_GLOBALES_H
#define VARIABLES_GLOBALES_H

#include <stdio.h>
#include <stdlib.h> 
#include <commons/log.h>
#include "cpu_config.h"


#define SERVERNAME "CPU"
#define SERVER_MEMORIA "SERVER MEMORIA"

// VARIABLES GLOBALES
extern t_log* cpu_kernel;
extern t_log* cpu_kernel_debug;

//CONFIG
extern t_config* config_cpu;
extern t_cpu_config* cpu_config;

//FD
extern int fd_kernel_memoria;
extern int fd_kernel_IO;
extern int fd_kernel_dispatch;

#endif