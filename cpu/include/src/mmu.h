#ifndef MMU_H
#define MMU_H

#include <stdio.h>
#include <commons/log.h>
#include <stdbool.h>
#include <math.h>

#include "variables_globales.h"
#include "cpu_config.h"

#include "cpu_dispatch.h"
#include "cpu_interrupt.h"

typedef struct {
    uint32_t num_pagina; 
    uint32_t desplazamiento_pagina; 
} t_datos_dir_logica;



t_datos_dir_logica* crear_dir_logica(uint32_t dir_logica_decimal);

uint32_t traductor_cantidad_paginas(uint32_t dir_logica_decimal, uint32_t tamano); 

#endif