#include "tlb.h"

// Definición de variables globales

int32_t ultima_referencia = -1;

int32_t _proxima_referencia_tlb();

bool obtener_marco(uint32_t pid, uint32_t page, uint32_t *frame) {
    if (cpu_config->CANTIDAD_ENTRADAS_TLB == 0) {
        log_info(logger_cpu, "TLB deshabilitada\n");
        return false;
    }


    for (int i = 0; i < list_size(TLB_list); i++) {
        t_TLB* entry = (t_TLB*)list_get(TLB_list, i);
        if (entry->page == page && entry->pid == pid) {
            *frame = entry->frame; // TLB HIT
            log_info(logger_cpu, "PID: %d - TLB HIT - Pagina: %d\n", pid, page);

            if (strcmp(cpu_config->ALGORITMO_TLB, "LRU") == 0) {
                entry->cont_referencia = _proxima_referencia_tlb();
            }
            return true;
        }
    }

    log_info(logger_cpu, "PID: %d - TLB MISS - Pagina: %d\n", pid, page);
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
        int32_t referencia_actual = ((t_TLB*)list_get(TLB_list, 0))->cont_referencia;
        for (int i = 1; i < list_size(TLB_list); i++) {
            t_TLB* entry = (t_TLB*)list_get(TLB_list, i);
            if (referencia_actual > entry->cont_referencia) {
                referencia_actual = entry->cont_referencia;
                victima_a_reemplazar = i;
            }
        }
    }

    // Se reemplaza la seleccionada
    t_TLB* entry = (t_TLB*)list_get(TLB_list, victima_a_reemplazar);
    entry->pid = pid;
    entry->page = page;
    entry->frame = frame;
    entry->cont_referencia = _proxima_referencia_tlb();

    //  imprimir_tlb();
}

void limpiar_proceso_TLB(uint32_t pid) {
    if (cpu_config->CANTIDAD_ENTRADAS_TLB == 0) return;

    for (int i = 0; i < list_size(TLB_list); i++) {
        t_TLB* entry = (t_TLB*)list_get(TLB_list, i);
        if (pid == entry->pid) {
            entry->pid = -1;
            entry->page = -1;
            entry->frame = -1;
            entry->cont_referencia = -1;
        }
    }
}

// void imprimir_tlb() {
//     if (cpu_config->CANTIDAD_ENTRADAS_TLB == 0) return;

//     char* string = malloc(1024); // Cambia el tamaño según sea necesario
//     strcpy(string, "=========================TLB===============================\n");
//     for (int i = 0; i < list_size(TLB_list); i++) {
//         t_TLB* entry = (t_TLB*)list_get(TLB_list, i);
//         char entrada_string[128];
//         sprintf(entrada_string, "| PID: %i | PAGINA: %i | MARCO: %i | contador: %i\n",
//                 entry->pid, entry->page, entry->frame, entry->cont_referencia);
//         strcat(string, entrada_string);
//     }
//     strcat(string, "===========================================================\n");
//     printf("%s\n", string);
//     free(string);
// }

int32_t _proxima_referencia_tlb() {
    static int32_t ultima_referencia = -1;
    ultima_referencia++;
    return ultima_referencia;
}