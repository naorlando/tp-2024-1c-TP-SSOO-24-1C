#ifndef ENTRADASALIDA_SERVER_H
#define ENTRADASALIDA_SERVER_H

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

void inicializar_sockets();
void crear_hilo_interfaz_generica();
void crear_hilo_interfaz_stdin();
void crear_hilo_interfaz_stdout();
void crear_hilo_interfaz_dialfs();

#endif