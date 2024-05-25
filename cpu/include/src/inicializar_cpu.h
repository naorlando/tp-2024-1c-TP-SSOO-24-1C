#ifndef INICIALIZAR_CPU_H_
#define INICIALIZAR_CPU_H_

#include <stdio.h>
#include <stdlib.h>
#include <commons/log.h>
#include "utils/estructuras.h"
#include <commons/config.h>
#include "cpu_config.h"

// VARIABLES GLOBALES
extern t_log* logger_cpu;
extern t_log* logger_cpu_debug;
extern t_config* config_cpu;
extern t_cpu_config* cpu_config;
extern t_cpu_registers* cpu_registers;

void init();
void _iniciar_logger();
void _iniciar_config();
void imprimir_config();
void init_cpu_registers();

#endif /*INICIALIZAR_CPU_H_*/