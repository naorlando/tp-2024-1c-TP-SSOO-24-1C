#ifndef PROTOCOLO_KERNEL_H_
#define PROTOCOLO_KERNEL_H_

#include "utils/protocolo.h"
#include <variables_globales.h>

typedef struct {
    int pid;
    unsigned int program_counter;
    int estado;  // Ejemplo: 0=NEW, 1=READY, 2=RUNNING, etc.
    unsigned int registros[4];  // Simulando registros AX, BX, CX, DX
} t_PCB;

int send_example_cpu();
int send_example_memoria();
// void requests_entradasalida();
int recv_example_msg_entradasalida();

void atender_kernel_memoria();
void atender_kernel_IO();
void atender_kernel_cpu_dispatch();

#endif
