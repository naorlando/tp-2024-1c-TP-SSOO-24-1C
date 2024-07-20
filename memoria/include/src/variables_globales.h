#ifndef VARIABLES_GLOBALES_H_
#define VARIABLES_GLOBALES_H_



#include <commons/log.h>
#include <commons/config.h>
#include <commons/collections/dictionary.h>
#include <commons/collections/list.h>
#include "memoria_config.h"
#include "inicializar_espacio.h"


#define SERVERNAME "MEMORIA"
#define CLIENTE_CPU "CLIENTE CPU"
#define CLIENTE_KERNEL "CLIENTE KERNEL"
#define CLIENTE_ENTRADASALIDA "CLIENTE ENTRADASALIDA"
#define MAX_LINE_LENGTH 256


// VARIABLES GLOBALES
extern t_log* logger_memoria_debug;
extern memory_t * espacio_memoria;
extern t_dictionary* tabla_procesos;
extern t_log* logger_memoria;

//CONFIG
extern t_config* config_memoria;
extern t_memoria_config* memoria_config;


extern t_dictionary *diccionario_tablas_de_paginas_por_proceso;
extern pthread_mutex_t MUTEX_DICCIONARIO_TABLAS_PAGINAS;
extern bool *bitarray_marcos_de_memoria;
extern pthread_mutex_t MUTEX_BIT_ARRAY_MEMORY;
extern pthread_mutex_t MUTEX_MEMORY_SPACE;


//FD
extern int fd_server;
extern int fd_entradasalida;
extern int fd_cpu;
extern int fd_kernel;

//PORTS
extern char* server_port;

#endif /*VARIABLES_GLOBALES_H_*/