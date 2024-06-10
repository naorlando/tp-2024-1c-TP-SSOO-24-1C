#ifndef PROTOCOLO_KERNEL_H_
#define PROTOCOLO_KERNEL_H_

#include "utils/protocolo.h"
#include "utils/estructuras.h"
#include <variables_globales.h>
#include <stdlib.h>
#include <string.h>

int send_example_cpu();
int send_example_memoria();
void send_pcb_cpu(t_PCB*);
// void requests_entradasalida();
int recv_example_msg_entradasalida(int);

#endif
