#ifndef PROTOCOLO_MEMORIA_H_
#define PROTOCOLO_MEMORIA_H_

#include <commons/log.h>
#include <utils/sockets.h>
#include "variables_globales.h"
#include "utils/protocolo.h"

void solicitudes_cpu();

void solicitudes_kernel();

void solicitudes_entradasalida();

int recv_example_msg_cpu();

int recv_example_msg_kernel();

int recv_example_msg_entradasalida();

#endif /*PROTOCOLO_MEMORIA_H_*/