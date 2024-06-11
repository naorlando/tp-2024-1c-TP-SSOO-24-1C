#ifndef ENTRADASALIDA_SERVER_H_
#define ENTRADASALIDA_SERVER_H_

#include "variables_globales.h"
#include "protocolo_entrada.h"
#include "commons/string.h"
#include "utils/sockets.h"
#include "utils/estructuras.h"
#include "utils/buffer.h"
#include "utils/sockets.h"
#include <commons/log.h>
#include "entradasalida_config.h"
#include <pthread.h>


void requests_kernel();
void requests_memoria();
void inicializar_sockets();
void crear_hilos_conexiones();
void cerrar_cliente();
void _cerrar_conexiones();
void _cerrar_puertos();

#endif //ENTRADASALIDA_SERVER