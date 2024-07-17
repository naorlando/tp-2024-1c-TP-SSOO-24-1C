#include "mmu.h"

t_datos_dir_logica *crear_dir_logica(uint32_t dir_logica_decimal)
{

    t_datos_dir_logica *dir_logica = malloc(sizeof(t_datos_dir_logica));

    dir_logica->num_pagina = floor(dir_logica_decimal / page_size);
    dir_logica->desplazamiento_pagina = dir_logica_decimal - (dir_logica->num_pagina * page_size); 
    // número_página = floor(dirección_lógica / tamaño_página)
    // desplazamiento = dirección_lógica - número_página * tamaño_página
    return dir_logica;
}