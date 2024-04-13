#include "memoria_config.h"

t_memoria_config* crear_memoria_config(){
    t_memoria_config* mc = malloc(sizeof(t_memoria_config));

    if(mc == NULL){
        return NULL;
    }

    return mc;
}

void destruir(t_memoria_config* mc){

}