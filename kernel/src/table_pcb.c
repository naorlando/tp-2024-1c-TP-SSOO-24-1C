#include "table_pcb.h"

bool add_pcb(t_PCB *new_pcb)
{
   
    char *key = uint32_to_string(new_pcb->pid);
    if (key == NULL)
        return false;

    dictionary_put(table_pcb, key, new_pcb);

    return true;
}

void create_pcb_table()
{
    table_pcb = dictionary_create();
}

t_PCB *get_pcb(uint32_t pid)
{
    if (_is_empty_table())
        return NULL;

    char *key = uint32_to_string(pid);

    if (key == NULL)
        return NULL;

    t_PCB *pcb = (t_PCB *)dictionary_get(table_pcb, key);

    return (pcb != NULL) ? pcb : NULL;
}

bool delete_pcb(uint32_t pid)
{
    if (_is_empty_table())
        return NULL;

    bool isDelete = true;

    char *key = uint32_to_string(pid);

    if (key == NULL)
        return NULL;

    if (dictionary_has_key(table_pcb, key))
    {
        dictionary_remove_and_destroy(table_pcb, key, free_pcb);
    }
    else
    {
        isDelete = false;
    }

    return isDelete;
}

void delete_pcb_table()
{
    if (!_is_empty_table())
    {
        dictionary_clean_and_destroy_elements(table_pcb, free_pcb);
    }
}

void free_pcb(void * pcb)
{
    t_PCB * pcb_eliminar = (t_PCB *)pcb;
    free(pcb_eliminar->cpu_registers);
    free(pcb_eliminar);
}

uint8_t sizeof_table_pcb()
{
    return dictionary_size(table_pcb);
}

bool _is_empty_table()
{
    return dictionary_is_empty(table_pcb);
}

void update_pcb(t_PCB *pcb)
{
    // actualizar el pcb en la tabla de pcb:
    // actualizar el pcb que ingresa en la tabla de pcbs macheando por pid:
    // Elimino el PCB de la tabla de pcbs que gestiona el Kernel
    delete_pcb(pcb->pid);
    // Agrego el PCB pero con su contexto nuevo
    add_pcb(pcb);
}