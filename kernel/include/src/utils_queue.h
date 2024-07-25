#ifndef UTILS_QUEUE_H_
#define UTILS_QUEUE_H_

#include <pthread.h>
#include "variables_globales.h"
#include "utils/estructuras.h"
#include "kernel_config.h"
#include "cronometro.h"
#include "utils/utils.h"

// ############################################################################################################
// PUSH:
// ############################################################################################################

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

void agregar_a_cola_new(t_PCB*);

// ############################################################################################################
// POP:
// ############################################################################################################

// Obtenemos el siguiente pcb a ejecutar de una determinada cola NEW
// pre: queue != NULL
// post: return pcb
t_PCB* siguiente_pcb_cola_new();

// Obtenemos el siguiente pcb a ejecutar de una determinada cola READY
// pre: queue != NULL
// post: return pcb
t_PCB* get_next_pcb_ready_to_exec();

// obtiene el siguiente pcb a ejecutar de la cola auxiliar de ready
// pre: cola_aux_ready != NULL
// post: return pcb
t_PCB *get_next_pcb_aux_ready_to_exec();

// obtiene el siguiente pcb a ejecutar de la cola de EXIT
// pre: cola_exit != NULL
// post: return pcb
t_PCB *get_next_pcb_exit();

// ############################################################################################################
// Funciones auxiliares:
// ############################################################################################################

// Muestra los elementos de una cola
// pre: queue != NULL and char* de nombre de la cola.
// post: imprime los elementos de la cola
void mostrar_elementos_de_cola(t_list *COLA, char *nombre_cola);

// lista los pids de una cola
// pre: queue != NULL
// post: return lista de pids
t_list* listar_pids_de_queue(t_list *queue);


void execute_to_null();




#endif //UTILS_QUEUE_