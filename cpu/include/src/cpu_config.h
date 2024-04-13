#ifndef CPU_CONFIG_H
#define CPU_CONFIG_H

#include <stdint.h>
#include <commons/log.h>


typedef struct {
    // IPs y puertos
    uint16_t PUERTO_ESCUCHA_DISPATCH;
    char* IP_MEMORIA;
    uint16_t PUERTO_MEMORIA;
    uint16_t PUERTO_ESCUCHA_INTERRUPT;
    uint16_t CANTIDAD_ENTRADAS_TLB;
    char* ALGORITMO_TLB;
    
} t_config_cpu;


#endif