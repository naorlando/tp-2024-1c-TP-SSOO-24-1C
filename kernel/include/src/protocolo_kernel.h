#ifndef PROTOCOLO_KERNEL_H_
#define PROTOCOLO_KERNEL_H_

#include "utils/protocolo.h"
#include <variables_globales.h>

int send_example_cpu();
int send_example_memoria();
void requests_entradasalida();
int recv_example_msg_entradasalida();

void atender_kernel_memoria();
void atender_kernel_IO();
void atender_kernel_cpu_dispatch();

#endif
