#include "tlb.h"

// Definición de variables globales
t_TLB* TLB;

int32_t ultima_referencia = -1;

int32_t _proxima_referencia_tlb();

void inicializar_TLB() {
    if (cpu_config->CANTIDAD_ENTRADAS_TLB == 0) {
        TLB = NULL;
        return;
    }

    TLB = malloc(sizeof(t_TLB) * cpu_config->CANTIDAD_ENTRADAS_TLB);

    for (int i = 0; i < cpu_config->CANTIDAD_ENTRADAS_TLB; i++) {
        TLB[i].pid = -1;
        TLB[i].page = -1;
        TLB[i].frame = -1;
        TLB[i].cont_referencia = -1;
    }
}

bool obtener_marco(uint32_t pid, uint32_t page, uint32_t* frame) {
    if (cpu_config->CANTIDAD_ENTRADAS_TLB == 0) {
        printf("TLB deshabilitada\n");
        return false;
    }

    printf("PID: %d - QUE BUSCO EN TLB? - Pagina: %d\n", pid, page);

    for (int i = 0; i < cpu_config->CANTIDAD_ENTRADAS_TLB; i++) {
        if (TLB[i].page == page && TLB[i].pid == pid) {
            *frame = TLB[i].frame; // TLB HIT
            printf("PID: %d - TLB HIT - Pagina: %d\n", pid, page);
            
            if (strcmp(cpu_config->ALGORITMO_TLB, "LRU") == 0) {
                TLB[i].cont_referencia = _proxima_referencia_tlb();
            }
            return true;
        }
    }

    printf("PID: %d - TLB MISS - Pagina: %d\n", pid, page);
    return false; // TLB MISS
}

void reemplazar(uint32_t pid, uint32_t page, uint32_t frame) {
    if (cpu_config->CANTIDAD_ENTRADAS_TLB == 0) return;

    int32_t victima_a_reemplazar = 0;

    if (strcmp(cpu_config->ALGORITMO_TLB, "FIFO") == 0) {
        static int32_t fifo_index = 0;
        victima_a_reemplazar = fifo_index;
        fifo_index = (fifo_index + 1) % cpu_config->CANTIDAD_ENTRADAS_TLB;
    } else if (strcmp(cpu_config->ALGORITMO_TLB, "LRU") == 0) {
        int32_t referencia_actual = TLB[0].cont_referencia;
        for (int i = 1; i < cpu_config->CANTIDAD_ENTRADAS_TLB; i++) {
            if (referencia_actual > TLB[i].cont_referencia) {
                referencia_actual = TLB[i].cont_referencia;
                victima_a_reemplazar = i;
            }
        }
    }

    // Se reemplaza la seleccionada
    TLB[victima_a_reemplazar].pid = pid;
    TLB[victima_a_reemplazar].page = page;
    TLB[victima_a_reemplazar].frame = frame;
    TLB[victima_a_reemplazar].cont_referencia = _proxima_referencia_tlb();

    imprimir_tlb();
}

void limpiar_proceso_TLB(uint32_t pid) {
    if (cpu_config->CANTIDAD_ENTRADAS_TLB == 0) return;

    for (int i = 0; i < cpu_config->CANTIDAD_ENTRADAS_TLB; i++) {
        if (pid == TLB[i].pid) {
            TLB[i].pid = -1;
            TLB[i].page = -1;
            TLB[i].frame = -1;
            TLB[i].cont_referencia = -1;
        }
    }
}

void imprimir_tlb() {
    if (cpu_config->CANTIDAD_ENTRADAS_TLB == 0) return;

    char *string = malloc(1024); // Cambia el tamaño según sea necesario
    strcpy(string, "=========================TLB===============================\n");
    for (int i = 0; i < cpu_config->CANTIDAD_ENTRADAS_TLB; i++) {
        char entrada_string[128];
        sprintf(entrada_string, "| PID: %i | PAGINA: %i | MARCO: %i | contador: %i\n", 
            TLB[i].pid, TLB[i].page, TLB[i].frame, TLB[i].cont_referencia);
        strcat(string, entrada_string);
    }
    strcat(string, "===========================================================\n");
    printf("%s\n", string);
    free(string);
}

int32_t _proxima_referencia_tlb() {
    static int32_t ultima_referencia = -1;
    ultima_referencia++;
    return ultima_referencia;
}
