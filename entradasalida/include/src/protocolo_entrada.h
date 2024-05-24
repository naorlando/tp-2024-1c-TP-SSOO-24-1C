#ifndef PROTOCOLO_ENTRADASALIDA_H_
#define PROTOCOLO_ENTRADASALIDA_H_

#include "variables_globales.h"
#include "utils/protocolo.h"

// Prototipos de funciones
void requests_kernel();
// void requests_memoria();
// int send_example_kernel();
// int recv_example_msg_kernel();
// int send_example_memoria();
// void atender_io_kernel();
// void atender_io_memoria();
void atender_instruccion_sleep();
int recibir_instruccion(int fd, t_instruction **instruccion);
int enviar_confirmacion(int fd, t_msg_header header);

#endif