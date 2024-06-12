#ifndef CPU_INTERRUPT_H_
#define CPU_INTERRUPT_H_

#include "variables_globales.h"
#include "utils/sockets.h"
#include "utils/estructuras.h"
#include <commons/log.h>
#include "utils/buffer.h"
#include "utils/protocolo.h"
#include "protocolo_cpu.h"
#include "cpu_syncro.h"

void atender_cpu_kernel_interrupt();

#endif