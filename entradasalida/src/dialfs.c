#include "dialfs.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <commons/collections/list.h>

//===============================================
// FUNCIONES DE INICIALIZACIÓN Y DESTRUCCIÓN
//===============================================

t_dialfs* inicializar_dialfs(char* path_base, uint32_t block_size, uint32_t block_count, uint32_t retraso_compactacion) {
    t_dialfs* fs = malloc(sizeof(t_dialfs));
    fs->path_base = strdup(path_base);
    fs->block_size = block_size;
    fs->block_count = block_count;
    fs->retraso_compactacion = retraso_compactacion;
    
    int bitmap_size = (block_count + 7) / 8;
    void* bitarray = calloc(bitmap_size, 1);
    fs->bitmap = bitarray_create_with_mode(bitarray, bitmap_size, LSB_FIRST);
    
    fs->archivos = list_create();
    
    return fs;
}

void destruir_dialfs(t_dialfs* fs) {
    free(fs->path_base);
    bitarray_destroy(fs->bitmap);
    list_destroy_and_destroy_elements(fs->archivos, free);
    free(fs);
}

//===============================================
// FUNCIONES DE OPERACIONES DE ARCHIVO
//===============================================
bool crear_archivo(t_dialfs* fs, char* nombre) {
    t_archivo_dialfs* nuevo_archivo = malloc(sizeof(t_archivo_dialfs));
    nuevo_archivo->nombre = strdup(nombre);
    nuevo_archivo->bloque_inicial = buscar_bloque_libre(fs);
    nuevo_archivo->tamanio = 0;

    if (nuevo_archivo->bloque_inicial == -1) {
        free(nuevo_archivo->nombre);
        free(nuevo_archivo);
        return false;
    }

    list_add(fs->archivos, nuevo_archivo);
    bitarray_set_bit(fs->bitmap, nuevo_archivo->bloque_inicial);
    return true;
}

bool eliminar_archivo(t_dialfs* fs, char* nombre) {
    for (int i = 0; i < list_size(fs->archivos); i++) {
        t_archivo_dialfs* archivo = list_get(fs->archivos, i);
        if (strcmp(archivo->nombre, nombre) == 0) {
            liberar_bloques(fs, archivo->bloque_inicial, archivo->tamanio);
            list_remove_and_destroy_element(fs->archivos, i, (void*)destruir_archivo_dialfs);
            return true;
        }
    }
    return false;
}

bool truncar_archivo(t_dialfs* fs, char* nombre, uint32_t nuevo_tamanio) {
    t_archivo_dialfs* archivo = buscar_archivo(fs, nombre);
    if (archivo == NULL) return false;

    if (nuevo_tamanio > archivo->tamanio) {
        if (!ampliar_archivo(fs, archivo, nuevo_tamanio)) {
            return false;
        }
    } else if (nuevo_tamanio < archivo->tamanio) {
        liberar_bloques(fs, archivo->bloque_inicial + nuevo_tamanio / fs->block_size, archivo->tamanio - nuevo_tamanio);
    }

    archivo->tamanio = nuevo_tamanio;
    return true;
}

bool escribir_archivo(t_dialfs* fs, char* nombre, void* datos, uint32_t tamanio, uint32_t offset) {
    
    t_archivo_dialfs* archivo = buscar_archivo(fs, nombre);
    if (archivo == NULL) return false;

    if (offset + tamanio > archivo->tamanio) {
        if (!truncar_archivo(fs, nombre, offset + tamanio)) {
            return false;
        }
    }

    uint32_t bloque_inicio = archivo->bloque_inicial + offset / fs->block_size;
    uint32_t offset_bloque = offset % fs->block_size;

    escribir_bloques(fs, bloque_inicio, offset_bloque, datos, tamanio);
    return true;
}

bool leer_archivo(t_dialfs* fs, char* nombre, void* buffer, uint32_t tamanio, uint32_t offset) {
    // Implementación faltante
    t_archivo_dialfs* archivo = buscar_archivo(fs, nombre);
    if (archivo == NULL) return false;

    if (offset >= archivo->tamanio) return false;

    uint32_t tamanio_real = (offset + tamanio > archivo->tamanio) ? (archivo->tamanio - offset) : tamanio;
    uint32_t bloque_inicio = archivo->bloque_inicial + offset / fs->block_size;
    uint32_t offset_bloque = offset % fs->block_size;

    leer_bloques(fs, bloque_inicio, offset_bloque, buffer, tamanio_real);
    return true;
}

//===============================================
// FUNCIONES DE MANTENIMIENTO
//===============================================

void compactar_fs(t_dialfs* fs) {
    // Implementación faltante
    list_sort(fs->archivos, (void*)comparar_bloques_iniciales);

    uint32_t bloque_actual = 0;
    for (int i = 0; i < list_size(fs->archivos); i++) {
        t_archivo_dialfs* archivo = list_get(fs->archivos, i);
        if (archivo->bloque_inicial != bloque_actual) {
            mover_bloques(fs, archivo->bloque_inicial, bloque_actual, archivo->tamanio / fs->block_size);
            archivo->bloque_inicial = bloque_actual;
        }
        bloque_actual += (archivo->tamanio + fs->block_size - 1) / fs->block_size;
    }

    // Limpiar bloques no utilizados
    for (uint32_t i = bloque_actual; i < fs->block_count; i++) {
        bitarray_clean_bit(fs->bitmap, i);
    }

    usleep(fs->retraso_compactacion * 1000);
}

bool es_necesario_compactar(t_dialfs* fs) {
    // Implementación faltante
    uint32_t bloques_libres = 0;
    uint32_t bloques_libres_contiguos = 0;
    uint32_t max_bloques_libres_contiguos = 0;

    for (uint32_t i = 0; i < fs->block_count; i++) {
        if (!bitarray_test_bit(fs->bitmap, i)) {
            bloques_libres++;
            bloques_libres_contiguos++;
            if (bloques_libres_contiguos > max_bloques_libres_contiguos) {
                max_bloques_libres_contiguos = bloques_libres_contiguos;
            }
        } else {
            bloques_libres_contiguos = 0;
        }
    }

    return bloques_libres > max_bloques_libres_contiguos;
}

//===============================================
// FUNCIONES AUXILIARES
//===============================================

void enviar_datos_leidos(int fd, void* buffer, uint32_t tamanio) {
    t_package* package = package_create(MSG_DIALFS_DATA, tamanio);
    t_buffer* package_buffer = get_buffer(package);
    buffer_add_data(package_buffer, buffer, tamanio);
    package_send(package, fd);
    package_destroy(package);
}