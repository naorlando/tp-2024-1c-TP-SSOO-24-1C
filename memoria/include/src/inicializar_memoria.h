#ifndef INICIALIZAR_MEMORIA_H_
#define INICIALIZAR_MEMORIA_H_

#include <stdio.h>
#include <stdlib.h>
#include "variables_globales.h"
#include "tabla_procesos.h"

void init();
void _iniciar_logger();
void _iniciar_config();
void _iniciar_tabla_procesos();
void imprimir_config();

#endif /*INICIALIZAR_MEMORIA_H_*/