#include "syncro.h"



void initializeSemaphores(){
    pthread_mutex_init(&MUTEX_READY, NULL);
    pthread_mutex_init(&MUTEX_EXIT, NULL);
    pthread_mutex_init(&MUTEX_NEW,NULL);

    sem_init(&SEM_READY, 0, 0);
    sem_init(&SEM_EXIT,0,0);
    sem_init(&BLOQUEADOR, 0, 0);
    sem_init(&SEM_NEW,0,0);
    sem_init(&SEM_MULTIPROGRAMACION,0, kernel_config->GRADO_MULTIPROGRAMACION);
    sem_init(&SEM_CPU,0,1);
}