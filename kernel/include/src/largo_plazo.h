#ifndef LARGO_PLAZO_H
#define LARGO_PLAZO_H

// Standard library
#include <stdio.h>
#include <stdlib.h> 
#include <stdbool.h>

// Commons
#include <commons/log.h>
#include <commons/collections/list.h>
#include <commons/string.h>
#include <commons/collections/queue.h>

// Libraries
#include <semaphore.h>
#include <string.h>

//project
#include "utils/estructuras.h"
#include "syncro.h"
#include "utils_queue.h"



void planificador_largo_plazo();
void process_to_new(t_PCB * pcb);
void send_new_to_ready();
void send_to_exit(t_PCB * pcb);
void end_process();
void liberar_recursos_de_proceso(u_int32_t pid);

#endif