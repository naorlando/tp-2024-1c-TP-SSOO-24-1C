#ifndef KERNEL_DISPATCH_H_
#define KERNEL_DISPATCH_H_

#include <commons/log.h>
#include <commons/string.h>
#include "utils/buffer.h"
#include "utils/protocolo.h"
#include "kernel.h"

extern int fd_server_dispatch;
extern int fd_cpu_dispatch;


void atender_kernel_cpu_dispatch();



#endif