#ifndef CONSOLA_H_
#define CONSOLA_H_

#include <kernel.h>
#include <variables_globales.h>

// TODO: mover de lugar:
pthread_mutex_t  mutex_pid = PTHREAD_MUTEX_INITIALIZER;

int identificador_PID = 1;

void iniciar_consola_interactiva();
bool _validacion_de_instrucciones_consola(char *leido);
void _atender_instruccion(char *leido);

// FORMAS DE ATENDER PROCESO:
void f_iniciar_proceso(t_buffer* un_buffer);
int asignar_pid();

#endif //CONSOLA_H_