#ifndef MEMORIA_SERVER_H_
#define MEMORIA_SERVER_H_

#include "variables_globales.h"
#include "protocolo_memoria.h"
#include "utils/sockets.h"
#include "utils/buffer.h"
#include "utils/estructuras.h"
#include "memoria_config.h"
#include "protocolo_memoria.h"
#include <commons/log.h>
#include <commons/string.h>
#include <pthread.h>

void requests_cpu();

void requests_kernel();

void requests_entradasalida(void*);

void levantar_servidor();

void esperar_clientes();

int process_message_cpu_data_read(t_buffer *buffer);

int process_message_cpu_data_write(t_buffer *buffer);

int process_message_cpu_resize(t_buffer *buffer);

void crear_hilos_conexiones();

void* esperar_conexiones_IO(void*);

void cerrar_servidor();

void _cerrar_puertos();

void _cerrar_conexiones();


#endif //MEMORIA_SERVER_H