#include "inicializar_memoria.h"

t_log *logger_memoria;
t_log *logger_memoria_debug;
t_config *config_memoria;
t_memoria_config *memoria_config;
t_dictionary *tabla_procesos;

memory_t * espacio_memoria;
t_dictionary *diccionario_tablas_de_paginas_por_proceso;
pthread_mutex_t MUTEX_DICCIONARIO_TABLAS_PAGINAS;
bool *bitarray_marcos_de_memoria;
pthread_mutex_t MUTEX_BIT_ARRAY_MEMORY;
pthread_mutex_t MUTEX_MEMORY_SPACE;
bool is_structure_created = false;
bool handshake_cpu = false;

int fd_server;
int fd_entradasalida;
int fd_cpu;
int fd_kernel;

char *server_port;

void init()
{
    _iniciar_logger();
    _iniciar_config();
    _inicar_espacio_memoria();
    _iniciar_tabla_procesos();
    imprimir_config();
}

void _iniciar_logger()
{
    logger_memoria = log_create("memoria.log", "MEMORIA_LOG", 1, LOG_LEVEL_INFO);

    if (logger_memoria == NULL)
    {
        perror("No se pudo crear el archivo log para el modulo de memoria");
        // exit(EXIT_FAILURE);
    }

    logger_memoria_debug = log_create("memoria.log", "MEMORIA_LOG", 1, LOG_LEVEL_TRACE);

    if (logger_memoria_debug == NULL)
    {
        perror("No se pudo crear el archivo log de debug para el modulo de memoria");
        // exit(EXIT_FAILURE);
    }
}

void _iniciar_config()
{
    config_memoria = config_create("/home/utnso/tp-2024-1c-TP-SSOO-24-1C/memoria/cfg/memoria.config");

    if (config_memoria == NULL)
    {
        perror("No se pudo crear el archivo de configuracion para el modulo de memoria");
        // exit(EXIT_FAILURE);
    }

    // Creo la estructura para guardar los datos del archivo de configuracion
    memoria_config = crear_memoria_config();

    if (memoria_config == NULL)
    {
        log_error(logger_memoria, "Error al crear la estructura t_memoria_config");
    }

    bool cargo = cargar_memoria_config(memoria_config, config_memoria);

    // chequeo si el segmentation fault es por la carga del config de memoria
    if (!cargo)
    {
        log_error(logger_memoria, "Error al cargar memoria config");
    }
    else
    {
        log_info(logger_memoria, "Se cargo correctamente MEMORIA.CONFIG");
    }
}
void _inicar_espacio_memoria()
{
   espacio_memoria = initialize_memory(memoria_config->TAM_MEMORIA,memoria_config->TAM_PAGINA);
   create_page_tables_structure();


}

void _iniciar_tabla_procesos()
{
    crear_tabla_procesos();

    if (tabla_procesos != NULL)
    {
        log_info(logger_memoria, "Se creo correctamente la tabla de procesos.");
    }
    else
    {
        log_error(logger_memoria, "Error al crear la tabla de procesos.");
    }
}

void imprimir_config()
{
    log_trace(logger_memoria_debug, "PUERTO DE ESCUCHA DE LA MEMORIA: %d", obtener_puerto_escucha(memoria_config));
}

