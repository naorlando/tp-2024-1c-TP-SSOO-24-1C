#include "cpu_syncro.h"

void initializeSemaphores(){
    sem_init(&SEM_INTERRUPT,0,1);
}