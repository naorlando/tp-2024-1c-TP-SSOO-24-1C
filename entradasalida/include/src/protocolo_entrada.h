#ifndef PROTOCOLO_ENTRADASALIDA_H_
#define PROTOCOLO_ENTRADASALIDA_H_

#include "utils/protocolo.h"
#include "variables_globales.h"
#include "utils/estructuras.h"
#include "utils/solicitudes_io.h"

int send_example_kernel();
int recv_example_msg_kernel();
int send_example_memoria();

void atender_instruccion_generica(int fd);
int enviar_confirmacion_io(int fd);
int enviar_confirmacion(int fd, t_msg_header header);

#endif