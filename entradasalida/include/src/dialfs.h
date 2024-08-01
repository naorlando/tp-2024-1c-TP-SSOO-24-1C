#ifndef DIALFS_H
#define DIALFS_H

#include <stdint.h>
#include <stdbool.h>
#include <commons/collections/list.h>
#include <commons/string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // Librería para usleep
#include <sys/stat.h> // Librería para manejo de archivos


// Estructura para almacenar los datos de un archivo en el sistema DialFS.
typedef struct {
    char* nombre;
    char* path_archivo; 
} t_archivo_dialfs;

// Estructura para almacenar los datos del sistema de archivos DialFS.
typedef struct {
    char* path_base;
    uint32_t block_size;
    uint32_t block_count;
    uint32_t retraso_compactacion; 
    t_list* archivos;
    char* path_bitmap;
    char* path_bloques;

} t_dialfs;




#endif // DIALFS_H