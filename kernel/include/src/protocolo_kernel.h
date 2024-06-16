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
int recv_example_msg_entradasalida();
int enviar_io_gen_sleep(int fd, int pid, int unidades_trabajo);
int recibir_confirmacion_io(int fd_kernel);
int recv_example_msg_entradasalida(int);
t_PCB* recv_pcb_interrupt();
void send_interruption_cpu(t_interruption*);

#endif
