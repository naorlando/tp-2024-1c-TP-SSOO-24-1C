#ifndef KERNEL_SERVER_H
#define KERNEL_SERVER_H

#include "variables_globales.h"
#include "protocolo_kernel.h"
#include "commons/string.h"
#include "utils/sockets.h"
//#include "utils/estructuras.h"
//#include "utils/buffer.h"
#include <commons/collections/dictionary.h>
#include <commons/log.h>
#include "kernel_config.h"
#include <pthread.h>
//#include "datos_hilos.h"
#include "manager_dispatch.h"
#include "manager_ios.h"

void levantar_servidor();
void inicializar_sockets();
void crear_hilos_conexiones();
void cerrar_servidor();
void* esperar_conexiones_IO(void* arg);
//void cancelar_hilo_quantum(uint32_t);

// Funciones de manejo de conexiones memoria
void atender_kernel_memoria();

// Funciones de manejo de conexiones CPU
void atender_kernel_cpu_dispatch();

// Funciones de manejo de conexiones I/O
t_IO_connection* recibir_io_connection(int cliente_io);
void agregar_IO_connection(t_IO_connection* io_connection);
t_IO_connection* get_IO_connection(char* nombre_interfaz);
void liberar_IO_connection(t_IO_connection* io_connection);

// Funciones auxiliares
int obtener_file_descriptor(t_IO_connection* conexion);
char* obtener_nombre_conexion(t_IO_connection* conexion);
tipo_interfaz_t obtener_tipo_conexion(t_IO_connection* conexion);

// Funciones de cierre
void _cerrar_conexiones();
void _cerrar_puertos();

#endif //KERNEL_SERVER