#ifndef MANAGER_IOS_H_
#define MANAGER_IOS_H_

#include <pthread.h>
#include <commons/log.h>
#include "variables_globales.h"
#include "protocolo_kernel.h"
#include "utils/estructuras.h"
#include "utils/solicitudes_io.h"

typedef void (*procesar_solicitud_func)(void*);
procesar_solicitud_func obtener_procesador_solicitud(int);
void procesar_solicitud_IO(t_IO_connection*, procesar_solicitud_func);
void procesar_solicitud_generica(t_solicitud_io_generica*);
void procesar_solicitud_stdin(t_solicitud_io_stdin*);
void procesar_solicitud_stdout(t_solicitud_io_stdout*);
void procesar_solicitud_dialfs();

t_IO_connection* get_IO_connection(char*);
void agregar_IO_connection(t_IO_connection*);
char* nuevo_IO_cliente_conectado(int);

#endif //MANAGER_IOS_H_