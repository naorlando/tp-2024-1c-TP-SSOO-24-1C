#ifndef KERNEL_CONFIG_H_
#define KERNEL_CONFIG_H_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <utils/utils.h>
#include <commons/string.h>
#include <commons/config.h>

typedef struct
{
    // IPs y puertos
    uint16_t PUERTO_ESCUCHA;
    char *IP_MEMORIA;
    uint16_t PUERTO_MEMORIA;
    char *IP_CPU;
    uint16_t PUERTO_CPU_DISPATCH;
    uint16_t PUERTO_CPU_INTERRUPT;

    // KERNEL
    char *ALGORITMO_PLANIFICACION;
    uint16_t QUANTUM;
    char **RECURSOS;
    uint32_t *INSTANCIAS_RECURSOS;
    uint16_t GRADO_MULTIPROGRAMACION;

} t_kernel_config;

// Crea una nueva configuración de kernel.
// Pre: No hay condiciones previas.
// Post: Se retorna un puntero a t_kernel_config inicializado o NULL si falla la asignación de memoria.
t_kernel_config *crear_kernel_config();

// Destruye la configuración del kernel liberando la memoria utilizada.
// Pre: El puntero kc debe ser un puntero válido a t_kernel_config.
// Post: La memoria asociada a kc es liberada.
void destruir(t_kernel_config *);

// Carga la configuración del kernel desde un archivo de configuración.
// Pre: El puntero kc debe ser un puntero válido a t_kernel_config.
//      El puntero cf debe ser un puntero válido a t_config inicializado con un archivo de configuración válido.
// Post: Se retornará true si la carga fue exitosa, false en caso contrario.
bool cargar_kernel_config(t_kernel_config *, t_config *);

// Obtiene el puerto de escucha del kernel.
// Pre: El puntero kc debe ser un puntero válido a t_kernel_config.
// Post: Se retorna el puerto de escucha.
uint16_t obtener_puerto_escucha(t_kernel_config*);

// Obtiene la IP de la memoria.
// Pre: El puntero kc debe ser un puntero válido a t_kernel_config.
// Post: Se retorna un puntero a una cadena con la IP de la memoria.
char* obtener_ip_memoria(t_kernel_config*);

// Obtiene el puerto de la memoria.
// Pre: El puntero kc debe ser un puntero válido a t_kernel_config.
// Post: Se retorna el puerto de la memoria.
uint16_t obtener_puerto_memoria(t_kernel_config*);

// Obtiene la IP del CPU.
// Pre: El puntero kc debe ser un puntero válido a t_kernel_config.
// Post: Se retorna un puntero a una cadena con la IP del CPU.
char* obtener_ip_cpu(t_kernel_config*);

// Obtiene el puerto de dispatch del CPU.
// Pre: El puntero kc debe ser un puntero válido a t_kernel_config.
// Post: Se retorna el puerto de dispatch del CPU.
uint16_t obtener_puerto_cpu_dispatch(t_kernel_config*);

// Obtiene el puerto de interrupt del CPU.
// Pre: El puntero kc debe ser un puntero válido a t_kernel_config.
// Post: Se retorna el puerto de interrupt del CPU.
uint16_t obtener_puerto_cpu_interrupt(t_kernel_config*);

// Obtiene el algoritmo de planificación del kernel.
// Pre: El puntero kc debe ser un puntero válido a t_kernel_config.
// Post: Se retorna un puntero a una cadena con el algoritmo de planificación.
char* obtener_algoritmo_planificacion(t_kernel_config*);

// Obtiene el quantum del kernel.
// Pre: El puntero kc debe ser un puntero válido a t_kernel_config.
// Post: Se retorna el valor del quantum.
uint16_t obtener_quantum(t_kernel_config*);

// Obtiene los recursos del kernel.
// Pre: El puntero kc debe ser un puntero válido a t_kernel_config.
// Post: Se retorna un puntero a un array de cadenas con los recursos.
char** obtener_recursos(t_kernel_config*);

// Obtiene las instancias de los recursos del kernel.
// Pre: El puntero kc debe ser un puntero válido a t_kernel_config.
// Post: Se retorna un puntero a un array de enteros con las instancias de los recursos.
uint32_t* obtener_instancias_recursos(t_kernel_config*);

// Obtiene el grado de multiprogramación del kernel.
// Pre: El puntero kc debe ser un puntero válido a t_kernel_config.
// Post: Se retorna el grado de multiprogramación.
uint16_t obtener_grado_multiprogramacion(t_kernel_config*);

#endif