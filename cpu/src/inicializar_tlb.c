#include "inicializar_tlb.h"


void inicializar_TLB(uint32_t entradas, t_list **TLB_list) {
    *TLB_list = list_create();
    for (uint32_t i = 0; i < entradas; i++) {
        t_TLB* entry = malloc(sizeof(t_TLB));
        entry->page = -1; // Inicializar con valores no válidos
        entry->pid = -1;
        entry->frame = -1;
        entry->cont_referencia = -1; // Inicializar cont_referencia también si es necesario
        list_add(*TLB_list, entry);
    }
}