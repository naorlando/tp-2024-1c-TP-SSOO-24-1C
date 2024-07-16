#include "syncro.h"

void initialize_mutexes() 
{
    pthread_mutex_init(&MUTEX_READY, NULL);
    pthread_mutex_init(&MUTEX_EXIT, NULL);
    pthread_mutex_init(&MUTEX_NEW,NULL);
    pthread_mutex_init(&MUTEX_EXECUTE,NULL);
    pthread_mutex_init(&MUTEX_DICTIONARY,NULL);
    pthread_mutex_init(&MUTEX_RECURSOS, NULL);
    pthread_mutex_init(&MUTEX_AUX_READY, NULL);
    pthread_mutex_init(&MUTEX_COLA_RETORNO_PCB, NULL); // ¿se podria hacer por una variable global?
}

void initialize_semaphores()
{
    sem_init(&SEM_READY, 0, 0);
    sem_init(&SEM_EXIT,0,0);
    sem_init(&BLOQUEADOR, 0, 0);
    sem_init(&SEM_NEW,0,0);
    sem_init(&SEM_MULTIPROGRAMACION,0, kernel_config->GRADO_MULTIPROGRAMACION);
    sem_init(&SEM_CPU,0,1);
    sem_init(&SEM_PCB_RETURNS, 0, 0);
    sem_init(&SEM_AUX_READY, 0, 0);
}