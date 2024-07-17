#include "interruption.h"

char* get_string_from_interruption(t_name_interruption tipo) {
    switch (tipo) {
        case QUANTUM_INTERRUPT:
            return "QUANTUM";
        case EXIT_INTERRUPT:
            return "EXIT";
        default:
            return "UNKNOWN";
    }
}

t_interruption* create_interruption(t_name_interruption name, uint32_t pid)
{
    t_interruption* interruption = malloc(sizeof(t_interruption));
    if(interruption == NULL) {
        return NULL;
    }
    interruption->name = name;
    interruption->pid = pid;
    return interruption;
}

void destroy_interruption(t_interruption* interruption)
{
    free(interruption);
}

t_name_interruption get_name(t_interruption* interruption)
{
    return interruption->name;
}

uint32_t get_pid_interruption(t_interruption* interruption)
{
    return interruption->pid;
}

uint32_t get_interruption_size(t_interruption* interruption)
{
    return sizeof(t_name_interruption) + sizeof(uint32_t);
}