#ifndef INICIALIZAR_CPU_H_
#define INICIALIZAR_CPU_H_

#include <stdio.h>
#include <stdlib.h>
#include <commons/log.h>
#include "utils/estructuras.h"
#include <commons/config.h>
#include "cpu_config.h"
#include <variables_globales.h>
#include "cpu_syncro.h"

void init(char* path);
void _iniciar_logger();
void _iniciar_config(char* path);
void _init_cpu_registers();
void imprimir_config();


#endif /*INICIALIZAR_CPU_H_*/