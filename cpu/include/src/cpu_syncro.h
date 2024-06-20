#ifndef SYNCHRO_H
#define SYNCHRO_H

#include <stdint.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <variables_globales.h>


void initializeSemaphores();


void initialize_mutexes();

#endif //SYNCHRO_H