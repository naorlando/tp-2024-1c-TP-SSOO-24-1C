#ifndef INTERRUPTION_H_
#define INTERRUPTION_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/collections/list.h>
#include "utils/utils.h"

typedef enum {
    QUANTUM_INTERRUPT,
    EXIT_INTERRUPT
}t_name_interruption;

typedef struct {
    t_name_interruption name;
    uint32_t pid;
}t_interruption;

char* get_string_from_interruption(t_name_interruption);

// crear una interrupcion
// pre: nombre de interrupcion y pid validos
// post: devuelve una interrupcion
t_interruption* create_interruption(t_name_interruption name, uint32_t pid);

// destruir una interrupcion
// pre: interrupcion creada
// post: se destruye la interrupcion
void destroy_interruption(t_interruption* interruption);

// obtener nombre de interrupcion
// pre: interrupcion creada
// post: se retorna el nombre de la interrupcion
t_name_interruption get_name(t_interruption* interruption);

// obtener pid de interrupcion
// pre: interrupcion creada
// post: se retorna el pid de la interrupcion
uint32_t get_pid_interruption(t_interruption* interruption);

// obtener tamaño de interrupcion
// pre: interrupcion creada
// post: se retorna el tamaño de la interrupcion
uint32_t get_interruption_size(t_interruption* interruption);

#endif //INTERRUPTION_H_