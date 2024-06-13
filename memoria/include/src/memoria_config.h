#ifndef MEMORIA_CONFIG_H_
#define MEMORIA_CONFIG_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <commons/config.h>
#include <stdint.h>

typedef struct {
    // Puerto
    uint16_t PUERTO_ESCUCHA;

    // MEMORIA
    uint16_t TAM_MEMORIA;
    uint16_t TAM_PAGINA;
    char* PATH_INSTRUCCIONES;
    uint16_t RETARDO_RESPUESTA;
} t_memoria_config;

// Crea una nueva estructura t_memoria_config.
// Pre: Ninguna.
// Post: Retorna un puntero a una estructura t_memoria_config.
//       Si no se puede asignar memoria, retorna NULL.
t_memoria_config* crear_memoria_config();

// Destruye una estructura t_memoria_config, liberando su memoria.
// Pre: El puntero a t_memoria_config debe ser válido y no NULL.
// Post: La memoria asignada a la estructura t_memoria_config es liberada.
void destruir(t_memoria_config*);

// Carga la configuración de memoria desde una estructura t_config a una estructura t_memoria_config.
// Pre: Los punteros a t_memoria_config y t_config deben ser válidos y no NULL.
// Post: La estructura t_memoria_config es llenada con los valores de configuración de t_config.
//       Retorna true si la configuración fue cargada correctamente.
bool cargar_memoria_config(t_memoria_config*, t_config*);

// Obtiene el path de instrucciones de un t_memoria_config.
// Pre: El puntero a t_memoria_config debe ser válido y no NULL.
// Post: Retorna un puntero a la cadena de texto que contiene el path de las instrucciones.
char* obtener_path_instrucciones(t_memoria_config*);

// Obtiene el tamaño de la memoria de un t_memoria_config.
// Pre: El puntero a t_memoria_config debe ser válido y no NULL.
// Post: Retorna el tamaño de la memoria.
uint16_t obtener_tam_memoria(t_memoria_config*);

// Obtiene el tamaño de la página de un t_memoria_config.
// Pre: El puntero a t_memoria_config debe ser válido y no NULL.
// Post: Retorna el tamaño de la página.
uint16_t obtener_tam_pagina(t_memoria_config*);

// Obtiene el retardo de respuesta de un t_memoria_config.
// Pre: El puntero a t_memoria_config debe ser válido y no NULL.
// Post: Retorna el retardo de respuesta.
uint16_t obtener_retardo_respuesta(t_memoria_config*);

// Obtiene el puerto de escucha de un t_memoria_config.
// Pre: El puntero a t_memoria_config debe ser válido y no NULL.
// Post: Retorna el puerto de escucha.
uint16_t obtener_puerto_escucha(t_memoria_config*);

#endif /*MEMORIA_CONFIG_H_*/