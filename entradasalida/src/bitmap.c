#include "bitmap.h"

void create_bitmap(const char* path, uint32_t block_count) {
    // Calcular el tamaño del bitmap en bytes. Cada bit representa un bloque,
    // así que se necesitan (block_count + 7) / 8 bytes (redondeando hacia arriba).
    uint32_t bitmap_size = (block_count + 7) / 8;

    // Abrir el archivo en modo de escritura binaria
    FILE *file = fopen(path, "wb");
    if (file == NULL) {
        perror("Failed to create bitmap file");
        return;
    }

    // Crear un bloque de memoria inicializado con ceros para el bitmap
    char *bitarray_memory = calloc(bitmap_size, sizeof(char));
    if (bitarray_memory == NULL) {
        perror("Failed to allocate memory for bitmap");
        fclose(file);
        return;
    }

    // Crear un bitarray con el bloque de memoria
    t_bitarray *bitarray = bitarray_create_with_mode(bitarray_memory, bitmap_size, LSB_FIRST);

    // Escribir el bitmap inicializado con ceros en el archivo
    size_t bytes_written = fwrite(bitarray_memory, sizeof(char), bitmap_size, file);
    if (bytes_written != bitmap_size) {
        perror("Failed to write the complete bitmap to the file");
    }

    // Limpiar recursos
    bitarray_destroy(bitarray);
    free(bitarray_memory);
    fclose(file);
}

// Ejemplo de funciones adicionales para gestionar el bitmap
void set_block_as_used(const char* path, uint32_t block_index) {
    // Calcular el tamaño del bitmap en bytes
    FILE *file = fopen(path, "rb+");
    if (file == NULL) {
        perror("Failed to open bitmap file for updating");
        return;
    }

    // Obtener el tamaño del archivo
    fseek(file, 0, SEEK_END);
    size_t bitmap_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Leer el contenido del bitmap
    char *bitarray_memory = malloc(bitmap_size);
    fread(bitarray_memory, sizeof(char), bitmap_size, file);

    // Crear un bitarray con el bloque de memoria
    t_bitarray *bitarray = bitarray_create_with_mode(bitarray_memory, bitmap_size, LSB_FIRST);

    // Setear el bloque como usado
    bitarray_set_bit(bitarray, block_index);

    // Escribir el bitmap actualizado en el archivo
    fseek(file, 0, SEEK_SET);
    fwrite(bitarray_memory, sizeof(char), bitmap_size, file);

    // Limpiar recursos
    bitarray_destroy(bitarray);
    free(bitarray_memory);
    fclose(file);
}

void set_block_as_free(const char* path, uint32_t block_index) {
    // Calcular el tamaño del bitmap en bytes
    FILE *file = fopen(path, "rb+");
    if (file == NULL) {
        perror("Failed to open bitmap file for updating");
        return;
    }

    // Obtener el tamaño del archivo
    fseek(file, 0, SEEK_END);
    size_t bitmap_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Leer el contenido del bitmap
    char *bitarray_memory = malloc(bitmap_size);
    fread(bitarray_memory, sizeof(char), bitmap_size, file);

    // Crear un bitarray con el bloque de memoria
    t_bitarray *bitarray = bitarray_create_with_mode(bitarray_memory, bitmap_size, LSB_FIRST);

    // Setear el bloque como libre
    bitarray_clean_bit(bitarray, block_index);

    // Escribir el bitmap actualizado en el archivo
    fseek(file, 0, SEEK_SET);
    fwrite(bitarray_memory, sizeof(char), bitmap_size, file);

    // Limpiar recursos
    bitarray_destroy(bitarray);
    free(bitarray_memory);
    fclose(file);
}

uint32_t find_first_free_block(const char* path) {
    // Abrir el archivo bitmap en modo de lectura binaria
    FILE *file = fopen(path, "rb");
    if (file == NULL) {
        perror("Failed to open bitmap file for reading");
        return -1;
    }

    // Obtener el tamaño del archivo
    fseek(file, 0, SEEK_END);
    size_t bitmap_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Leer el contenido del bitmap
    char *bitarray_memory = malloc(bitmap_size);
    if (bitarray_memory == NULL) {
        perror("Failed to allocate memory for bitmap");
        fclose(file);
        return -1;
    }
    fread(bitarray_memory, sizeof(char), bitmap_size, file);

    // Crear un bitarray con el bloque de memoria
    t_bitarray *bitarray = bitarray_create_with_mode(bitarray_memory, bitmap_size, LSB_FIRST);

    // Buscar el primer bloque libre
    uint32_t first_free_block = -1;
    size_t max_bits = bitarray_get_max_bit(bitarray);
    for (size_t i = 0; i < max_bits; i++) {
        if (!bitarray_test_bit(bitarray, i)) {
            first_free_block = i;
            break;
        }
    }

    // Limpiar recursos
    bitarray_destroy(bitarray);
    free(bitarray_memory);
    fclose(file);

    return first_free_block;
}