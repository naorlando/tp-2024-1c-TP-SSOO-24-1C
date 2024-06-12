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

uint16_t obtener_puerto_escucha_dispatch(t_cpu_config* cpu_config)
{
    return cpu_config->PUERTO_ESCUCHA_DISPATCH;
}

char* obtener_ip_memoria(t_cpu_config* cpu_config)
{
    return cpu_config->IP_MEMORIA;
}

uint16_t obtener_puerto_memoria(t_cpu_config* cpu_config)
{
    return cpu_config->PUERTO_MEMORIA;
}

uint16_t obtener_puerto_escucha_interrupt(t_cpu_config* cpu_config)
{
    return cpu_config->PUERTO_ESCUCHA_INTERRUPT;
}

uint16_t obtener_cantidad_entradas_tlb(t_cpu_config* cpu_config)
{
    return cpu_config->CANTIDAD_ENTRADAS_TLB;
}

char* obtener_algoritmo_tlb(t_cpu_config* cpu_config)
{
    return cpu_config->ALGORITMO_TLB;
}