#ifndef MEMORIA_SERVER_H_
#define MEMORIA_SERVER_H_

#include "variables_globales.h"
#include "protocolo_memoria.h"
#include "utils/sockets.h"
#include <commons/log.h>
#include <commons/string.h>
#include <pthread.h>

#include "memoria_config.h"
#include "manager_memoria.h"
#include "utils/io_interface.h"
#include "utils/gestion_conexiones_io.h"

void requests_cpu();

void requests_kernel();

void levantar_servidor();

void esperar_clientes();

void crear_hilos_conexiones();

void* esperar_conexiones_IO(void*);

void cerrar_servidor();

void _cerrar_puertos();

void _cerrar_conexiones();

void atender_memoria_IO(void*);


#endif //MEMORIA_SERVER_H