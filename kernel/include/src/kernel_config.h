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

t_kernel_config *crear_kernel_config();
void destruir(t_kernel_config *);
bool cargar_kernel_config(t_kernel_config *, t_config *);

#endif