#ifndef INICIALIZAR_MEMORIA_H_
#define INICIALIZAR_MEMORIA_H_

#include <stdio.h>
#include <stdlib.h>
#include "variables_globales.h"
#include "tabla_procesos.h"

// Inicializa el módulo de memoria configurando los logs, la configuración y la tabla de procesos.
// Pre: Ninguna.
// Post: Los logs, la configuración y la tabla de procesos están inicializados y listos para usar.
void init();

// Inicializa los loggers para el módulo de memoria.
// Pre: Ninguna.
// Post: Los loggers están creados y listos para escribir mensajes de log.
void _iniciar_logger();

// Carga la configuración del módulo de memoria desde un archivo.
// Pre: El archivo de configuración debe existir y ser accesible.
// Post: La configuración está cargada y almacenada en memoria_config.
void _iniciar_config();

// Crea la tabla de procesos para el módulo de memoria.
// Pre: Ninguna.
// Post: La tabla de procesos está creada y lista para almacenar procesos.
void _iniciar_tabla_procesos();

// Imprime la configuración cargada en el log de debug.
// Pre: La configuración debe estar cargada en memoria_config.
// Post: La configuración se ha registrado en el log de debug.
void imprimir_config();

#endif /*INICIALIZAR_MEMORIA_H_*/