#ifndef CONSOLA_H_
#define CONSOLA_H_

#include <kernel.h>
#include <variables_globales.h>



void iniciar_consola_interactiva();
bool _validacion_de_instrucciones_consola(char *leido);
void _atender_instruccion(char *leido);

// FORMAS DE ATENDER PROCESO:
void f_iniciar_proceso(t_buffer* un_buffer);
int asignar_pid();

#endif //CONSOLA_H_