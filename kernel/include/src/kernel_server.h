#ifndef KERNEL_SERVER_H
#define KERNEL_SERVER_H

#include "variables_globales.h"
#include "protocolo_kernel.h"
#include "commons/string.h"
#include "utils/sockets.h"
//#include "utils/estructuras.h"
//#include "utils/buffer.h"
#include <commons/log.h>
#include "kernel_config.h"
#include <pthread.h>
//#include "datos_hilos.h"
#include "manager_dispatch.h"
#include "manager_ios.h"

void atender_kernel_memoria();
void atender_kernel_IO(void*);
void atender_kernel_cpu_dispatch();
void levantar_servidor();
void inicializar_sockets();
void crear_hilos_conexiones();
void cerrar_servidor();
void* esperar_conexiones_IO(void*);
void _cerrar_conexiones();
void _cerrar_puertos();
char* recibir_io_connection(int);
//void cancelar_hilo_quantum(uint32_t);

#endif //KERNEL_SERVER