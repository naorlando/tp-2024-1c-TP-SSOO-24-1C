#include "estructuras.h"

/*
 *  Create & destroy structures
 */

// PCB
t_PCB *pcb_create(uint32_t pid, uint32_t quantum)
{
    t_PCB *pcb = malloc(sizeof(t_PCB));

    pcb->pid = pid;
    pcb->quantum = quantum;
    pcb->program_counter = 0;
    pcb->cpu_registers = cpu_registers_create();

    return pcb;
}


void pcb_destroy(t_PCB *pcb)
{
    cpu_registers_destroy(pcb->cpu_registers);
    free(pcb);
}

t_cpu_registers* get_cpu_registers(t_PCB* pcb)
{
    return pcb->cpu_registers;
}

uint32_t get_pcb_size(t_PCB* pcb)
{
    return (sizeof(uint32_t) * 3) + get_cpu_registers_size(get_cpu_registers(pcb));
}

t_state get_state(t_PCB* pcb){
    return pcb->state;
}

void set_state(t_PCB* pcb, t_state state){
    pcb->state = state;
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

uint32_t get_cpu_registers_size(t_cpu_registers *cpu_registers)
{
    return (sizeof(uint32_t) * 7) + (sizeof(uint8_t) * 4);
}

// lo usa memoria para crear un proceso
// t_proceso* crear_proceso(uint32_t pid, const char* path) {
//     t_proceso* proceso = malloc(sizeof(t_proceso));
//     proceso->pid = pid;
//     proceso->path = strdup(path);
//     return proceso;
// }

// void destruir_proceso(t_proceso* proceso) {
//     free(proceso->path);
//     // list_destroy(proceso->tabla_paginas);
//     free(proceso);
// }
// 
t_new_process* create_new_process(uint32_t pid,char* path)
{    
    t_new_process *new_process = malloc(sizeof(t_new_process));

    new_process->pid = pid;
    new_process->path = strdup(path);

    return new_process;  
}

void destroy_new_process(t_new_process* new_process) 
{
    if(new_process->path != NULL) {
        free(new_process->path);
    }

    free(new_process);
}

char* get_path_new_process(t_new_process* new_process)
{
    return new_process->path;
}

uint32_t get_size_new_process(t_new_process* new_process) 
{
    return sizeof(uint32_t) + strlen(new_process->path) + 1 + sizeof(uint32_t);
}