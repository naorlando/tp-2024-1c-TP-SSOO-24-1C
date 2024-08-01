#include "metadata.h"

void create_metadata(char *path, uint32_t initial_block)
{
    FILE *file = fopen(path, "wb");
    if (file == NULL)
    {
        perror("No se pudo crear el archivo de metadata");
        return;
    }

    fprintf(file, "BLOQUE_INICIAL=%u\n", initial_block);
    fprintf(file, "TAMANIO_ARCHIVO=0");

    fclose(file); // Cerrar archivo
}

t_config * read_metadata_(char *path)
{
    t_config *metadata_config = config_create(path);

    return metadata_config;
}

void write_metadata(char *path, uint32_t bloque_inicial, uint32_t tamano_archivo) {
    FILE *file = fopen(path, "wb");
    if (file == NULL) {
        perror("No se pudo abrir el archivo de metadata para actualizar");
        return;
    }

    fprintf(file, "BLOQUE_INICIAL=%u\n", bloque_inicial);
    fprintf(file, "TAMANIO_ARCHIVO=%u\n", tamano_archivo);

    fclose(file); // Cerrar archivo
}

uint32_t bloque_inicial(char *path){

    t_config* metadata = read_metadata_(path);
    return config_get_int_value(metadata, "BLOQUE_INICIAL");

}

uint32_t bloque_tamano_archivo(char *path){

    t_config* metadata = read_metadata_(path);
    return config_get_int_value(metadata, "TAMANIO_ARCHIVO");

}

