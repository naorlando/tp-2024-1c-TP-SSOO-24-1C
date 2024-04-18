#ifndef PROCESAR_SOLICITUDES_H_
#define PROCESAR_SOLICITUDES_H_

#include <commons/log.h>
#include <utils/sockets.h>

void solicitudes_cpu(int, t_log*);

void solicitudes_kernel(int, t_log*);

void solicitudes_entradasalida(int, t_log*);

#endif