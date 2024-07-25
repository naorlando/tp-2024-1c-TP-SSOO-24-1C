#ifndef MANAGER_IOS_H_
#define MANAGER_IOS_H_

#include <pthread.h>
#include <commons/log.h>
#include "variables_globales.h"
#include "protocolo_kernel.h"
#include "utils/estructuras.h"
#include "utils/solicitudes_io.h"
#include "utils/gestion_conexiones_io.h"
#include "utils_queue.h"
#include "table_pcb.h"

typedef int (*procesar_solicitud_func)(int, void*);
procesar_solicitud_func obtener_procesador_solicitud(int);
int procesar_solicitud_IO(int, void*, procesar_solicitud_func);
int procesar_solicitud_generica(int, t_solicitud_io_generica*);
int procesar_solicitud_stdin(int, t_solicitud_io_stdin*);
int procesar_solicitud_stdout(int, t_solicitud_io_stdout*);
void procesar_solicitud_dialfs();
void procesar_respuesta_io(int, char*);

void proceso_solicita_io(int, void*);

#endif //MANAGER_IOS_H_