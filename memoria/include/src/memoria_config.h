#ifndef MEMORIA_CONFIG_H_
#define MEMORIA_CONFIG_H_

#include <stdio.h>
#include <stdlib.h>
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

t_memoria_config* crear_memoria_config();
void destruir(t_memoria_config*);
bool cargar_memoria_config(t_memoria_config*, t_config*);


#endif /*MEMORIA_CONFIG_H_*/