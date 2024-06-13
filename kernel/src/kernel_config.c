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
    free(kc->IP_MEMORIA);
    free(kc->IP_CPU);
    free(kc->ALGORITMO_PLANIFICACION);
    string_array_destroy(kc->RECURSOS);
    //free(kc->INSTANCIAS_RECURSOS); TODO: Armar funcion que elimine el array uint32_t*
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

uint16_t obtener_puerto_escucha(t_kernel_config *kc)
{
    return kc->PUERTO_ESCUCHA;
}

char* obtener_ip_memoria(t_kernel_config *kc)
{
    return kc->IP_MEMORIA;
}

uint16_t obtener_puerto_memoria(t_kernel_config *kc)
{
    return kc->PUERTO_MEMORIA;
}

char* obtener_ip_cpu(t_kernel_config *kc)
{
    return kc->IP_CPU;
}

uint16_t obtener_puerto_cpu_dispatch(t_kernel_config *kc)
{
    return kc->PUERTO_CPU_DISPATCH;
}

uint16_t obtener_puerto_cpu_interrupt(t_kernel_config *kc)
{
    return kc->PUERTO_CPU_INTERRUPT;
}

char* obtener_algoritmo_planificacion(t_kernel_config *kc)
{
    return kc->ALGORITMO_PLANIFICACION;
}

uint16_t obtener_quantum(t_kernel_config *kc)
{
    return kc->QUANTUM;
}

char** obtener_recursos(t_kernel_config *kc)
{
    return kc->RECURSOS;
}

uint32_t* obtener_instancias_recursos(t_kernel_config *kc)
{
    return kc->INSTANCIAS_RECURSOS;
}

uint16_t obtener_grado_multiprogramacion(t_kernel_config *kc)
{
    return kc->GRADO_MULTIPROGRAMACION;
}