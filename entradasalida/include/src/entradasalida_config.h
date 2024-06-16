#ifndef ENTRADASALIDA_CONFIG_H_
#define ENTRADASALIDA_CONFIG_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <commons/config.h>
#include <stdint.h>

typedef struct {
    char* TIPO_INTERFAZ;              // Tipo de Interfaz de I/O
    uint32_t TIEMPO_UNIDAD_TRABAJO;   // Tiempo en milisegundos por unidad de trabajo
    char* IP_KERNEL;                  // IP para conectar con Kernel
    uint16_t PUERTO_KERNEL;           // Puerto para conectar con Kernel
    char* IP_MEMORIA;                 // IP para conectar con Memoria
    uint16_t PUERTO_MEMORIA;          // Puerto para conectar con Memoria
    char* PATH_BASE_DIALFS;           // Path base para archivos DialFS
    uint32_t BLOCK_SIZE;              // Tamaño de los bloques del FS
    uint32_t BLOCK_COUNT;             // Cantidad de bloques del FS
    uint32_t RETRASO_COMPACTACION;    // Tiempo de retraso para compactación
} t_IO_config;

// Crea una nueva estructura t_IO_config.
// Pre: Ninguna.
// Post: Retorna un puntero a una estructura t_IO_config.
//       Si no se puede asignar memoria, retorna NULL.
t_IO_config* crear_IO_config();

// Destruye una estructura t_IO_config, liberando su memoria.
// Pre: El puntero a t_IO_config debe ser válido y no NULL.
// Post: La memoria asignada a la estructura t_IO_config es liberada.
void destruir_IO_config(t_IO_config*);

// Carga la configuración de I/O desde una estructura t_config a una estructura t_IO_config.
// Pre: Los punteros a t_IO_config y t_config deben ser válidos y no NULL.
// Post: La estructura t_IO_config es llenada con los valores de configuración de t_config.
//       Retorna true si la configuración fue cargada correctamente.
bool cargar_IO_config(t_IO_config*, t_config*);

// Verifica si la configuración de Kernel está presente
// Pre: El puntero a t_IO_config debe ser válido y no NULL.
// Post: Retorna true si la configuración de Kernel está presente.
bool tiene_configuracion_kernel(t_IO_config* config);

// Verifica si la configuración de Memoria está presente
// Pre: El puntero a t_IO_config debe ser válido y no NULL.
// Post: Retorna true si la configuración de Memoria está presente.
bool tiene_configuracion_memoria(t_IO_config*);

// Obtiene el tipo de interfaz de una estructura t_IO_config.
// Pre: El puntero a t_IO_config debe ser válido y no NULL.
// Post: Retorna un puntero a la cadena de texto que contiene el tipo de interfaz.
char* obtener_tipo_interfaz(t_IO_config*);

// Obtiene el tiempo de unidad de trabajo de una estructura t_IO_config.
// Pre: El puntero a t_IO_config debe ser válido y no NULL.
// Post: Retorna el tiempo de unidad de trabajo en milisegundos.
uint32_t obtener_tiempo_unidad_trabajo(t_IO_config*);

// Obtiene la IP del kernel de una estructura t_IO_config.
// Pre: El puntero a t_IO_config debe ser válido y no NULL.
// Post: Retorna un puntero a la cadena de texto que contiene la IP del kernel.
char* obtener_ip_kernel(t_IO_config*);

// Obtiene el puerto del kernel de una estructura t_IO_config.
// Pre: El puntero a t_IO_config debe ser válido y no NULL.
// Post: Retorna el puerto del kernel.
uint16_t obtener_puerto_kernel(t_IO_config*);

// Obtiene la IP de la memoria de una estructura t_IO_config.
// Pre: El puntero a t_IO_config debe ser válido y no NULL.
// Post: Retorna un puntero a la cadena de texto que contiene la IP de la memoria.
char* obtener_ip_memoria(t_IO_config*);

// Obtiene el puerto de la memoria de una estructura t_IO_config.
// Pre: El puntero a t_IO_config debe ser válido y no NULL.
// Post: Retorna el puerto de la memoria.
uint16_t obtener_puerto_memoria(t_IO_config*);

// Obtiene el path base de DialFS de una estructura t_IO_config.
// Pre: El puntero a t_IO_config debe ser válido y no NULL.
// Post: Retorna un puntero a la cadena de texto que contiene el path base de DialFS.
char* obtener_path_base_dialfs(t_IO_config*);

// Obtiene el tamaño de los bloques del sistema de archivos de una estructura t_IO_config.
// Pre: El puntero a t_IO_config debe ser válido y no NULL.
// Post: Retorna el tamaño de los bloques del sistema de archivos.
uint32_t obtener_block_size(t_IO_config*);

// Obtiene la cantidad de bloques del sistema de archivos de una estructura t_IO_config.
// Pre: El puntero a t_IO_config debe ser válido y no NULL.
// Post: Retorna la cantidad de bloques del sistema de archivos.
uint32_t obtener_block_count(t_IO_config*);

// Obtiene el tiempo de retraso de compactación de una estructura t_IO_config.
// Pre: El puntero a t_IO_config debe ser válido y no NULL.
// Post: Retorna el tiempo de retraso de compactación en milisegundos.
uint32_t obtener_retraso_compactacion(t_IO_config*);

#endif /* ENTRADASALIDA_CONFIG_H_ */