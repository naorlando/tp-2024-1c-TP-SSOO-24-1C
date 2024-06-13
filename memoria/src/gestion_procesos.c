#include "gestion_procesos.h"

bool add_process_to_memory(t_new_process* new_process) {
    //Se envia un proceso nulo
    if(new_process == NULL) return false;

    t_proceso* process = crear_proceso(new_process->pid, _get_path_absolut(new_process->path));

    if(process == NULL) return false;

    return agregar_proceso(process);
}

char* _get_path_absolut(char* path_relative) {
    char* path_instrucciones = obtener_path_instrucciones(memoria_config);

    char* path_absoluto = join(path_instrucciones, path_relative, "");

    if(path_absoluto == NULL) return NULL;

    return path_absoluto;
}