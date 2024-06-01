#ifndef NEXT_INSTRUCCION_H_
#define NEXT_INSTRUCCION_H_

#include <stdint.h>
#include <stdlib.h>

typedef struct {
    uint32_t pid_process;
    uint32_t program_counter_process;
}t_next_instruction;

// Crea una nueva estructura t_next_instruction con el PID del proceso y el program counter especificados.
// Pre: Los valores de pid y program_counter deben ser válidos.
// Post: Retorna un puntero a una estructura t_next_instruction con los valores asignados.
//       Si no se puede asignar memoria, retorna NULL.
t_next_instruction* crear_siguiente_instruccion(uint32_t, uint32_t);


// Obtiene el PID del proceso desde una estructura t_next_instruction.
// Pre: El puntero a t_next_instruction debe ser válido y no NULL.
// Post: Retorna el valor del PID del proceso.
uint32_t obtener_pid_process(t_next_instruction*);

// Obtiene el program counter del proceso desde una estructura t_next_instruction.
// Pre: El puntero a t_next_instruction debe ser válido y no NULL.
// Post: Retorna el valor del program counter del proceso.
uint32_t obtener_pc_process(t_next_instruction*);

// Elimina una estructura t_next_instruction, liberando su memoria.
// Pre: El puntero a t_next_instruction debe ser válido y no NULL.
// Post: La memoria asignada a la estructura t_next_instruction es liberada.
void eliminar_next_instruction(t_next_instruction*);

#endif //NEXT_INSTRUCCION_H_