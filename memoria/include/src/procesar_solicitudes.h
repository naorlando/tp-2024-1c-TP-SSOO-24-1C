#ifndef PROCESAR_SOLICITUDES_H_
#define PROCESAR_SOLICITUDES_H_

#include <commons/log.h>
#include <utils/sockets.h>
#include "variables_globales.h"

// Agrego DEFINES que despues voy a eliminar
#define MENSAJE 0
#define ERROR -1

void solicitudes_cpu();

void solicitudes_kernel();

void solicitudes_entradasalida();

#endif