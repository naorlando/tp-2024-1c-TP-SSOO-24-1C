#include "syncro.h"

void initialize_mutexes() 
{
    pthread_mutex_init(&MUTEX_READY, NULL);
    pthread_mutex_init(&MUTEX_EXIT, NULL);
    pthread_mutex_init(&MUTEX_NEW,NULL);
    pthread_mutex_init(&MUTEX_EXECUTE,NULL);
    pthread_mutex_init(&MUTEX_DICTIONARY,NULL);
    pthread_mutex_init(&MUTEX_SOLICITUD, NULL);
    pthread_mutex_init(&MUTEX_RECURSOS, NULL);
    pthread_mutex_init(&MUTEX_AUX_READY, NULL);
    pthread_mutex_init(&MUTEX_COLA_RETORNO_PCB, NULL); // ¿se podria hacer por una variable global?
}

void initialize_semaphores()
{
    sem_init(&SEM_READY, 0, 0);
    sem_init(&SEM_EXIT,0,0);
    sem_init(&SEM_BLOCKED, 0, 0);
    sem_init(&SEM_NEW,0,0);
    sem_init(&SEM_MULTIPROGRAMACION,0, kernel_config->GRADO_MULTIPROGRAMACION);
    sem_init(&SEM_CPU,0,1);
    sem_init(&SEM_SOLICITUDES,0,0);
    sem_init(&SEM_PCB_RETURNS, 0, 0);
    sem_init(&SEM_AUX_READY, 0, 0);
    sem_init(&SEM_PLANIFICACION_NEW_READY_INICIADA, 0, 1);
    sem_init(&SEM_PLANIFICACION_READY_INICIADA, 0, 1);
    sem_init(&SEM_PLANIFICACION_EXEC_INICIADA, 0, 1);
    sem_init(&SEM_PLANIFICACION_BLOCKED_INICIADA, 0, 1);
}

// funciones auxiliares para el manejo de la planificacion:
void detener_cola_new(void* arg)
{
    sem_wait(&SEM_PLANIFICACION_NEW_READY_INICIADA);
}
void detener_cola_ready(void* arg)
{
    sem_wait(&SEM_PLANIFICACION_READY_INICIADA);
}
void detener_cola_exec(void* arg)
{
    sem_wait(&SEM_PLANIFICACION_EXEC_INICIADA);
}

// TODO: distribuir los semaforos en los distintos modulos para que se detenga la planificacion con la logica de bloqueados.
void detener_cola_blocked(void* arg)
{
    sem_wait(&SEM_PLANIFICACION_BLOCKED_INICIADA);
}