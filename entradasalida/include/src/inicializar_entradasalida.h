#ifndef INICIALIZAR_ENTRADASALIDA_H_
#define INICIALIZAR_ENTRADASALIDA_H_

#include <gestor_entradasalida.h>

#include <stdio.h>
#include <stdlib.h>
#include <commons/log.h>
#include <commons/config.h>
#include <commons/string.h>


void init();

void _iniciar_logger();
void _iniciar_config();
void _imprimir_config();



#endif /* INICIALIZAR_ENTRADASALIDA_H_ */