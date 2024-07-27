#ifndef ENTRADASALIDA_SERVER_H
#define ENTRADASALIDA_SERVER_H

#include "variables_globales.h"
#include "protocolo_entradasalida.h"
#include <commons/string.h>
#include <utils/sockets.h>
#include <utils/estructuras.h>
#include <utils/buffer.h>
#include <commons/log.h>
#include <entradasalida_config.h>
#include <pthread.h>


void inicializar_sockets();
// void crear_hilo_interfaz_generica();
// void crear_hilo_interfaz_stdin();
// void crear_hilo_interfaz_stdout();
// void crear_hilo_interfaz_dialfs();
void crear_hilos_conexiones();


// void* atender_solicitudes_generica(void* args);
// void* atender_solicitudes_stdin(void* args);
// void* atender_solicitudes_stdout(void* args);
// void* atender_solicitudes_dialfs(void* args);
void atender_solicitudes_io_kernel();
void atender_solicitudes_io_memoria();

void cerrar_cliente();
void _cerrar_puertos();
void _cerrar_conexiones();

void enviar_io_interface_kernel();
void enviar_io_interface_memoria();

#endif