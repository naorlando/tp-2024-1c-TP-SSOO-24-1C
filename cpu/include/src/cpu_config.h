#ifndef CPU_CONFIG_H
#define CPU_CONFIG_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <commons/string.h>
#include <commons/config.h>
#include <string.h>

typedef struct
{
    // IPs y puertos
    uint16_t PUERTO_ESCUCHA_DISPATCH;
    char *IP_MEMORIA;
    uint16_t PUERTO_MEMORIA;
    uint16_t PUERTO_ESCUCHA_INTERRUPT;
    uint16_t CANTIDAD_ENTRADAS_TLB;
    char *ALGORITMO_TLB;

} t_cpu_config;

// Crea y devuelve una nueva estructura de configuración del CPU.
// Pre: Ninguna.
// Post: Devuelve un puntero a una estructura t_cpu_config inicializada, o NULL en caso de error.
t_cpu_config* crear_cpu_config();

// Libera la memoria asociada a una estructura de configuración del CPU.
// Pre: El puntero cpu_config debe apuntar a una estructura t_cpu_config válida y no debe ser NULL.
// Post: La memoria asociada a cpu_config es liberada.
void destruir(t_cpu_config*);

// Carga la configuración del CPU desde un archivo de configuración.
// Pre: El puntero cpu_config debe apuntar a una estructura t_cpu_config válida y no debe ser NULL.
//      El puntero cf debe apuntar a una estructura t_config válida y no debe ser NULL.
// Post: La configuración del CPU es cargada desde el archivo de configuración y almacenada en cpu_config.
//       Devuelve true si la configuración se cargó correctamente, false en caso contrario.
bool cargar_cpu_config(t_cpu_config*, t_config*);

// Devuelve el algoritmo TLB configurado.
// Pre: El puntero cpu_config debe apuntar a una estructura t_cpu_config válida y no debe ser NULL.
// Post: Devuelve una cadena con el algoritmo TLB configurado.
char* obtener_algoritmo_tlb(t_cpu_config*);

// Devuelve la cantidad de entradas TLB configuradas.
// Pre: El puntero cpu_config debe apuntar a una estructura t_cpu_config válida y no debe ser NULL.
// Post: Devuelve la cantidad de entradas TLB configuradas.
uint16_t obtener_cantidad_entradas_tlb(t_cpu_config*);

// Devuelve el puerto de escucha de interrupciones configurado.
// Pre: El puntero cpu_config debe apuntar a una estructura t_cpu_config válida y no debe ser NULL.
// Post: Devuelve el puerto de escucha de interrupciones configurado.
uint16_t obtener_puerto_escucha_interrupt(t_cpu_config*);

// Devuelve el puerto de la memoria configurado.
// Pre: El puntero cpu_config debe apuntar a una estructura t_cpu_config válida y no debe ser NULL.
// Post: Devuelve el puerto de la memoria configurado.
uint16_t obtener_puerto_memoria(t_cpu_config*);

// Devuelve la IP de la memoria configurada.
// Pre: El puntero cpu_config debe apuntar a una estructura t_cpu_config válida y no debe ser NULL.
// Post: Devuelve una cadena con la IP de la memoria configurada.
char* obtener_ip_memoria(t_cpu_config*);

// Devuelve el puerto de escucha de dispatch configurado.
// Pre: El puntero cpu_config debe apuntar a una estructura t_cpu_config válida y no debe ser NULL.
// Post: Devuelve el puerto de escucha de dispatch configurado.
uint16_t obtener_puerto_escucha_dispatch(t_cpu_config*);

#endif