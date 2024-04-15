#include "cpu_config.h"

t_cpu_config *crear_cpu_config()
{
    t_cpu_config *cpu_config = malloc(sizeof(t_cpu_config));

    if (cpu_config == NULL)
    {
        return NULL;
    }

    return cpu_config;
}

void destruir(t_cpu_config *cpu_config)
{
    free(cpu_config);
}

bool cargar_cpu_config(t_cpu_config *cpu_config, t_config *cf)
{

    cpu_config->IP_MEMORIA = strdup(config_get_string_value(cf, "IP_MEMORIA"));
    cpu_config->PUERTO_MEMORIA = config_get_int_value(cf, "PUERTO_MEMORIA");
    cpu_config->PUERTO_ESCUCHA_DISPATCH = config_get_int_value(cf, "PUERTO_ESCUCHA_DISPATCH");
    cpu_config->PUERTO_ESCUCHA_INTERRUPT = config_get_int_value(cf, "PUERTO_ESCUCHA_INTERRUPT");
    cpu_config->CANTIDAD_ENTRADAS_TLB = config_get_int_value(cf, "CANTIDAD_ENTRADAS_TLB");
    cpu_config->ALGORITMO_TLB = config_get_string_value(cf, "ALGORITMO_TLB");

    return true;
}
