#ifndef PROTOCOLO_CPU_H_
#define PROTOCOLO_CPU_H_

#include "utils/protocolo.h"
#include <variables_globales.h>
#include "utils/estructuras.h"
#include "utils/instruccion.h"

int send_example_memoria();
int recv_example_msg_kernel();
int recv_pcb_cpu();

// Instrucciones:
void atender_instruccion_memoria();

#endif