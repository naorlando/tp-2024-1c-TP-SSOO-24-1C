#ifndef PROTOCOLO_ENTRADASALIDA_H_
#define PROTOCOLO_ENTRADASALIDA_H_

#include "utils/protocolo.h"
#include "variables_globales.h"
#include "utils/estructuras.h"

int send_example_kernel();
int recv_example_msg_kernel();
int send_example_memoria();

void atender_instruccion_sleep();
// void atender_io_kernel();
// void atender_io_memoria();

#endif