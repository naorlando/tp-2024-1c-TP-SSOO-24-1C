#ifndef CPU_H
#define CPU_H
#define SERVERNAME "CPU"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <commons/string.h>
#include <commons/config.h>
#include <commons/log.h>

#include "utils/sockets.h"


#define SERVERNAME "CPU"

/* incluir .h files del cpu*/

#include "cpu_config.h"
#include "inicializar_cpu.h"
#include "cpu_dispatch.h"



#endif