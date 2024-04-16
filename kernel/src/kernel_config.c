#include "kernel_config.h"

t_kernel_config *crear_kernel_config()
{
    t_kernel_config *kc = malloc(sizeof(t_kernel_config));

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
    kc->PUERTO_ESCUCHA = config_get_int_value(cf, "PUERTO_ESCUCHA");
    kc->IP_MEMORIA = strdup(config_get_string_value(cf, "IP_MEMORIA"));
    kc->PUERTO_MEMORIA = config_get_int_value(cf, "PUERTO_MEMORIA");
    kc->IP_CPU = strdup(config_get_string_value(cf, "IP_CPU"));
    kc->PUERTO_CPU_DISPATCH = config_get_int_value(cf, "PUERTO_CPU_DISPATCH");
    kc->PUERTO_CPU_INTERRUPT = config_get_int_value(cf, "PUERTO_CPU_INTERRUPT");
    kc->ALGORITMO_PLANIFICACION = strdup(config_get_string_value(cf, "ALGORITMO_PLANIFICACION"));
    kc->QUANTUM = config_get_int_value(cf, "QUANTUM");
    kc->RECURSOS = config_get_array_value(cf, "RECURSOS");
    char **INSTANCIAS_RECURSOS = config_get_array_value(cf, "INSTANCIAS_RECURSOS");
    kc->INSTANCIAS_RECURSOS = convert_string_array_to_int_array(INSTANCIAS_RECURSOS);
    string_array_destroy(INSTANCIAS_RECURSOS);
    kc->GRADO_MULTIPROGRAMACION = config_get_int_value(cf, "GRADO_MULTIPROGRAMACION");

    return true;
}