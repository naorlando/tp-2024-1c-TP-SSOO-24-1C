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

uint32_t traductor_cantidad_paginas(uint32_t dir_logica_decimal, uint32_t tamano) {
    t_datos_dir_logica *dir_logica_inicial = crear_dir_logica(dir_logica_decimal);

    uint32_t paginas_necesarias = 1; // Al menos una página será necesaria
    uint32_t espacio_restante_primera_pagina = page_size - dir_logica_inicial->desplazamiento_pagina;

    // Si el tamaño cabe en el espacio restante de la primera página
    if (tamano <= espacio_restante_primera_pagina) {
        free(dir_logica_inicial);
        return paginas_necesarias;
    }

    // Restamos el espacio que cabe en la primera página y contamos la página usada
    tamano -= espacio_restante_primera_pagina;

    // Calculamos cuántas páginas completas adicionales son necesarias
    paginas_necesarias += (tamano + page_size - 1) / page_size;

    free(dir_logica_inicial);
    return paginas_necesarias;
}
