#ifndef PROTOCOLO_ENTRADASALIDA_H_
#define PROTOCOLO_ENTRADASALIDA_H_

#include <commons/log.h>
#include <utils/sockets.h>
#include "variables_globales.h"
#include "utils/protocolo.h"

void requests_kernel();

int send_example_kernel();
int recv_example_msg_kernel();
int send_example_memoria();

#endif