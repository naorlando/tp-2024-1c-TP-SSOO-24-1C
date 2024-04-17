#ifndef ENTRADASALIDA_H_
#define ENTRADASALIDA_H_

#include<gestor_entradasalida.h>

#include <stdio.h>
#include <stdlib.h>
#include <commons/log.h>
#include <commons/config.h>
#include <commons/string.h>

#include<inicializar_entradasalida.h>
#include<utils/sockets.h>


/*
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 VARIABLES GLOBALES: ir agregando al gesto_entradasalida.h
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
*/
int conexion_io_kernel;
int fd_entradasalida;
int fd_memoria;
int fd_kernel;

t_log *logger_entradasalida;
t_log *logger_entradasalida_debug;
t_config *config_entradasalida;
// t_entradasalida_config* entradasalida_config;

//Datos de la CONFIG:
char* TIPO_INTERFAZ;
int TIEMPO_UNIDAD_TRABAJO;
char* IP_KERNEL;
char* PUERTO_KERNEL;
char* IP_MEMORIA;
char* PUERTO_MEMORIA;
char* PATH_BASE_DIALFS;
int BLOCK_SIZE;
int BLOCK_COUNT;




#endif /*ENTRADASALIDA_H_*/