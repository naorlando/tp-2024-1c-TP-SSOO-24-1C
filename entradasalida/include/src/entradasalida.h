#ifndef ENTRADASALIDA_H_
#define ENTRADASALIDA_H_

#include <stdio.h>
#include <stdlib.h>
#include <commons/log.h>
#include <commons/config.h>
#include <commons/string.h>
#include <utils/sockets.h>

// VARIABLES GLOBALES:

t_log *logger_entradasalida;
t_log *logger_entradasalida_debug;
t_config *config_entradasalida;
// t_entradasalida_config* entradasalida_config;

// Datos de la CONFIG:
char* TIPO_INTERFAZ;
int TIEMPO_UNIDAD_TRABAJO;
char* IP_KERNEL;
char* PUERTO_KERNEL;
char* IP_MEMORIA;
char* PUERTO_MEMORIA;
char* PATH_BASE_DIALFS;
int BLOCK_SIZE;
int BLOCK_COUNT;

void init();
void _iniciar_logger();
void _iniciar_config();

#endif /*ENTRADASALIDA_H_*/