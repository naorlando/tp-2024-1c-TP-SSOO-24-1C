#include "cpu_syncro.h"

void initializeSemaphores(){
    sem_init(&SEM_INTERRUPT,0,1);
    sem_init(&SEM_SOCKET_KERNEL_DISPATCH, 0, 1);
}

void initialize_mutexes() 
{
    pthread_mutex_init(&MUTEX_INTERRUPT, NULL);
}