#ifndef VARIABLES_GLOBALES_H_
#define VARIABLES_GLOBALES_H_

#include <stdio.h>
#include <stdlib.h>
#include <commons/log.h>
#include <commons/config.h>
#include <commons/string.h>
#include <pthread.h>


#define SERVER_MEMORIA "SERVER MEMORIA"
#define SERVER_KERNEL "SERVER KERNEL"

// @@@@@ IMPORTANTE: @@@@@
// el EXTERN nos ayuda a que las librerias que importen este .h no vuelvan a declarar las variables,
// sino que simplemente las ulilizan.
// Aca agregamos las variables globales.

// VARIABLES GLOBALES:
extern int conexion_io_memoria;
extern int conexion_io_kernel;
extern int fd_entradasalida;
extern int fd_memoria;
extern int fd_kernel;
extern t_log *logger_entradasalida;
extern t_log *logger_entradasalida_debug;
extern t_config *config_entradasalida;
// t_entradasalida_config* entradasalida_config;

// Datos de la CONFIG:
extern char *TIPO_INTERFAZ;
extern int TIEMPO_UNIDAD_TRABAJO;
extern char *IP_KERNEL;
extern char *PUERTO_KERNEL;
extern char *IP_MEMORIA;
extern char *PUERTO_MEMORIA;
extern char *PATH_BASE_DIALFS;
extern int BLOCK_SIZE;
extern int BLOCK_COUNT;

#endif