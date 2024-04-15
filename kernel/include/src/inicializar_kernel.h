#ifndef INICIALIZAR_KERNEL_H_
#define INICIALIZAR_KERNEL_H_

#include <stdio.h>
#include <stdlib.h>
#include <commons/log.h>
#include <commons/config.h>
#include "kernel_config.h"

// VARIABLES GLOBALES
extern t_log* logger_kernel;
extern t_log* logger_kernel_debug;
extern t_config* config_kernel;
extern t_kernel_config* kernel_config;

void init();
void _iniciar_logger();
void _iniciar_config();
void imprimir_config();

#endif /*INICIALIZAR_KERNEL_H_*/