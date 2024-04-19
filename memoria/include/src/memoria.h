#ifndef MEMORIA_H_
#define MEMORIA_H_

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <commons/log.h>
#include <commons/config.h>
#include <commons/string.h>
#include <utils/sockets.h>
#include "inicializar_memoria.h"
#include "procesar_solicitudes.h"

#define SERVERNAME "MEMORIA"

int fd_server;
int fd_entradasalida;

#endif /*MEMORIA_H*/