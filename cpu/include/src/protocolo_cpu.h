#ifndef PROTOCOLO_CPU_H_
#define PROTOCOLO_CPU_H_

#include "utils/protocolo.h"
#include <variables_globales.h>
#include "utils/estructuras.h"

int send_example_memoria();
void atender_cpu_kernel_dispatch();
void atender_cpu_memoria();
int recv_example_msg_kernel();
int recv_pcb_cpu();

#endif