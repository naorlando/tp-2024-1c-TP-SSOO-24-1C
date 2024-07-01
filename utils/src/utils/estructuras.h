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

typedef enum
{
    NEW,
    READY,
    EXEC,
    BLOCKED,
    FINISHED //ex EXIT
} t_state;
typedef struct
{
    uint32_t pid;
    uint32_t program_counter;
    uint32_t quantum;                
    t_cpu_registers* cpu_registers;
    t_state state;
} t_PCB;

// proceso que se envia a memoria.
typedef struct 
{
    uint32_t pid;
    char* path;
} t_new_process;

// t_dictionary

// Estructura para los recursos
typedef struct {
    char *nombre;
    int instancias;
    t_queue *cola_bloqueados;
} t_recurso;



t_PCB* pcb_create(uint32_t pid,  uint32_t quantum);
void pcb_destroy(t_PCB* pcb);
uint32_t get_pcb_size(t_PCB*);
t_cpu_registers* get_cpu_registers(t_PCB*);

// obtengo el estado del pcb
// pre: el pcb fue creado previamente
// post: se retorna el estado del pcb
t_state get_state(t_PCB*);
// seteo el estado del pcb
// pre: el pcb fue creado previamente
// post: se setea el estado del pcb
void set_state(t_PCB*, t_state);

t_cpu_registers* cpu_registers_create();
void cpu_registers_destroy(t_cpu_registers* cpu_registers);
uint32_t get_cpu_registers_size(t_cpu_registers*);

//TODO Corregir
//t_proceso* crear_proceso(uint32_t pid, const char* path);
//oid destruir_proceso(t_proceso* proceso);

t_new_process* create_new_process(uint32_t pid, char* path);
void destroy_new_process(t_new_process*);

char* get_path_new_process(t_new_process*);
uint32_t get_size_new_process(t_new_process*);

#endif