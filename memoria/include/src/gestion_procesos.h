#ifndef GESTION_PROCESOS_H_
#define GESTION_PROCESOS_H_

#include "tabla_procesos.h"
#include "proceso.h"
#include "utils/estructuras.h"


// Agrega un nuevo proceso a la memoria.
// Pre: El puntero a t_new_process debe ser válido y no NULL.
// Post: Retorna true si el proceso se ha agregado exitosamente a la memoria, 
//       o false si hubo un error.
bool add_process_to_memory(t_new_process*);

// Obtiene la ruta absoluta dado un path relativo.
// Pre: El puntero a path_relative debe ser válido y no NULL.
// Post: Retorna un puntero a una cadena que contiene la ruta absoluta.
char* _get_path_absolut(char*);

#endif