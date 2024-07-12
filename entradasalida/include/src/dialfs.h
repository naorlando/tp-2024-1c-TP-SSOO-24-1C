#ifndef DIALFS_H
#define DIALFS_H

#include <stdint.h>
#include <stdbool.h>
#include <commons/bitarray.h>

typedef struct {
    char* nombre;
    uint32_t bloque_inicial;
    uint32_t tamanio;
} t_archivo_dialfs;

typedef struct {
    char* path_base;
    uint32_t block_size;
    uint32_t block_count;
    uint32_t retraso_compactacion;
    t_bitarray* bitmap;
    t_list* archivos;
} t_dialfs;


t_dialfs* inicializar_dialfs(char* path_base, uint32_t block_size, uint32_t block_count, uint32_t retraso_compactacion);
void destruir_dialfs(t_dialfs* fs);

bool crear_archivo(t_dialfs* fs, char* nombre);
bool eliminar_archivo(t_dialfs* fs, char* nombre);
bool truncar_archivo(t_dialfs* fs, char* nombre, uint32_t nuevo_tamanio);
bool escribir_archivo(t_dialfs* fs, char* nombre, void* datos, uint32_t tamanio, uint32_t offset);
bool leer_archivo(t_dialfs* fs, char* nombre, void* buffer, uint32_t tamanio, uint32_t offset);
void compactar_fs(t_dialfs* fs);

#endif