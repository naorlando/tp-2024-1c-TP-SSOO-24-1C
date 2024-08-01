#include "archivos.h"




t_archivo_dialfs* buscar_archivo(t_dialfs *fs, char *nombre_archivo) {
    bool archivo_match(void *archivo_ptr) {
        t_archivo_dialfs *archivo = (t_archivo_dialfs *)archivo_ptr;
        return strcmp(archivo->nombre, nombre_archivo) == 0;
    }

    return list_find(fs->archivos, archivo_match);
}


uint32_t get_bloque_inicial_archivo(t_dialfs *fs, char *nombre_archivo) {
    t_archivo_dialfs *archivo = buscar_archivo(fs, nombre_archivo);
    if (archivo == NULL) {
        log_error(logger_entradasalida, "Archivo %s no encontrado en el sistema de archivos.", nombre_archivo);
        return (uint32_t)-1;
    }

    // Abrir el archivo de metadata para leer
    FILE *metadata_file = fopen(archivo->path_archivo, "r");
    if (metadata_file == NULL) {
        perror("No se pudo abrir el archivo de metadata");
        return (uint32_t)-1;
    }

    // Leer el bloque inicial del archivo
    uint32_t bloque_inicial;
    fscanf(metadata_file, "BLOQUE_INICIAL=%u\n", &bloque_inicial);
    fclose(metadata_file);

    return bloque_inicial;
}

uint32_t get_tamanio_archivo(t_dialfs *fs, char *nombre_archivo) {
    t_archivo_dialfs *archivo = buscar_archivo(fs, nombre_archivo);
    if (archivo == NULL) {
        log_error(logger_entradasalida, "Archivo %s no encontrado en el sistema de archivos.", nombre_archivo);
        return (uint32_t)-1;
    }

    // Abrir el archivo de metadata para leer
    FILE *metadata_file = fopen(archivo->path_archivo, "r");
    if (metadata_file == NULL) {
        perror("No se pudo abrir el archivo de metadata");
        return (uint32_t)-1;
    }

    // Leer el tamaño del archivo
    uint32_t tamanio_archivo;
    fscanf(metadata_file, "BLOQUE_INICIAL=%*u\nTAMANIO_ARCHIVO=%u\n", &tamanio_archivo);
    fclose(metadata_file);

    return tamanio_archivo;
}




t_list* listar_archivos_metadata_en_directorio(t_dialfs* fs) {
    DIR* dir;
    struct dirent* entry;
    t_list* lista_archivos = list_create();

    if ((dir = opendir(get_path_base(fs))) == NULL) {
        perror("No se pudo abrir el directorio");
        return NULL;
    }

    // Obtener los paths absolutos de bitmap y bloques
    char* path_bitmap = get_path_bitmap(fs);
    char* path_bloques = get_path_bloques(fs);

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) { // Asegurarse de que es un archivo regular
            // Obtener el path absoluto del archivo actual
            char* path_archivo = string_from_format("%s/%s", get_path_base(fs), entry->d_name);

            // Excluir los archivos que coinciden con los paths de bitmap y bloques
            if (strcmp(path_archivo, path_bitmap) != 0 && strcmp(path_archivo, path_bloques) != 0) { 
                    list_add(lista_archivos, strdup(entry->d_name));
            }

            free(path_archivo);
        }
    }

    closedir(dir);
    return lista_archivos;
}

bool levantar_metadata(t_dialfs *fs, char *nombre_metadata) {
    t_archivo_dialfs *archivo_existente = malloc(sizeof(t_archivo_dialfs)); // Crear estructura para el archivo existente

    // Extraer el nombre del archivo sin la extensión .metadata
    archivo_existente->nombre = my_strdup(nombre_metadata); // Copiar nombre del archivo

    char *path_archivo = string_from_format("%s/%s", get_path_base(fs), nombre_metadata);
    archivo_existente->path_archivo = my_strdup(path_archivo);

    list_add(fs->archivos, archivo_existente); // Agregar archivo a la lista de archivos del FS
    
    free(path_archivo);
    return true;
}

void levantar_archivos_desde_metadata(t_dialfs* fs) {
    t_list* archivos_metadata = listar_archivos_metadata_en_directorio(fs);

    if (archivos_metadata == NULL) {
        log_error(logger_entradasalida, "No se pudo obtener la lista de archivos metadata");
        list_destroy(archivos_metadata);
        return;
    }

    for (int i = 0; i < list_size(archivos_metadata); i++) {
        char* nombre_metadata = list_get(archivos_metadata, i);
        
        // Levantar la metadata del archivo existente
        if (!levantar_metadata(fs, nombre_metadata)) {
            log_warning(logger_entradasalida, "No se pudo levantar la metadata del archivo %s", nombre_metadata);
        }

        free(nombre_metadata);
    }

    list_destroy(archivos_metadata);
}