#ifndef UTILS_QUEUE_H_
#define UTILS_QUEUE_H_

#include <pthread.h>
#include "variables_globales.h"
#include "utils/estructuras.h"
#include "kernel_config.h"

void agregar_de_new_a_ready(t_PCB*);
void agregar_a_cola_ready(t_PCB*);
void agregar_de_blocked_a_ready(t_PCB*);
void agregar_a_cola_exit(t_PCB*);
void agregar_a_cola_aux_ready(t_PCB* pcb) ;

#endif //UTILS_QUEUE_