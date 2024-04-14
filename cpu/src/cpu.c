
#include "cpu.h"

#define SERVERNAME "CPU"

int main(int argc, char *argv[])
{

    init();

    log_info(logger_cpu, "Archivo de configuracion cargado correctamente");

    // ############## Socket ###################

    char *server_port = string_itoa(cpu_config->PUERTO_ESCUCHA_DISPATCH);
  
    int fd_server = iniciar_servidor(logger_cpu, NULL, NULL, server_port);

    if (fd_server != -1)
    {
        log_info(logger_cpu, "%s server listo escuchando en puerto %s", SERVERNAME, server_port);
    }
    else
    {
        log_error(logger_cpu, "Error al iniciar %s server en puerto %s", SERVERNAME, server_port);
    }

    free(server_port);

    return EXIT_SUCCESS;

}
