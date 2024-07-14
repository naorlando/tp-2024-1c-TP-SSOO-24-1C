#include "io_interface.h"

//===============================================
// FUNCIONES DE TIPO_INTERFAZ_T
//===============================================

tipo_interfaz_t string_to_tipo_interfaz(char* str) {
    if (strcmp(str, "GENERICA") == 0) {
        return GENERICA;
    } else if (strcmp(str, "STDIN") == 0) {
        return STDIN;
    } else if (strcmp(str, "STDOUT") == 0) {
        return STDOUT;
    } else if (strcmp(str, "DIALFS") == 0) {
        return DIALFS;
    } else {
        return -1; // Si no se encuentra
    }
}

char* tipo_interfaz_to_string(tipo_interfaz_t tipo) {
    switch (tipo) {
        case GENERICA:
            return "GENERICA";
        case STDIN:
            return "STDIN";
        case STDOUT:
            return "STDOUT";
        case DIALFS:
            return "DIALFS";
        default:
            return "UNKNOWN";
    }
}

//===============================================
// FUNCIONES DE T_IO_INTERFACE
//===============================================

t_IO_interface* crear_IO_interface(const char* nombre, tipo_interfaz_t tipo) {
    t_IO_interface* nueva_interfaz = (t_IO_interface*)malloc(sizeof(t_IO_interface));
    if (nueva_interfaz == NULL) {
        return NULL;
    }
    nueva_interfaz->nombre_interfaz = strdup(nombre);
    if (nueva_interfaz->nombre_interfaz == NULL) {
        free(nueva_interfaz);
        return NULL;
    }
    nueva_interfaz->tipo_interfaz = tipo;
    return nueva_interfaz;
}

void liberar_IO_interface(t_IO_interface* interfaz) {
    if (interfaz) {
        free(interfaz->nombre_interfaz);
        free(interfaz);
    }
}

char* obtener_nombre_IO_interface(t_IO_interface* interfaz) {
    if (interfaz) {
        return interfaz->nombre_interfaz;
    }
    return NULL;
}

tipo_interfaz_t obtener_tipo_IO_interface(t_IO_interface* interfaz) {
    if (interfaz) {
        return interfaz->tipo_interfaz;
    }
    return GENERICA;
}

uint32_t obtener_size_IO_interface(t_IO_interface* interfaz) {
    return sizeof(uint32_t) + (strlen(obtener_nombre_IO_interface(interfaz)) + 1) + sizeof(uint32_t);
}

//===============================================
// FUNCIONES DE T_IO_CONNECTION
//===============================================

t_IO_connection* crear_IO_connection(const char* nombre, tipo_interfaz_t tipo, int fd) {
    t_IO_connection* nueva_conexion = (t_IO_connection*)malloc(sizeof(t_IO_connection));
    if (nueva_conexion == NULL) {
        return NULL;
    }
    nueva_conexion->nombre_interfaz = strdup(nombre);
    if (nueva_conexion->nombre_interfaz == NULL) {
        free(nueva_conexion);
        return NULL;
    }
    nueva_conexion->tipo_interfaz = tipo;
    nueva_conexion->file_descriptor = fd;
    nueva_conexion->cola_procesos_bloqueados = queue_create();
    sem_init(&(nueva_conexion->sem_cola_bloqueados), 0, 0);
    pthread_mutex_init(&(nueva_conexion->mutex_cola_bloqueados), NULL);

    return nueva_conexion;
}

void liberar_IO_connection(t_IO_connection* conexion) {
    if (conexion) {
        free(conexion->nombre_interfaz); 
        //TODO: Liberar el fd y la cola de procesos bloqueados
        free(conexion);
    }
}

char* obtener_nombre_conexion(t_IO_connection* conexion) {
    if (conexion) {
        return conexion->nombre_interfaz;
    }
    return NULL;
}

tipo_interfaz_t obtener_tipo_conexion(t_IO_connection* conexion) {
    if (conexion) {
        return conexion->tipo_interfaz;
    }
    return GENERICA;
}

int obtener_file_descriptor(t_IO_connection* conexion) {
    if (conexion) {
        return conexion->file_descriptor;
    }
    return -1;  // Valor por defecto en caso de error
}

t_queue* obtener_cola_procesos_bloqueados(t_IO_connection* conexion) {
    if (conexion) {
        return conexion->cola_procesos_bloqueados;
    }
    return NULL;
}

void* obtener_proceso_bloqueado(t_IO_connection* conexion) {
    t_queue* cola_bloqueados_io = obtener_cola_procesos_bloqueados(conexion);

    if(cola_bloqueados_io == NULL) return NULL;

    pthread_mutex_t* mutex_blocked = obtener_mutex_cola_bloqueados(conexion);

    pthread_mutex_lock(mutex_blocked);
    void* proceso_bloqueado = queue_pop(cola_bloqueados_io);
    pthread_mutex_unlock(mutex_blocked);

    return proceso_bloqueado;
}

sem_t* obtener_semaforo_cola_bloqueados(t_IO_connection* conexion) {
    if (conexion) {
        return &(conexion->sem_cola_bloqueados);
    }
    return NULL;
}

pthread_mutex_t* obtener_mutex_cola_bloqueados(t_IO_connection* conexion) {
    if (conexion) {
        return &(conexion->mutex_cola_bloqueados);
    }
    return NULL;
}

bool agregar_proceso_bloqueado(t_IO_connection* conexion, void* proceso) {
    t_queue* cola_bloqueados_io = obtener_cola_procesos_bloqueados(conexion);

    if(cola_bloqueados_io == NULL) return false;

    pthread_mutex_t* mutex_blocked = obtener_mutex_cola_bloqueados(conexion);

    pthread_mutex_lock(mutex_blocked);
    queue_push(cola_bloqueados_io, proceso);
    pthread_mutex_unlock(mutex_blocked);

    sem_post(obtener_semaforo_cola_bloqueados(conexion));

    return true;
}