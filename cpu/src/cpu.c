
#include "cpu.h"

#define SERVERNAME "CPU"

int main(int argc, char* argv[]) {
   
    t_log *cpu_logger;

    cpu_logger = log_create("../cfg/cpu.log", "cpu", true, LOG_LEVEL_INFO);

    if (cpu_logger == NULL)
    {
        perror("Hubo un error con 'Cpu.log'. No se pudo crear o encontrar el archivo de log.");
        exit(EXIT_FAILURE);
    }

    // ############## Config ###################
    t_config_cpu *cpu_config;
    cpu_config = malloc(sizeof(t_config_cpu));

    char *config_path = "../cfg/cpu-ejemplo.config";

    t_config *cfg_aux = config_create(config_path);

    if (cfg_aux == NULL)
    {
        log_error(cpu_logger, "La ruta de la config es erronea");
        return 0;
    }


    cpu_config->IP_MEMORIA = strdup(config_get_string_value(cfg_aux, "IP_MEMORIA"));
    cpu_config->PUERTO_MEMORIA = config_get_int_value(cfg_aux, "PUERTO_MEMORIA");
    cpu_config->PUERTO_ESCUCHA_DISPATCH = config_get_int_value(cfg_aux, "PUERTO_ESCUCHA_DISPATCH");
    cpu_config->PUERTO_ESCUCHA_INTERRUPT = config_get_int_value(cfg_aux, "PUERTO_ESCUCHA_INTERRUPT");
    cpu_config->CANTIDAD_ENTRADAS_TLB = config_get_int_value(cfg_aux, "CANTIDAD_ENTRADAS_TLB");
    cpu_config->ALGORITMO_TLB = config_get_string_value(cfg_aux, "ALGORITMO_TLB");
   

    log_info(cpu_logger, "Archivo de configuracion cargado correctamente");

    // ############## Socket ###################

  char *server_port = string_itoa(cpu_config->PUERTO_ESCUCHA_DISPATCH);
    int fd_server;

    fd_server = iniciar_servidor(
        cpu_logger,
        SERVERNAME,
        "0.0.0.0",
        server_port);

    if (fd_server != -1)
        log_info(cpu_logger, "%s server listo escuchando en puerto %s", SERVERNAME, server_port);
    else
        log_error(cpu_logger, "Error al iniciar %s server en puerto %s", SERVERNAME, server_port);

    free(server_port);

    config_destroy(cfg_aux);


    return 0;
}
