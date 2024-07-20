#ifndef MANAGER_IOS_H_
#define MANAGER_IOS_H_

#include <pthread.h>
#include <commons/log.h>
#include "variables_globales.h"
#include "protocolo_kernel.h"
#include "utils/estructuras.h"
#include "utils/solicitudes_io.h"

typedef int (*procesar_solicitud_func)(int, void*);
procesar_solicitud_func obtener_procesador_solicitud(int);
int procesar_solicitud_IO(int, void*, procesar_solicitud_func);
int procesar_solicitud_generica(int, t_solicitud_io_generica*);
int procesar_solicitud_stdin(int, t_solicitud_io_stdin*);
int procesar_solicitud_stdout(int, t_solicitud_io_stdout*);
void procesar_solicitud_dialfs();
void procesar_respuesta_io(int, char*);
void* obtener_siguiente_proceso(t_IO_connection*);

t_IO_connection* get_IO_connection(char*);
void agregar_IO_connection(t_IO_connection*);
t_IO_connection* nuevo_IO_cliente_conectado(int);
t_IO_connection* recibir_io_connection(int);

void proceso_solicita_io(void*);

#endif //MANAGER_IOS_H_