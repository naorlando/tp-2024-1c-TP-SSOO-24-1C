#ifndef BLOQUES_H
#define BLOQUES_H

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdint.h>
#include <string.h>
#include <commons/log.h>
#include <commons/config.h>


void crear_archivo_bloques(const char* path, uint32_t block_size, uint32_t block_count);
void leer_bloque(uint32_t puntero, void * buffer, uint32_t bytes_a_leer , char * bloques_path);
void escribir_bloque(uint32_t puntero, uint32_t bytes_a_escribir , void * buffer , char * bloques_path);



#endif