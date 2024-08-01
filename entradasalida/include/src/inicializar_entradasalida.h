#ifndef INICIALIZAR_ENTRADASALIDA_H_
#define INICIALIZAR_ENTRADASALIDA_H_

#include <stdio.h>
#include <stdlib.h>
#include <commons/log.h>
#include <commons/config.h>
#include <commons/string.h>
#include <variables_globales.h>
#include <dialfs.h>
#include "comportamientos_dialfs.h"

void init(char*);

void _iniciar_logger();
void _iniciar_config(char*);
void _imprimir_config();

void _inicializar_dialfs();



#endif /* INICIALIZAR_ENTRADASALIDA_H_ */