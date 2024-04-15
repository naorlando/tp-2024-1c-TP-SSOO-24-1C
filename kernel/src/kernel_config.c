#include "kernel_config.h"

t_kernel_config *crear_kernel_config()
{
    t_kernel_config* kc = malloc(sizeof(t_kernel_config));

    if (kc == NULL)
    {
        return NULL;
    }

    return kc;
}

void destruir(t_kernel_config *kc)
{
	free(kc);
}

bool cargar_kernel_config(t_kernel_config *kc, t_config *cf)
{
    kc->IP_MEMORIA = strdup(config_get_string_value(cf, "IP_MEMORIA"));
    kc->PUERTO_MEMORIA = config_get_int_value(cf, "PUERTO_MEMORIA");
    kc->IP_CPU = strdup(config_get_string_value(cf, "IP_CPU"));
    kc->PUERTO_CPU_DISPATCH = config_get_int_value(cf, "PUERTO_CPU_DISPATCH");
    kc->PUERTO_CPU_INTERRUPT = config_get_int_value(cf, "PUERTO_CPU_INTERRUPT");
    kc->PUERTO_ESCUCHA = config_get_int_value(cf, "PUERTO_ESCUCHA");
    kc->ALGORITMO_PLANIFICACION = strdup(config_get_string_value(cf, "ALGORITMO_PLANIFICACION"));
    kc->GRADO_MAX_MULTIPROGRAMACION = config_get_int_value(cf, "GRADO_MAX_MULTIPROGRAMACION");
    kc->DISPOSITIVOS_IO = config_get_array_value(cf, "DISPOSITIVOS_IO");
    char **tiempos_io = config_get_array_value(cf, "TIEMPOS_IO");
    // kc->TIEMPOS_IO = conversor_string_to_array(tiempos_io); crear un parser
    string_array_destroy(tiempos_io);
    kc->QUANTUM_RR = config_get_int_value(cf, "QUANTUM_RR");

    return true;
}