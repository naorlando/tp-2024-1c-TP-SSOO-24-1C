#ifndef ESTRUCTURAS_H_
#define ESTRUCTURAS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <stdbool.h>
#include <sys/socket.h>


typedef struct
{
    uint32_t pc;
    uint8_t ax;
    uint8_t bx;
    uint8_t cx;
    uint8_t dx;
    uint32_t eax;
    uint32_t ebx;
    uint32_t ecx;
    uint32_t edx;
    uint32_t si;
    uint32_t di;
} t_cpu_registers;

typedef struct
{
    uint32_t pid;
    uint32_t program_counter;
    uint32_t quantum;                
    t_cpu_registers* cpu_registers; 
} t_PCB;

// proceso que se envia a memoria.
typedef struct {
    uint32_t pid;
    char* path;
} t_new_process;

// t_dictionary



t_PCB* pcb_create(uint32_t pid,  uint32_t quantum);
void pcb_destroy(t_PCB* pcb);

t_cpu_registers* cpu_registers_create();
void cpu_registers_destroy(t_cpu_registers* cpu_registers);

//TODO Corregir
//t_proceso* crear_proceso(uint32_t pid, const char* path);
//oid destruir_proceso(t_proceso* proceso);

t_new_process* create_new_process(uint32_t pid, char* path);


#endif