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
    // Implementación de la creación de archivo
    // ...
}

bool eliminar_archivo(t_dialfs* fs, char* nombre) {
    // Implementación de la eliminación de archivo
    // ...
}

bool truncar_archivo(t_dialfs* fs, char* nombre, uint32_t nuevo_tamanio) {
    // Implementación del truncado de archivo
    // ...
}

bool escribir_archivo(t_dialfs* fs, char* nombre, void* datos, uint32_t tamanio, uint32_t offset) {
    // Implementación de la escritura de archivo
    // ...
}

bool leer_archivo(t_dialfs* fs, char* nombre, void* buffer, uint32_t tamanio, uint32_t offset) {
    // Implementación de la lectura de archivo
    // ...
}

//===============================================
// FUNCIONES DE MANTENIMIENTO
//===============================================

void compactar_fs(t_dialfs* fs) {
    // Implementación de la compactación
    // ...
    usleep(fs->retraso_compactacion * 1000);
}

bool es_necesario_compactar(t_dialfs* fs) {
    // Implementación de la verificación de necesidad de compactación
    // ...
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