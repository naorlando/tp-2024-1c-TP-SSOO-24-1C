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
#include <commons/temporal.h>

// Libraries
#include <semaphore.h>
#include <string.h>

// Project
#include "utils/estructuras.h"
#include "syncro.h"
#include "protocolo_kernel.h"
#include "datos_hilos.h"
#include "utils/interruption.h"
#include "utils_queue.h"
#include "solicitud.h"
#include "cronometro.h"

typedef enum {
    FIFO,
    RR,
    VRR
}t_planificador;

typedef struct {
    uint32_t pid;
    uint32_t quantum;
} hilo_args;

void planificador_corto_plazo();
// TODO: Eliminar luego de probar la nueva implementacion de manager_ios
//void blocked();
t_planificador _obtener_planificador (char * str);
void planificador_FIFO();
void planificador_RR();
void planificador_VRR();
void interrupcion_quantum(uint32_t pid,uint32_t quantum);
void* funcion_hilo_quantum(void*);

// envia una interrupcion a la cpu
void enviar_interrupcion_a_cpu(uint32_t); 

// Seteamos EXECUTE = pcb y mandamos pcb
// pre: pcb != NULL
// post: pcb->estado = EXEC and send_pcb_cpu(pcb)
void pcb_execute(t_PCB*);
#endif