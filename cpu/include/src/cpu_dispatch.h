#ifndef CPU_DISPATCH_H_
#define CPU_DISPATCH_H_

#include "variables_globales.h"
#include "utils/instruccion.h"
#include "utils/estructuras.h"
#include "protocolo_cpu.h"
#include "cpu_interrupt.h"
#include "utils/utils.h"

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

// Recibe un PCB del kernel y prepara el contexto de ejecución.
// Pre: La variable global fd_kernel_dispatch debe ser un descriptor de archivo válido y abierto.
// Post: La variable global pcb_execute se inicializa con el PCB recibido.
//       Se carga el contexto de ejecución del PCB en los registros de la CPU.
//       Se solicita la siguiente instrucción a memoria.
void recibir_pcb();

void solicitar_IO(t_instruction*);

// Obtiene el registro correspondiente a un nombre dado.
// Pre: Los registros deben ser válidos y no NULL.
// Post: Retorna un puntero al registro correspondiente al nombre dado.
uint32_t* _obtener_registro(t_cpu_registers *, const char *);

// Establece el valor de un registro dado un nombre.
// Pre: Los registros deben ser válidos y no NULL.
// Post: El registro correspondiente al nombre dado se establece con el valor dado.
void _establecer_registro(t_cpu_registers *, char *, uint32_t);

// Obtiene el valor de un registro dado un nombre.
// Pre: Los registros deben ser válidos y no NULL.
// Post: Retorna el valor del registro correspondiente al nombre dado.
uint32_t _obtener_valor_registro(t_cpu_registers *, char *);


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

// TODO: Implementar.
uint32_t leer_memoria(uint32_t);
void escribir_memoria(uint32_t direccion_logica,uint32_t valor_datos);
bool ajustar_tamano_proceso(t_cpu_registers *cpu_registers, int nuevo_tamano);
void informar_kernel_error(const char *mensaje);
void copiar_cadena(uint32_t origen, uint32_t destino, int tamano);
#endif