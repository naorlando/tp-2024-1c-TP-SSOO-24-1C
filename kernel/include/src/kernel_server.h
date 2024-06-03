#ifndef KERNEL_SERVER_H
#define KERNEL_SERVER_H

#include "variables_globales.h"
#include "protocolo_kernel.h"
#include "utils/estructuras.h"
#include "utils/buffer.h"
#include "utils/sockets.h"

void atender_kernel_memoria();
void atender_kernel_IO();
void atender_kernel_cpu_dispatch();

#endif //KERNEL_SERVER