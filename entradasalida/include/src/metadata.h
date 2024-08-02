#ifndef METADATA_H
#define METADATA_H

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdint.h>
#include <string.h>
#include <commons/log.h>
#include <commons/config.h>
#include <utils/utils.h>


void create_metadata(char *path, uint32_t initial_block);

t_config * read_metadata_(char *path);

void write_metadata(char *path, uint32_t bloque_inicial, uint32_t tamano_archivo);

uint32_t bloque_inicial_archivo(char *path);
 
uint32_t tamano_archivo(char *path);


#endif