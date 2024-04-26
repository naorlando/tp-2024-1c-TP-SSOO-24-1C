#ifndef PROTOCOLO_MEMORIA_H_
#define PROTOCOLO_MEMORIA_H_

#include <commons/log.h>
#include <utils/sockets.h>
#include "variables_globales.h"
#include "utils/protocolo.h"

void requests_cpu();

void requests_kernel();

void requests_entradasalida();

int recv_example_msg_cpu();

int recv_example_msg_kernel();

int recv_example_msg_entradasalida();

void atender_memoria_cpu();

void atender_memoria_kernel();

void atender_memoria_io();

#endif /*PROTOCOLO_MEMORIA_H_*/