#ifndef CPU_DISPATCH_H_
#define CPU_DISPATCH_H_

#include <commons/log.h>
#include <commons/string.h>
#include "utils/buffer.h"
#include "utils/protocolo.h"
#include "cpu.h"

extern int fd_server_dispatch;
extern int fd_kernel_dispatch;


void atender_cpu_kernel_dispatch();




#endif