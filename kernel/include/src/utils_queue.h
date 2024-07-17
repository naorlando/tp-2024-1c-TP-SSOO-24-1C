#ifndef UTILS_QUEUE_H_
#define UTILS_QUEUE_H_

#include <pthread.h>
#include "variables_globales.h"
#include "utils/estructuras.h"
#include "kernel_config.h"

void agregar_de_new_a_ready(t_PCB*);
void agregar_a_cola_ready(t_PCB*);
void agregar_a_ready_fin_quantum(t_PCB*);
void agregar_de_blocked_a_ready(t_PCB*);
void agregar_a_cola_exit(t_PCB*);
void agregar_a_cola_new(t_PCB*);
t_PCB* siguiente_pcb_cola_new();
t_PCB* siguiente_pcb_cola_ready();

#endif //UTILS_QUEUE_