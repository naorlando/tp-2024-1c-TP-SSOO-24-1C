#ifndef PROTOCOLO_CPU_H_
#define PROTOCOLO_CPU_H_

#include "utils/protocolo.h"
#include <variables_globales.h>
#include "utils/estructuras.h"
#include "utils/instruccion.h"
#include "utils/next_instruction.h"
#include "utils/solicitudes_io.h"

int send_example_memoria();
int recv_example_msg_kernel();

/*********** SEND AND RECIVE FUNCTIONS KERNEL <-> CPU***********/
// Recibe un t_PCB desde el Kernel.
// Pre: El descriptor de archivo fd_kernel_dispatch debe ser válido y estar abierto.
// Post: Retorna un puntero a t_PCB que contiene el PCB recibido.
t_PCB* recv_pcb_kernel();

// Envía un t_PCB al Kernel.
// Pre: La variable global pcb_execute debe ser un puntero válido a t_PCB y no debe ser NULL.
//      El descriptor de archivo fd_kernel_dispatch debe ser válido y estar abierto.
// Post: El PCB contenido en pcb_execute se envía al kernel.
void send_pcb_kernel();

// Envia un t_PCB al Kernel por una interrupción.
// Pre: La variable global pcb_execute debe ser un puntero válido a t_PCB y no debe ser NULL.
//      El descriptor de archivo fd_kernel_dispatch debe ser válido y estar abierto.
// Post: El PCB contenido en pcb_execute se envía al kernel con una interrupción.
void send_pcb_kernel_interruption(t_name_interruption);

/*********** SEND AND RECIVE FUNCTIONS MEMORIA <-> CPU***********/
// Envía la solicitud para obtener la siguiente instrucción.
// Pre: Los parámetros pid y program_counter deben ser válidos.
//      El descriptor de archivo fd_memoria debe ser válido y estar abierto.
// Post: Se envía una solicitud de la siguiente instrucción a memoria.
void send_get_next_instruction_memoria(uint32_t, uint32_t);

// Recibe una instrucción desde la memoria.
// Pre: El descriptor de archivo fd_memoria debe ser válido y estar abierto.
// Post: Retorna un puntero a t_instruction que contiene la instrucción recibida.
t_instruction* recv_instruction_memoria();



void send_solicitud_io_generica_kernel(t_PCB*, t_instruction*);

#endif