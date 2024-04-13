#ifndef INICIALIZAR_MEMORIA_H_
#define INICIALIZAR_MEMORIA_H_

#include <stdio.h>
#include <stdlib.h>
#include <commons/log.h>
#include <commons/config.h>
#include "memoria_config.h"

// VARIABLES GLOBALES
extern t_log* logger_memoria;
extern t_log* logger_memoria_debug;
extern t_config* config_memoria;
extern t_memoria_config* memoria_config;

void init();
void _iniciar_logger();
void _iniciar_config();
void imprimir_config();

#endif /*INICIALIZAR_MEMORIA_H_*/