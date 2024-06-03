#ifndef CPU_DISPATCH_H_
#define CPU_DISPATCH_H_

#include "variables_globales.h"
#include "utils/instruccion.h"
#include "utils/estructuras.h"
#include "protocolo_cpu.h"

void ejecutar_instruccion(t_instruction*, t_cpu_registers*);

void cargar_contexto_ejecucion(t_PCB*);

// Solicita la siguiente instrucción a memoria para un proceso dado.
// Pre: Los parámetros pid y pc deben ser valores válidos.
// Post: Se envía una solicitud de la siguiente instrucción a memoria.
void solicitar_instruccion(uint32_t, uint32_t);

// Maneja el ciclo de instrucción de la CPU (fetch, execute, interrupt).
// Pre: La variable global pcb_execute debe apuntar a un PCB válido y no NULL.
//      Los registros de la CPU deben estar correctamente inicializados.
// Post: La instrucción recibida se ejecuta y se maneja cualquier interrupción pendiente.
//       Si no hay interrupción, se solicita la siguiente instrucción.
void manejar_ciclo_de_instruccion();

// Maneja las interrupciones de la CPU.
// Pre: La variable interrupcion_pendiente debe estar correctamente inicializada.
//      La variable global pcb_execute debe apuntar a un PCB válido y no NULL.
// Post: Si hay una interrupción pendiente, se envía el PCB de vuelta al kernel y se resetea la interrupción.
//       Retorna true si hubo una interrupción y se manejó, false en caso contrario.
bool manejar_interrupcion();

// Recibe un PCB del kernel y prepara el contexto de ejecución.
// Pre: La variable global fd_kernel_dispatch debe ser un descriptor de archivo válido y abierto.
// Post: La variable global pcb_execute se inicializa con el PCB recibido.
//       Se carga el contexto de ejecución del PCB en los registros de la CPU.
//       Se solicita la siguiente instrucción a memoria.
void recibir_pcb();

#endif