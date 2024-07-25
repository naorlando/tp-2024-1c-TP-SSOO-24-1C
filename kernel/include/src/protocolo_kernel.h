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
t_PCB* recv_pcb_cpu();
// void requests_entradasalida();
int recv_example_msg_entradasalida();
int send_kernel_io_gen_sleep(int, char*, t_io_generica*);
int send_kernel_io_stdin(int, char*, t_io_stdin*);
int send_kernel_io_stdout(int, char*, t_io_stdout*);
int recv_example_msg_entradasalida(int);
t_PCB* recv_pcb_interrupt();
void send_interruption_cpu(t_interruption*);
t_solicitud_io_generica* recv_solicitud_io_generica_cpu();
t_manejo_recurso*  recv_wait_or_signal_request();
t_solicitud_io_stdin* recv_solicitud_io_stdin_cpu();
void send_msg_kernel_memoria_end_process(uint32_t);
#endif
