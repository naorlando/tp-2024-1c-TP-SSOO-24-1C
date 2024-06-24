#ifndef VARIABLES_GLOBALES_H_
#define VARIABLES_GLOBALES_H_

#include <commons/log.h>
#include <commons/config.h>
#include "entradasalida_config.h"

#define SERVER_MEMORIA "SERVER MEMORIA"
#define SERVER_KERNEL "SERVER KERNEL"

// VARIABLES GLOBALES:
extern t_log *logger_entradasalida;
extern t_log *logger_entradasalida_debug;
extern char* nombre_interfaz;

//FD
extern int fd_memoria;
extern int fd_kernel;

//PORTS
extern char* memoria_port;
extern char* kernel_port;

//CONFIG
extern t_config *config_entradasalida;
extern t_IO_config* entradasalida_config;

#endif /*VARIABLES_GLOBALES_H_*/