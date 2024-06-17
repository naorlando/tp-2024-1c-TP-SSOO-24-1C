#ifndef PROTOCOLO_MEMORIA_H_
#define PROTOCOLO_MEMORIA_H_

#include <commons/log.h>
#include <utils/sockets.h>
#include "variables_globales.h"
#include "utils/protocolo.h"
#include "utils/buffer.h"
#include "gestion_procesos.h" 

int recv_example_msg_cpu();

int recv_example_msg_kernel();

int recv_example_msg_entradasalida();

t_new_process* recv_new_process_kernel();

t_next_instruction* recv_next_instruction_cpu();

void send_instrution_cpu(t_instruction*);

#endif /*PROTOCOLO_MEMORIA_H_*/