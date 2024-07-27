#ifndef BITMAP_H
#define BITMAP_H

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdint.h>
#include <string.h>
#include <commons/log.h>
#include <commons/config.h>
#include <commons/bitarray.h>

void create_bitmap(const char *path, uint32_t block_count);
void set_block_as_used(const char *path, uint32_t block_index);
void set_block_as_free(const char *path, uint32_t block_index);
uint32_t find_first_free_block(const char *path);

#endif