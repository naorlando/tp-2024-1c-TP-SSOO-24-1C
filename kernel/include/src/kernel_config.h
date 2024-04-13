#ifndef KERNEL_CONFIG_H_
#define KERNEL_CONFIG_H_

#include <stdint.h>

typedef struct {
    // IPs y puertos
    uint16_t PUERTO_ESCUCHA;
    char* IP_MEMORIA;
    uint16_t PUERTO_MEMORIA;
    char* IP_CPU;
    uint16_t PUERTO_CPU_DISPATCH;
    uint16_t PUERTO_CPU_INTERRUPT;
    
    
    // KERNEL
    char* ALGORITMO_PLANIFICACION;
    uint16_t QUANTUM_RR;
    char** DISPOSITIVOS_IO;
    uint32_t* TIEMPOS_IO;
    uint16_t GRADO_MAX_MULTIPROGRAMACION;
    
} t_config_kernel;

#endif