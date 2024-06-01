#include "next_instruction.h"

t_next_instruction* crear_siguiente_instruccion(uint32_t pid, uint32_t program_counter) {
    
    t_next_instruction* siguiente_instruccion = malloc(sizeof(t_next_instruction));

    if(siguiente_instruccion == NULL) return NULL;

    siguiente_instruccion->pid_process = pid;
    siguiente_instruccion->program_counter_process = program_counter;

    return siguiente_instruccion;
}

uint32_t obtener_pid_process(t_next_instruction* next_instruction) {
    return next_instruction->pid_process;
}

uint32_t obtener_pc_process(t_next_instruction* next_instruction) {
    return next_instruction->program_counter_process;
}

void eliminar_next_instruction(t_next_instruction* next_instruction) {
    free(next_instruction);
}