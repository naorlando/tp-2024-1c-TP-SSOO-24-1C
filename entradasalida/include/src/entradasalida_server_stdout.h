#ifndef ENTRADASALIDA_SERVER_STDOUT_H
#define ENTRADASALIDA_SERVER_STDOUT_H

#include "variables_globales.h"
#include "protocolo_entrada.h"
#include <commons/string.h>
#include <utils/sockets.h>
#include <utils/estructuras.h>
#include <utils/buffer.h>
#include <utils/sockets.h>
#include <commons/log.h>
#include <entradasalida_config.h>
#include <pthread.h>

void crear_hilos_conexiones_stdout();
void *atender_solicitudes_stdout(void *args);
void inicializar_sockets_stdout();

#endif