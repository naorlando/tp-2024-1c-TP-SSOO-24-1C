#include "estructuras.h"

/*
 *  Create & destroy structures
 */

// PCB
t_PCB *pcb_create(uint32_t pid, uint32_t quantum)
{

    t_PCB *pcb = malloc(sizeof(t_PCB));

    pcb->pid = pid;
    pcb->program_counter = 0;
    pcb->quantum = quantum;
    pcb->cpu_registers = cpu_registers_create();

    return pcb;
}

void pcb_destroy(t_PCB *pcb)
{

    cpu_registers_destroy(pcb->cpu_registers);
    free(pcb);
}

// CPU registers
t_cpu_registers *cpu_registers_create()
{

    t_cpu_registers *cpu_registers = malloc(sizeof(t_cpu_registers));

    cpu_registers->pc = 0;
    cpu_registers->ax = 0;
    cpu_registers->bx = 0;
    cpu_registers->cx = 0;
    cpu_registers->dx = 0;
    cpu_registers->eax = 0;
    cpu_registers->ebx = 0;
    cpu_registers->ecx = 0;
    cpu_registers->edx = 0;
    cpu_registers->si = 0;
    cpu_registers->di = 0;

    return cpu_registers;
}

void cpu_registers_destroy(t_cpu_registers *cpu_registers)
{

    if(cpu_registers != NULL){
    free(cpu_registers);
    }
}
