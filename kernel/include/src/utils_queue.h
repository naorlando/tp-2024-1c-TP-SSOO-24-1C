#ifndef UTILS_QUEUE_H_
#define UTILS_QUEUE_H_

#include <pthread.h>
#include "variables_globales.h"
#include "utils/estructuras.h"
#include "kernel_config.h"
#include "cronometro.h"

void agregar_de_new_a_ready(t_PCB*);
void agregar_a_cola_ready(t_PCB*);
void agregar_de_blocked_a_ready(t_PCB*);
void agregar_a_cola_exit(t_PCB*);
void agregar_a_cola_aux_ready(t_PCB* pcb);

// funcion mas general que agrega de execute a ready dependiendo del algoritmo de planificacion
// pre: pcb != NULL
// post: agrega el pcb a la cola de ready
void agregar_de_execute_a_ready(t_PCB* pcb);

// logica de planificacion VRR en el envio de colas
// pre: pcb != NULL
// post: agrega el pcb a la cola de ready que corresponde
void agregar_a_cola_ready_VRR(t_PCB* pcb);

#endif //UTILS_QUEUE_