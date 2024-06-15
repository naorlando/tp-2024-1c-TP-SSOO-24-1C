#include "datos_hilos.h"

t_datos_hilo* datos_hilo_create(uint32_t pid, uint32_t quantum, pthread_t* hilo)
{
    t_datos_hilo* datos_hilo = malloc(sizeof(t_datos_hilo));
    if(datos_hilo == NULL)
    {
        return NULL;
    }
    datos_hilo->pid = pid;
    datos_hilo->quantum = quantum;
    datos_hilo->hilo = hilo;
    return datos_hilo;
}

void datos_hilo_destroy(t_datos_hilo* datos_hilo)
{
    free(datos_hilo);
}

uint32_t get_pid_datos_hilo(t_datos_hilo* datos_hilo)
{
    return datos_hilo->pid;
}

uint32_t get_quantum(t_datos_hilo* datos_hilo)
{
    return datos_hilo->quantum;
}

pthread_t* get_hilo(t_datos_hilo* datos_hilo)
{
    return datos_hilo->hilo;
}