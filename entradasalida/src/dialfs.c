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
    list_destroy_and_destroy_elements(fs->archivos, (void*)destruir_archivo_dialfs);
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

    if (nuevo_archivo->bloque_inicial == (uint32_t)-1) {
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
        uint32_t espacio_necesario = nuevo_tamanio - archivo->tamanio;
        uint32_t bloques_necesarios = (espacio_necesario + fs->block_size - 1) / fs->block_size;
        
        if (!hay_espacio_contiguo(fs, bloques_necesarios)) {
            if (es_necesario_compactar(fs)) {
                compactar_fs(fs);
            }
        }
        
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
    list_sort(fs->archivos, (void*)comparar_bloques_iniciales);

    uint32_t bloque_actual = 0;
    for (int i = 0; i < list_size(fs->archivos); i++) {
        t_archivo_dialfs* archivo = list_get(fs->archivos, i);
        if (archivo->bloque_inicial != bloque_actual) {
            mover_bloques(fs, archivo->bloque_inicial, bloque_actual, (archivo->tamanio + fs->block_size - 1) / fs->block_size);
            archivo->bloque_inicial = bloque_actual;
        }
        bloque_actual += (archivo->tamanio + fs->block_size - 1) / fs->block_size;
    }

    for (uint32_t i = bloque_actual; i < fs->block_count; i++) {
        bitarray_clean_bit(fs->bitmap, i);
    }

    usleep(fs->retraso_compactacion * 1000);
}

bool es_necesario_compactar(t_dialfs* fs) {
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

uint32_t buscar_bloque_libre(t_dialfs* fs) {
    for (uint32_t i = 0; i < fs->block_count; i++) {
        if (!bitarray_test_bit(fs->bitmap, i)) {
            return i;
        }
    }
    return (uint32_t)-1;
}

void liberar_bloques(t_dialfs* fs, uint32_t bloque_inicial, uint32_t tamanio) {
    uint32_t bloques_a_liberar = (tamanio + fs->block_size - 1) / fs->block_size;
    for (uint32_t i = 0; i < bloques_a_liberar; i++) {
        bitarray_clean_bit(fs->bitmap, bloque_inicial + i);
    }
}

t_archivo_dialfs* buscar_archivo(t_dialfs* fs, char* nombre) {
    for (int i = 0; i < list_size(fs->archivos); i++) {
        t_archivo_dialfs* archivo = list_get(fs->archivos, i);
        if (strcmp(archivo->nombre, nombre) == 0) {
            return archivo;
        }
    }
    return NULL;
}

bool ampliar_archivo(t_dialfs* fs, t_archivo_dialfs* archivo, uint32_t nuevo_tamanio) {
    uint32_t bloques_actuales = (archivo->tamanio + fs->block_size - 1) / fs->block_size;
    uint32_t bloques_necesarios = (nuevo_tamanio + fs->block_size - 1) / fs->block_size;
    uint32_t bloques_adicionales = bloques_necesarios - bloques_actuales;

    for (uint32_t i = 0; i < bloques_adicionales; i++) {
        uint32_t nuevo_bloque = buscar_bloque_libre(fs);
        if (nuevo_bloque == (uint32_t)-1) {
            return false;
        }
        bitarray_set_bit(fs->bitmap, nuevo_bloque);
    }

    return true;
}

void escribir_bloques(t_dialfs* fs, uint32_t bloque_inicio, uint32_t offset_bloque, void* datos, uint32_t tamanio) {
    uint32_t bytes_escritos = 0;
    while (bytes_escritos < tamanio) {
        uint32_t bytes_en_bloque = fs->block_size - offset_bloque;
        uint32_t bytes_a_escribir = (tamanio - bytes_escritos < bytes_en_bloque) ? tamanio - bytes_escritos : bytes_en_bloque;
        
        // Aquí iría la lógica para escribir en el bloque físico
        // Por ahora, solo simulamos la escritura

        bytes_escritos += bytes_a_escribir;
        bloque_inicio++;
        offset_bloque = 0;
    }
}

void leer_bloques(t_dialfs* fs, uint32_t bloque_inicio, uint32_t offset_bloque, void* buffer, uint32_t tamanio) {
    uint32_t bytes_leidos = 0;
    while (bytes_leidos < tamanio) {
        uint32_t bytes_en_bloque = fs->block_size - offset_bloque;
        uint32_t bytes_a_leer = (tamanio - bytes_leidos < bytes_en_bloque) ? tamanio - bytes_leidos : bytes_en_bloque;
        
        // Aquí iría la lógica para leer del bloque físico
        // Por ahora, solo simulamos la lectura

        bytes_leidos += bytes_a_leer;
        bloque_inicio++;
        offset_bloque = 0;
    }
}

void mover_bloques(t_dialfs* fs, uint32_t bloque_origen, uint32_t bloque_destino, uint32_t cantidad_bloques) {
    // Aquí iría la lógica para mover bloques físicos
    // Por ahora, solo actualizamos el bitmap
    for (uint32_t i = 0; i < cantidad_bloques; i++) {
        bool bit = bitarray_test_bit(fs->bitmap, bloque_origen + i);
        bitarray_clean_bit(fs->bitmap, bloque_origen + i);
        if (bit) {
            bitarray_set_bit(fs->bitmap, bloque_destino + i);
        } else {
            bitarray_clean_bit(fs->bitmap, bloque_destino + i);
        }
    }
}

int comparar_bloques_iniciales(t_archivo_dialfs* a, t_archivo_dialfs* b) {
    return a->bloque_inicial - b->bloque_inicial;
}

void destruir_archivo_dialfs(t_archivo_dialfs* archivo) {
    free(archivo->nombre);
    free(archivo);
}

void enviar_datos_leidos(int fd, void* buffer, uint32_t tamanio) {
    t_package* package = package_create(MSG_DIALFS_DATA, tamanio);
    t_buffer* package_buffer = get_buffer(package);
    buffer_add_data(package_buffer, buffer, tamanio);
    package_send(package, fd);
    package_destroy(package);
}

bool hay_espacio_contiguo(t_dialfs* fs, uint32_t bloques_necesarios) {
    uint32_t bloques_contiguos = 0;
    for (uint32_t i = 0; i < fs->block_count; i++) {
        if (!bitarray_test_bit(fs->bitmap, i)) {
            bloques_contiguos++;
            if (bloques_contiguos == bloques_necesarios) {
                return true;
            }
        } else {
            bloques_contiguos = 0;
        }
    }
    return false;
}