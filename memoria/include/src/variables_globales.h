#ifndef VARIABLES_GLOBALES_H_
#define VARIABLES_GLOBALES_H_

#include <commons/log.h>
#include <commons/config.h>
#include <commons/collections/dictionary.h>
#include "memoria_config.h"

#define SERVERNAME "MEMORIA"
#define CLIENTE_CPU "CLIENTE CPU"
#define CLIENTE_KERNEL "CLIENTE KERNEL"
#define CLIENTE_ENTRADASALIDA "CLIENTE ENTRADASALIDA"
#define MAX_LINE_LENGTH 256

// VARIABLES GLOBALES
extern t_dictionary* tabla_procesos;
extern t_log* logger_memoria;
extern t_log* logger_memoria_debug;

//CONFIG
extern t_config* config_memoria;
extern t_memoria_config* memoria_config;

//FD
extern int fd_server;
extern int fd_entradasalida;
extern int fd_cpu;
extern int fd_kernel;

//PORTS
extern char* server_port;

#endif /*VARIABLES_GLOBALES_H_*/