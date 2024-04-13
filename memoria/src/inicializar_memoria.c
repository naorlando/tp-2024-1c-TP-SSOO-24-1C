#include "inicializar_memoria.h"

t_log* logger_memoria;
t_log* logger_memoria_debug;
t_config* config_memoria;
t_memoria_config* memoria_config;

void _iniciar_logger(){
    logger_memoria = log_create("memoria.log", "MEMORIA_LOG", 1, LOG_LEVEL_INFO);

    if(logger_memoria == NULL){
        perror("No se pudo crear el archivo log para el modulo de memoria");
        //exit(EXIT_FAILURE);
    }

    logger_memoria_debug = log_create("memoria.log", "MEMORIA_LOG", 1, LOG_LEVEL_TRACE);

    if(logger_memoria_debug == NULL){
        perror("No se pudo crear el archivo log de debug para el modulo de memoria");
        //exit(EXIT_FAILURE);
    }
}