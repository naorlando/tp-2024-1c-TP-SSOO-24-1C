#ifndef CPU_INTERRUPT_H_
#define CPU_INTERRUPT_H_

#include <commons/log.h>
#include "utils/buffer.h"
#include "utils/protocolo.h"

extern int fd_server_interrupt;

extern int fd_kernel_interrupt;
void atender_cpu_kernel_interrupt(t_log* logger);



#endif