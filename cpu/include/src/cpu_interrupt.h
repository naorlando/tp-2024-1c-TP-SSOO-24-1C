#ifndef CPU_INTERRUPT_H_
#define CPU_INTERRUPT_H_

#include <commons/log.h>
#include "utils/buffer.h"
#include "utils/protocolo.h"
#include "protocolo_cpu.h"

extern int fd_server_interrupt;

extern int fd_kernel_interrupt;
void atender_cpu_kernel_interrupt(t_log* logger);

// Maneja las interrupciones de la CPU.
// Pre: La variable interrupcion_pendiente debe estar correctamente inicializada.
//      La variable global pcb_execute debe apuntar a un PCB válido y no NULL.
// Post: Si hay una interrupción pendiente, se envía el PCB de vuelta al kernel y se resetea la interrupción.
//       Retorna true si hubo una interrupción y se manejó, false en caso contrario.
bool manejar_interrupcion();

// Carga el contexto de ejecución de la CPU en el PCB.
// Pre: El PCB debe ser válido y no NULL.
// Post: El contexto de ejecución de la CPU se carga en el PCB.
void cargar_contexto_ejecucion_a_pcb(t_PCB*);


#endif