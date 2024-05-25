#include "tabla_procesos.h"

//Tabla de procesos que van a estar en memoria
t_dictionary* tabla_procesos;

bool agregar_proceso(t_proceso* proceso) {
    if(_esta_vacia_tabla()){
        bool creacion_tabla = _crear_tabla();

        //Hubo problemas al crear la tabla
        if (!creacion_tabla) return false;
    }

    char* key = uint8_to_string(obtener_pid(proceso));

    if(key == NULL) return false;

    dictionary_put(tabla_procesos, key, proceso);

    return true;
}

t_proceso* obtener_proceso(uint8_t pid) {
    if(_esta_vacia_tabla()) return NULL;

    char* key = uint8_to_string(pid);

    if(key == NULL) return NULL;

    t_proceso* proceso = (t_proceso*)dictionary_get(tabla_procesos, key);
    
    return (proceso != NULL) proceso : NULL;
}

bool eliminar_proceso(uint8_t pid) {

    if(_esta_vacia_tabla()) return false;

    bool seElimino= true;

    char* key = uint8_to_string(pid);

    if(key == NULL) return false;

    if(dictionary_has_key(tabla_procesos, key)) {
        dictionary_remove_and_destroy(tabla_procesos, key, eliminar_proceso);
    }else{
        seElimino= false;
    }

    return seElimino;
}

uint8_t cantidad_procesos_activos() {
    return dictionary_size(tabla_procesos);
}

void eliminar_tabla() {

    if(!_esta_vacia_tabla()){
        dictionary_clean_and_destroy_elements(tabla_procesos, eliminar_proceso);
    }
}

bool _crear_tabla(){
    tabla_procesos = dictionary_create();

    return (tabla_procesos != NULL) ? true : false; 
}

bool _esta_vacia_tabla(){
    return dictionary_is_empty(tabla_procesos);
}

