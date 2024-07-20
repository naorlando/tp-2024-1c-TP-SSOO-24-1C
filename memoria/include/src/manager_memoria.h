#ifndef MANAGER_MEMORIA_H_
#define MANAGER_MEMORIA_H_

#include <commons/log.h>
#include <commons/string.h>
#include "variables_globales.h"
#include "protocolo_memoria.h"
#include "utils/estructuras.h"
#include "utils/solicitudes_io.h"
#include "memoria_config.h"
#include "tabla_paginas.h"
#include "memoria_espacio.h"


void retornar_siguiente_instruccion();

int process_message_cpu_page();

int process_message_cpu_data_read();

int process_message_cpu_data_write();

int process_message_cpu_resize();

int process_message_cpu_handshake();

void crear_imagen_proceso();


#endif //MANAGER_MEMORIA_H_