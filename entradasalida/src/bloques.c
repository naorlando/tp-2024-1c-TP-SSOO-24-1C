#include "bloques.h"



void crear_archivo_bloques(const char* path, uint32_t block_size, uint32_t block_count) {
    FILE* file = fopen(path, "wb");
    if (file == NULL) {
        perror("No se pudo crear el archivo de bloques");
        return;
    }
    
    char* block = calloc(block_size, 1); // Inicializar en 0
    for (uint32_t i = 0; i < block_count; i++) { // Escribir bloques vacíos
        fwrite(block, 1, block_size, file);
    }
    
    free(block); // Liberar memoria
    fclose(file); // Cerrar archivo
}
// Función para leer bytes de bloques.dat
void leer_bloque(uint32_t puntero, void *buffer, uint32_t bytes_a_leer, char *bloques_path) {
    FILE *file = fopen(bloques_path, "rb");
    if (file == NULL) {
        perror("Failed to open file for reading");
        return;
    }

    // Mover el puntero del archivo a la ubicación especificada
    fseek(file, puntero, SEEK_SET);

    // Leer los bytes especificados desde el archivo al buffer
    size_t bytes_read = fread(buffer, 1, bytes_a_leer, file);
    if (bytes_read != bytes_a_leer) {
        perror("Failed to read the specified number of bytes");
    }

    fclose(file);
}

// Función para escribir bytes en bloques.dat
void escribir_bloque(uint32_t puntero, uint32_t bytes_a_escribir, void *buffer, char *bloques_path) {
    FILE *file = fopen(bloques_path, "rb+");
    if (file == NULL) {
        perror("Failed to open file for writing");
        return;
    }

    // Mover el puntero del archivo a la ubicación especificada
    fseek(file, puntero, SEEK_SET);

    // Escribir los bytes especificados desde el buffer al archivo
    size_t bytes_written = fwrite(buffer, 1, bytes_a_escribir, file);
    if (bytes_written != bytes_a_escribir) {
        perror("Failed to write the specified number of bytes");
    }

    fclose(file);
}