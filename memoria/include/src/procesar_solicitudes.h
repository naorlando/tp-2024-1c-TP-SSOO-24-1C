#ifndef PROCESAR_SOLICITUDES_H_
#define PROCESAR_SOLICITUDES_H_

#include <commons/log.h>
#include <utils/sockets.h>

// Agrego DEFINES que despues voy a eliminar
#define MENSAJE 0
#define ERROR -1

void solicitudes_cpu(t_log *);

void solicitudes_kernel(int, t_log *);

void solicitudes_entradasalida(int, t_log *);

#endif