#ifndef INICIALIZAR_KERNEL_H_
#define INICIALIZAR_KERNEL_H_

#include <stdio.h>
#include <stdlib.h>
#include <commons/log.h>
#include <commons/config.h>

#include "kernel_config.h"
#include <variables_globales.h>
#include "table_pcb.h"
#include "largo_plazo.h"
#include "corto_plazo.h"



void init();
void _iniciar_logger();
void _iniciar_config();
void imprimir_config();
void _init_table_pcb();
void initialize_lists();
void inicializar_planificadores();
void inicializar_dictionarios();

#endif /*INICIALIZAR_KERNEL_H_*/