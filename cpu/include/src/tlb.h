#ifndef TLB_H
#define TLB_H

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <time.h>
#include <commons/string.h>
#include <commons/log.h>
#include <commons/config.h>
#include <stdbool.h>
#include <pthread.h>

#include "variables_globales.h"
#include "cpu_config.h"
#include "cpu_dispatch.h"
#include "cpu_interrupt.h"

// Shared
#include "protocolo_cpu.h"
typedef struct {
    int pid;    // Process ID
    int pagina; // Página
    int marco;  // Marco (Frame)
    int timestamp; // Timestamp para LRU
} TLBEntry;

typedef enum {
    FIFO,
    LRU
} algoritmoTLB;

typedef struct {
    TLBEntry *entries;
    int size;
    int max_entry;
    int next_fifo; // Index for the next FIFO replacement
    algoritmoTLB policy;
} TLB;

// Inicializa la TLB con la capacidad y política de reemplazo dadas
void init_tlb(void);

// Busca una entrada en la TLB, devuelve el marco si se encuentra, -1 si no
int search_tlb(int pid, int pagina);

// Agrega una nueva entrada a la TLB
void add_tlb_entry(int pid, int pagina, int marco);

// Limpia la TLB y libera memoria
void clear_tlb(void);

// Convierte una cadena de caracteres a un valor del enumerado algoritmoTLB
algoritmoTLB parse_algoritmo_tlb(const char *policy_str);

bool obtener_marco(uint32_t pid, uint32_t page, uint32_t* frame);

#endif // TLB_H
