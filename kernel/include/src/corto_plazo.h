#ifndef KERNEL_CORTO_PLAZO_H
#define KERNEL_CORTO_PLAZO_H

// Standard library
#include <stdio.h>
#include <stdlib.h> 
#include <stdbool.h>

// Commons
#include <commons/log.h>
#include <commons/collections/list.h>
#include <commons/string.h>

// Libraries
#include <semaphore.h>
#include <string.h>

// Project
#include "utils/estructuras.h"
#include "syncro.h"
#include "protocolo_kernel.h"

typedef enum {
    FIFO,
    RR,
    VRR
    
}t_planificador;


t_PCB* get_next_pcb_to_exec();
void planificador_corto_plazo ();
t_planificador _obtener_planificador (char * str);

#endif