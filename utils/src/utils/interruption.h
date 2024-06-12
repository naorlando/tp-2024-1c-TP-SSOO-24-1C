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

char* obtener_string_from_interruption(t_name_interruption);


#endif //INTERRUPTION_H_