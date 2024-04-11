#include <stdlib.h>
#include <stdio.h>
#include <commons/log.h>
#include <commons/string.h>
#include <commons/config.h>
#include <utils/hello.h>
#include <sockets.h>
#include <config.h>
#define SERVERNAME "KERNEL"

int main(int argc, char *argv[])
{
    decir_hola("Kernel");

    t_log *kernel_logger;

    kernel_logger = log_create("../cfg/kernel.log", "KERNEL", true, LOG_LEVEL_INFO);

    if (kernel_logger == NULL)
    {
        perror("Hubo un error con 'Kernel.log'. No se pudo crear o encontrar el archivo de log.");
        exit(EXIT_FAILURE);
    }

    // ############## Config ###################
    t_config_kernel *kernel_config;
    kernel_config = malloc(sizeof(t_config_kernel));

    char *config_path = "../cfg/kernel-ejemplo.config";

    t_config *cfg_aux = config_create(config_path);

    if (cfg_aux == NULL)
    {
        log_error(kernel_logger, "La ruta de la config es erronea");
        return 0;
    }

    char *properties[] = {
        "IP_MEMORIA",
        "PUERTO_MEMORIA",
        "IP_CPU",
        "PUERTO_CPU_DISPATCH",
        "PUERTO_CPU_INTERRUPT",
        "PUERTO_ESCUCHA",
        "ALGORITMO_PLANIFICACION",
        "GRADO_MAX_MULTIPROGRAMACION",
        "DISPOSITIVOS_IO",
        "TIEMPOS_IO",
        "QUANTUM_RR",
        NULL};

    kernel_config->IP_MEMORIA = strdup(config_get_string_value(cfg_aux, "IP_MEMORIA"));
    kernel_config->PUERTO_MEMORIA = config_get_int_value(cfg_aux, "PUERTO_MEMORIA");
    kernel_config->IP_CPU = strdup(config_get_string_value(cfg_aux, "IP_CPU"));
    kernel_config->PUERTO_CPU_DISPATCH = config_get_int_value(cfg_aux, "PUERTO_CPU_DISPATCH");
    kernel_config->PUERTO_CPU_INTERRUPT = config_get_int_value(cfg_aux, "PUERTO_CPU_INTERRUPT");
    kernel_config->PUERTO_ESCUCHA = config_get_int_value(cfg_aux, "PUERTO_ESCUCHA");
    kernel_config->ALGORITMO_PLANIFICACION = strdup(config_get_string_value(cfg_aux, "ALGORITMO_PLANIFICACION"));
    kernel_config->GRADO_MAX_MULTIPROGRAMACION = config_get_int_value(cfg_aux, "GRADO_MAX_MULTIPROGRAMACION");
    kernel_config->DISPOSITIVOS_IO = config_get_array_value(cfg_aux, "DISPOSITIVOS_IO");
    char **tiempos_io = config_get_array_value(cfg_aux, "TIEMPOS_IO");
    // kernel_config->TIEMPOS_IO = conversor_string_to_array(tiempos_io); crear un parser
    string_array_destroy(tiempos_io);
    kernel_config->QUANTUM_RR = config_get_int_value(cfg_aux, "QUANTUM_RR");

    log_info(kernel_logger, "Archivo de configuracion cargado correctamente");

    // ############## Socket ###################

    char *server_port = string_itoa(kernel_config->PUERTO_ESCUCHA);
    int fd_server;

    fd_server = iniciar_servidor(
        kernel_logger,
        SERVERNAME,
        "0.0.0.0",
        server_port);

    if (fd_server != -1)
        log_info(kernel_logger, "%s server listo escuchando en puerto %s", SERVERNAME, server_port);
    else
        log_error(kernel_logger, "Error al iniciar %s server en puerto %s", SERVERNAME, server_port);

    free(server_port);

    config_destroy(cfg_aux);
}
