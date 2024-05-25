#ifndef PROTOCOLO_ENTRADASALIDA_H_
#define PROTOCOLO_ENTRADASALIDA_H_



#include "variables_globales.h"
#include "utils/protocolo.h"

void requests_kernel();
void requests_memoria();
int send_example_kernel();
int recv_example_msg_kernel();
int send_example_memoria();


void atender_instruccion_sleep();
// void atender_io_kernel();
// void atender_io_memoria();

#endif