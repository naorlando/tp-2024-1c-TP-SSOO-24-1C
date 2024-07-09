#ifndef MANAGER_DISPATCH_H_
#define MANAGER_DISPATCH_H_

#include <pthread.h>
#include <commons/log.h>
#include <commons/string.h>
#include "variables_globales.h"
#include "protocolo_kernel.h"
#include "utils/estructuras.h"
#include "utils/solicitudes_io.h"
#include "kernel_config.h"
#include "datos_hilos.h"
#include "utils_queue.h"
#include "table_pcb.h"
#include "recursos.h"

void procesar_ios_genericas();
void procesar_pcb_exit();
void cancelar_hilo_quantum(uint32_t);
void procesar_interrupcion();
void handle_wait_request();
void handle_signal_request();



// forma planteada por el tipo:
void manejar_wait_signal(int cod_op);

// Función para manejar la operación WAIT
bool wait(char *resource_name, t_PCB *pcb);

// Función para manejar la operación SIGNAL
void signal(char *resource_name, t_PCB *pcb);

#endif //MANAGER_DISPATCH_