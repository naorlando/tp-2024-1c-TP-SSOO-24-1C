#ifndef CONSOLA_H_
#define CONSOLA_H_

#include <kernel.h>
#include <variables_globales.h>

void inicial_consola_interactiva();
void _validacion_de_instrucciones_consola();
void _atender_instruccion();
void _iniciar_proceso();
void _asignar_pid();

#endif