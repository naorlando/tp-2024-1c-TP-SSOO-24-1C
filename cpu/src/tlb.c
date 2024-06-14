#include "tlb.h"


static TLB tlb;  // Variable global de la TLB

algoritmoTLB parse_algoritmo_tlb(const char *policy_str) {
    if (strcmp(policy_str, "FIFO") == 0) {
        return FIFO;
    } else if (strcmp(policy_str, "LRU") == 0) {
        return LRU;
    } else {
        fprintf(stderr, "Unknown TLB replacement policy: %s\n", policy_str);
        exit(EXIT_FAILURE);
    }
}

void init_tlb(void) {
    int max_entry = cpu_config->CANTIDAD_ENTRADAS_TLB;
    algoritmoTLB policy = parse_algoritmo_tlb(cpu_config->ALGORITMO_TLB);

    tlb.entries = (TLBEntry *)malloc(sizeof(TLBEntry) * max_entry);
    if (tlb.entries == NULL) {
        perror("Error allocating memory for TLB entries");
        exit(EXIT_FAILURE);
    }
    tlb.size = 0;
    tlb.max_entry = max_entry;
    tlb.next_fifo = 0;
    tlb.policy = policy;
}

bool obtener_marco(uint32_t pid, uint32_t page, uint32_t* frame) {
    for (int i = 0; i < tlb.size; i++) {
        if (tlb.entries[i].pagina == page && tlb.entries[i].pid == pid) {
            *frame = tlb.entries[i].marco; // TLB HIT
            log_info(logger_cpu, "PID: %d - TLB HIT - Pagina: %d", pid, page);
            if (tlb.policy == LRU) {
                tlb.entries[i].timestamp = (int)time(NULL); // Update timestamp for LRU
            }
            return true;
        }
    }
    log_info(logger_cpu, "PID: %d - TLB MISS - Pagina: %d", pid, page);
    return false; // TLB MISS
}

int search_tlb(int pid, int pagina) {
    uint32_t frame;
    if (obtener_marco(pid, pagina, &frame)) {
        return frame;
    } else {
        return -1; // TLB Miss
    }
}

void add_tlb_entry(int pid, int pagina, int marco) {
    if (tlb.size < tlb.max_entry) {
      //no hay espacio en la tlb
        tlb.entries[tlb.size].pid = pid;
        tlb.entries[tlb.size].pagina = pagina;
        tlb.entries[tlb.size].marco = marco;
        tlb.entries[tlb.size].timestamp = (int)time(NULL);
        tlb.size++;
    } else {
        // TLB : no hay espacio en la tlb, reemplazar siguiendo el algoritmo 
        //configurado pudiendo elegir como víctima cualquier otra entrada (incluso de otro proceso)
        int index_to_replace = 0;
        if (tlb.policy == FIFO) {
            index_to_replace = tlb.next_fifo;
            tlb.next_fifo = (tlb.next_fifo + 1) % tlb.max_entry;
        } else if (tlb.policy == LRU) {
            int oldest_timestamp = INT_MAX;
            for (int i = 0; i < tlb.max_entry; i++) {
                if (tlb.entries[i].timestamp < oldest_timestamp) {
                    oldest_timestamp = tlb.entries[i].timestamp;
                    index_to_replace = i;
                }
            }
        }

        // Reemplazar la entrada seleccionada
        tlb.entries[index_to_replace].pid = pid;
        tlb.entries[index_to_replace].pagina = pagina;
        tlb.entries[index_to_replace].marco = marco;
        tlb.entries[index_to_replace].timestamp = (int)time(NULL);
    }
}

void clear_tlb(void) {
    free(tlb.entries);
    tlb.entries = NULL;
    tlb.size = 0;
    tlb.max_entry = 0;
}