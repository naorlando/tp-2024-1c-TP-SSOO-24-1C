#ifndef MEMORIA_SERVER_H_
#define MEMORIA_SERVER_H_

#include "variables_globales.h"
#include "protocolo_memoria.h"
#include "utils/sockets.h"
#include "utils/buffer.h"
#include "utils/estructuras.h"

void requests_cpu();

void requests_kernel();

void requests_entradasalida();


#endif //MEMORIA_SERVER_H