#ifndef SYNCHRO_H
#define SYNCHRO_H

#include <stdint.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <variables_globales.h>


void initialize_semaphores();
void initialize_mutexes();

// administracion de planificacion:
void detener_cola_new(void* arg);
void detener_cola_ready(void* arg);
void detener_cola_exec(void* arg);
void detener_cola_blocked(void* arg);

#endif