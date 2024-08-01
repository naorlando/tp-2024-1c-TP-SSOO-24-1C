#ifndef ARCHIVOS_H
#define ARCHIVOS_H


#include "variables_globales.h"
#include <stdint.h>
#include <stdbool.h>
#include <commons/bitarray.h>
#include <commons/collections/list.h>
#include <commons/string.h>
#include <commons/config.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <unistd.h> // Librería para usleep
// #include <sys/stat.h> // Librería para manejo de archivos
#include "comportamientos_dialfs.h"


t_list* listar_archivos_metadata_en_directorio(t_dialfs* fs);

bool levantar_metadata(t_dialfs *fs, char *nombre_metadata);

void levantar_archivos_desde_metadata(t_dialfs* fs);

t_archivo_dialfs* buscar_archivo(t_dialfs *fs, char *nombre_archivo);

uint32_t get_tamanio_archivo(t_dialfs *fs, char *nombre_archivo);

uint32_t get_bloque_inicial_archivo(t_dialfs *fs, char *nombre_archivo);



#endif // ARCHIVOS_H