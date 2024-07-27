#include "comportamientos_dialfs.h"


//===============================================
// FUNCIONES DE MANEJO DE ARCHIVOS
//===============================================

bool crear_archivo_dialfs(char* nombre) {
    return crear_archivo(dialfs, nombre);
}

bool eliminar_archivo_dialfs(char* nombre) {
    return eliminar_archivo(dialfs, nombre);
}

bool truncar_archivo_dialfs(char* nombre, uint32_t nuevo_tamanio) {
    return truncar_archivo(dialfs, nombre, nuevo_tamanio);
}

bool escribir_archivo_dialfs(char* nombre, void* datos, uint32_t tamanio, uint32_t offset) {
    return escribir_archivo(dialfs, nombre, datos, tamanio, offset);
}

bool leer_archivo_dialfs(char* nombre, void* buffer, uint32_t tamanio, uint32_t offset) {
    return leer_archivo(dialfs, nombre, buffer, tamanio, offset);
}

//===============================================
// FUNCIONES DE MANTENIMIENTO
//===============================================

void compactar_fs_dialfs() {
    compactar_fs(dialfs);
}

//===============================================
// FUNCIONES DE LECTURA Y ESCRITURA DE MEMORIA
//===============================================

/* EMPIEZO COMENTARIO de funciones de lectura y escritura de memoria
Modificar cuando se implemente la memoria

void* obtener_datos_de_memoria(uint32_t direccion_logica, uint32_t tamanio) {
    void* datos = malloc(tamanio);
    if (datos == NULL) {
        log_error(logger_entradasalida, "Error al asignar memoria para obtener datos");
        return NULL;
    }

    uint32_t pagina_inicial = direccion_logica / dialfs->block_size;
    uint32_t offset_inicial = direccion_logica % dialfs->block_size;
    uint32_t bytes_copiados = 0;

    while (bytes_copiados < tamanio) {
        uint32_t pagina_actual = pagina_inicial + bytes_copiados / dialfs->block_size;
        uint32_t offset_actual = (bytes_copiados == 0) ? offset_inicial : 0;
        uint32_t bytes_restantes = tamanio - bytes_copiados;
        uint32_t bytes_en_pagina = dialfs->block_size - offset_actual;
        uint32_t bytes_a_copiar = (bytes_restantes < bytes_en_pagina) ? bytes_restantes : bytes_en_pagina;

        t_entrada_tabla_de_paginas* entrada = get_page_data(dialfs->pid_actual, pagina_actual);
        if (entrada == NULL || !entrada->presencia) {
            log_error(logger_entradasalida, "Error: página no presente en memoria");
            free(datos);
            return NULL;
        }

        void* direccion_fisica = get_memory_address(entrada->frame, offset_actual);
        memcpy(datos + bytes_copiados, direccion_fisica, bytes_a_copiar);

        bytes_copiados += bytes_a_copiar;
    }

    return datos;
}


bool escribir_datos_en_memoria(uint32_t direccion_logica, void* datos, uint32_t tamanio) {
    uint32_t pagina_inicial = direccion_logica / dialfs->block_size;
    uint32_t offset_inicial = direccion_logica % dialfs->block_size;
    uint32_t bytes_escritos = 0;

    while (bytes_escritos < tamanio) {
        uint32_t pagina_actual = pagina_inicial + bytes_escritos / dialfs->block_size;
        uint32_t offset_actual = (bytes_escritos == 0) ? offset_inicial : 0;
        uint32_t bytes_restantes = tamanio - bytes_escritos;
        uint32_t bytes_en_pagina = dialfs->block_size - offset_actual;
        uint32_t bytes_a_escribir = (bytes_restantes < bytes_en_pagina) ? bytes_restantes : bytes_en_pagina;

        t_entrada_tabla_de_paginas* entrada = get_page_data(dialfs->pid_actual, pagina_actual);
        if (entrada == NULL || !entrada->presencia) {
            log_error(logger_entradasalida, "Error: página no presente en memoria");
            return false;
        }

        void* direccion_fisica = get_memory_address(entrada->frame, offset_actual);
        memcpy(direccion_fisica, datos + bytes_escritos, bytes_a_escribir);

        bytes_escritos += bytes_a_escribir;
    }

    return true;
}
TERMINO COMENTARIO*/