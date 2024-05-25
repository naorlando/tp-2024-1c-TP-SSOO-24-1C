#ifndef CONSOLA_H_
#define CONSOLA_H_

#include <variables_globales.h>
#include "utils/buffer.h"
#include "utils/estructuras.h"
#include "utils/protocolo.h"
#include "table_pcb.h"



void iniciar_consola_interactiva();
bool _validacion_de_instrucciones_consola(char *leido);
void _atender_instruccion(char *leido);

// FORMAS DE ATENDER PROCESO:
void * f_iniciar_proceso(char*);
int asignar_pid();

#endif //CONSOLA_H_