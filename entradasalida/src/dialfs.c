#include "dialfs.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

t_dialfs* inicializar_dialfs(char* path_base, uint32_t block_size, uint32_t block_count, uint32_t retraso_compactacion) {
    t_dialfs* fs = malloc(sizeof(t_dialfs));
    fs->path_base = strdup(path_base);
    fs->block_size = block_size;
    fs->block_count = block_count;
    fs->retraso_compactacion = retraso_compactacion;
    // Inicializa el bitmap y otras estructuras necesarias
    return fs;
}

void destruir_dialfs(t_dialfs* fs) {
    free(fs->path_base);
    // Libera otras estructuras
    free(fs);
}

bool crear_archivo(t_dialfs* fs, char* nombre) {
    // Implementa la lógica para crear un archivo
}

bool eliminar_archivo(t_dialfs* fs, char* nombre) {
    // Implementa la lógica para eliminar un archivo
}

bool truncar_archivo(t_dialfs* fs, char* nombre, uint32_t nuevo_tamanio) {
    // Implementa la lógica para truncar un archivo
}

bool escribir_archivo(t_dialfs* fs, char* nombre, void* datos, uint32_t tamanio, uint32_t offset) {
    // Implementa la lógica para escribir en un archivo
}

bool leer_archivo(t_dialfs* fs, char* nombre, void* buffer, uint32_t tamanio, uint32_t offset) {
    // Implementa la lógica para leer de un archivo
}

void compactar_fs(t_dialfs* fs) {
    // Implementa la lógica para compactar el sistema de archivos
    usleep(fs->retraso_compactacion * 1000);
}