#include "dialfs.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <commons/collections/list.h>

t_dialfs* inicializar_dialfs(char* path_base, uint32_t block_size, uint32_t block_count, uint32_t retraso_compactacion) {
    t_dialfs* fs = malloc(sizeof(t_dialfs));
    fs->path_base = strdup(path_base);
    fs->block_size = block_size;
    fs->block_count = block_count;
    fs->retraso_compactacion = retraso_compactacion;
    
    // Inicializar bitmap
    int bitmap_size = (block_count + 7) / 8;
    void* bitarray = calloc(bitmap_size, 1);
    fs->bitmap = bitarray_create_with_mode(bitarray, bitmap_size, LSB_FIRST);
    
    // Inicializar lista de archivos
    fs->archivos = list_create();
    
    return fs;
}

void destruir_dialfs(t_dialfs* fs) {
    free(fs->path_base);
    bitarray_destroy(fs->bitmap);
    list_destroy_and_destroy_elements(fs->archivos, free);
    free(fs);
}

bool crear_archivo(t_dialfs* fs, char* nombre) {
    // Implementar lógica para crear archivo
    // Buscar bloque libre en el bitmap
    // Actualizar bitmap y lista de archivos
}

bool eliminar_archivo(t_dialfs* fs, char* nombre) {
    // Implementar lógica para eliminar archivo
    // Actualizar bitmap y lista de archivos
}

bool truncar_archivo(t_dialfs* fs, char* nombre, uint32_t nuevo_tamanio) {
    // Implementar lógica para truncar archivo
    // Puede requerir compactación si se amplía
}

bool escribir_archivo(t_dialfs* fs, char* nombre, void* datos, uint32_t tamanio, uint32_t offset) {
    // Implementar lógica para escribir en archivo
    // Manejar escritura que cruza múltiples bloques
}

bool leer_archivo(t_dialfs* fs, char* nombre, void* buffer, uint32_t tamanio, uint32_t offset) {
    // Implementar lógica para leer de archivo
    // Manejar lectura que cruza múltiples bloques
}

void compactar_fs(t_dialfs* fs) {
    // Implementar lógica de compactación
    // Mover bloques para eliminar fragmentación
    usleep(fs->retraso_compactacion * 1000);
}