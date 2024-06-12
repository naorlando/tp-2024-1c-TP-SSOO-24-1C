#include "interruption.h"

char* obtener_string_from_interruption(t_name_interruption tipo) {
    switch (tipo) {
        case QUANTUM_INTERRUPT:
            return "QUANTUM";
        case EXIT_INTERRUPT:
            return "EXIT";
        default:
            return "UNKNOWN";
    }
}
