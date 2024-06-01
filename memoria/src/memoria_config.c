#include "memoria_config.h"
#include <stdbool.h>

t_memoria_config* crear_memoria_config(){
    t_memoria_config* mc = malloc(sizeof(t_memoria_config));

    if(mc == NULL){
        return NULL;
    }

    return mc;
}

void destruir(t_memoria_config* mc){

}

bool cargar_memoria_config(t_memoria_config* mc, t_config* cf){
    mc->PUERTO_ESCUCHA = config_get_int_value(cf, "PUERTO_ESCUCHA");
    mc->TAM_MEMORIA = config_get_int_value(cf, "TAM_MEMORIA");
    mc->TAM_PAGINA = config_get_int_value(cf, "TAM_PAGINA");
    mc->PATH_INSTRUCCIONES = config_get_string_value(cf, "PATH_INSTRUCCIONES");
    mc->RETARDO_RESPUESTA = config_get_int_value(cf, "RETARDO_RESPUESTA");

    return true;
}

char* obtener_path_instrucciones(t_memoria_config* mem_config) {
    return mem_config->PATH_INSTRUCCIONES;
}

uint16_t obtener_tam_memoria(t_memoria_config* mem_config) {
    return mem_config->TAM_MEMORIA;
}

uint16_t obtener_tam_pagina(t_memoria_config* mem_config) {
    return mem_config->TAM_PAGINA;
}

uint16_t obtener_retardo_respuesta(t_memoria_config* mem_config) {
    return mem_config->RETARDO_RESPUESTA;
}

uint16_t obtener_puerto_escucha(t_memoria_config* mem_config) {
    return mem_config->PUERTO_ESCUCHA;
}
