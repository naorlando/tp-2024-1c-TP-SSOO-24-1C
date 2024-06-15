#ifndef DATOS_HILOS_H_
#define DATOS_HILOS_H_

#include <pthread.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct {
    uint32_t pid;
    uint32_t quantum;
    pthread_t* hilo;
} t_datos_hilo;

// crear un t_datos_hilo
// pre: pid, quantum y hilo validos
// post: devuelve un t_datos_hilo
t_datos_hilo* datos_hilo_create(uint32_t pid, uint32_t quantum, pthread_t* hilo);

// destruir un t_datos_hilo
// pre: t_datos_hilo creado
// post: se destruye el t_datos_hilo
void datos_hilo_destroy(t_datos_hilo* datos_hilo);

// obtener pid de un t_datos_hilo
// pre: t_datos_hilo creado
// post: se retorna el pid del t_datos_hilo
uint32_t get_pid_datos_hilo(t_datos_hilo* datos_hilo);

// obtener quantum de un t_datos_hilo
// pre: t_datos_hilo creado
// post: se retorna el quantum del t_datos_hilo
uint32_t get_quantum(t_datos_hilo* datos_hilo);

// obtener hilo de un t_datos_hilo
// pre: t_datos_hilo creado
// post: se retorna el hilo del t_datos_hilo
pthread_t* get_hilo(t_datos_hilo* datos_hilo);

#endif /*DATOS_HILOS_H_*/