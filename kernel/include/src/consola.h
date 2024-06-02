#ifndef CONSOLA_H_
#define CONSOLA_H_

#include <variables_globales.h>
#include "utils/buffer.h"
#include "utils/estructuras.h"
#include "utils/protocolo.h"
#include "utils/utils.h"
#include "table_pcb.h"



void iniciar_consola_interactiva();
bool _validacion_de_instrucciones_consola(char*);
void _atender_instruccion(void*);

// FORMAS DE ATENDER PROCESO:
void f_iniciar_proceso(char*);
void f_ejecutar_script(const char*);

int asignar_pid();

#endif //CONSOLA_H_