#ifndef GESTION_PROCESOS_H_
#define GESTION_PROCESOS_H_

#include "tabla_procesos.h"
#include "proceso.h"
#include "utils/estructuras.h"

bool add_process_to_memory(t_new_process*);

char* _get_path_absolut(char*);

#endif