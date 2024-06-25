#ifndef MANAGER_IOS_H_
#define MANAGER_IOS_H_

#include <pthread.h>
#include <commons/log.h>
#include "variables_globales.h"
#include "protocolo_kernel.h"
#include "utils/estructuras.h"
#include "utils/solicitudes_io.h"

t_IO_connection* get_IO_connection(char*);
void agregar_IO_connection(t_IO_connection*);
void agregar_IO_cliente(int);

#endif //MANAGER_IOS_H_