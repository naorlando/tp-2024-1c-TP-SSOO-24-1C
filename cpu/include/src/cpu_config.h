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

t_cpu_config* crear_cpu_config();
void destruir(t_cpu_config*);
bool cargar_cpu_config(t_cpu_config*, t_config*);

#endif