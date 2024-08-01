#include "comportamientos_dialfs.h"


//===============================================
// FUNCIONES DE MANEJO DE ARCHIVOS
//===============================================

bool crear_archivo_dialfs(char* nombre) {
    return crear_archivo(dialfs, nombre);
}

bool eliminar_archivo_dialfs(char* nombre) {
    return eliminar_archivo(dialfs, nombre);
}

bool truncar_archivo_dialfs(char* nombre, uint32_t nuevo_tamanio) {
    // return truncar_archivo(dialfs, nombre, nuevo_tamanio);
    return true;
}

bool escribir_archivo_dialfs(char* nombre, void* datos, uint32_t tamanio, uint32_t offset) {
    // return escribir_archivo(dialfs, nombre, datos, tamanio, offset);
    return true;
}

bool leer_archivo_dialfs(char* nombre, void* buffer, uint32_t tamanio, uint32_t offset) {
    // return leer_archivo(dialfs, nombre, buffer, tamanio, offset);
    return true;
}

//===============================================
// FUNCIONES DE MANTENIMIENTO
//===============================================

void compactar_fs_dialfs() {
    // compactar_fs(dialfs);
}

//===============================================
// FUNCIONES DE INICIALIZACIÓN Y DESTRUCCIÓN
//===============================================

bool file_exists(const char *filename)
{
    struct stat buffer;
    return (stat(filename, &buffer) == 0);
}

t_dialfs *crear_dialfs(char *path_base, uint32_t block_size, uint32_t block_count, uint32_t retraso_compactacion)
{
    t_dialfs *fs = malloc(sizeof(t_dialfs));         // Crear instancia de DialFS
    fs->path_base = strdup(path_base);               // Copiar ruta base
    fs->block_size = block_size;                     // Asignar tamaño de bloque
    fs->block_count = block_count;                   // Asignar cantidad de bloques
    fs->retraso_compactacion = retraso_compactacion; // Asignar retraso de compactación
    fs->path_bloques = string_from_format("%s/bloques.dat", path_base);
    fs->path_bitmap = string_from_format("%s/bitmap.dat", path_base);

    if (!file_exists(get_path_bloques(fs)))
    {
        crear_archivo_bloques(get_path_bloques(fs), block_size, block_count);
    }

    //Validar si existe o no el archivo, caso de no existir se debe crear
    if (!file_exists(get_path_bitmap(fs)))
    {
        create_bitmap(get_path_bitmap(fs), block_count); // crea archivo bitmap
    }

    fs->archivos = list_create(); // Crear lista de archivos

    // Levantar archivos desde metadata si existen
    levantar_archivos_desde_metadata(fs);
    
    return fs;
}

void destruir_dialfs(t_dialfs *fs)
{
    free(fs->path_base);
    // bitarray_destroy(fs->bitmap);
    list_destroy_and_destroy_elements(fs->archivos, (void *)destruir_archivo_dialfs);

    free(fs);
}

//===============================================
// FUNCIONES DE MANEJO DE ARCHIVOS
//===============================================

bool crear_archivo(t_dialfs *fs, char *nombre)
{
    t_archivo_dialfs *nuevo_archivo = malloc(sizeof(t_archivo_dialfs)); // Crear nuevo archivo
    nuevo_archivo->nombre = my_strdup(nombre);                          // Copiar nombre

    char *path_archivo = string_from_format("%s/", get_path_base(fs));
    string_append(&path_archivo, nombre);
    nuevo_archivo->path_archivo = path_archivo;

    uint32_t bloque_libre = find_first_free_block(get_path_bitmap(fs)); // Buscar bloque libre

    if (bloque_libre == (uint32_t)-1)
    {                                // Verificar si hay espacio
        free(nuevo_archivo->nombre); // Liberar memoria
        free(nuevo_archivo);         // Liberar memoria
        return false;
    }

    list_add(fs->archivos, nuevo_archivo); // Agregar archivo a la lista de archivos del FS

    set_block_as_used(get_path_bitmap(fs), bloque_libre);

    create_metadata(get_path_archivo(nuevo_archivo), bloque_libre);

    FILE *file = fopen(path_archivo, "wb"); // crear archivo con su nombre
    if (file == NULL)
    {
        perror("No se pudo crear el archivo de metadata");
        return false;
    }

    fclose(file); // Cerrar archivo
    free(path_archivo);
    // free(split_nombre);
    return true;
}

bool eliminar_archivo(t_dialfs *fs, char *nombre) {
    t_archivo_dialfs *archivo = buscar_archivo(fs, nombre);
    if (archivo == NULL) {
        log_error(logger_entradasalida, "Archivo %s no encontrado en el sistema de archivos.", nombre);
        return false;
    }

    // Obtener el bloque inicial del archivo
    uint32_t bloque_inicial = get_bloque_inicial_archivo(fs, nombre);
    if (bloque_inicial == (uint32_t)-1) {
        return false;
    }

    // Obtener el tamaño del archivo
    uint32_t tamanio_archivo = get_tamanio_archivo(fs, nombre);
    if (tamanio_archivo == (uint32_t)-1) {
        return false;
    }

    // Calcular el número de bloques necesarios
    uint32_t bloques_necesarios = (tamanio_archivo + fs->block_size - 1) / fs->block_size;

    // Liberar los bloques en el bitmap
    for (uint32_t i = 0; i < bloques_necesarios; i++) {
        set_block_as_free(fs->path_bitmap, bloque_inicial + i);
    }

    // Eliminar el archivo de datos
    if (remove(archivo->path_archivo) != 0) {
        perror("No se pudo eliminar el archivo de datos");
        return false;
    }


    // Remover el archivo de la lista de archivos
    //list_remove_and_destroy_element(fs->archivos, archivo, (void *)free_archivo_dialfs);

    //TODO: ELIMINAR EL ARCHIVO DE LA LISTA DE ARCHIVOS Y CHEQUEAR EL BITMAP

    return true;
}


void destruir_archivo_dialfs(t_archivo_dialfs *archivo)
{
    if (archivo != NULL)
    {
        free(archivo->nombre);
        free(archivo);
    }
}

char *get_path_bitmap(t_dialfs *fs)
{
    if (fs == NULL)
    {
        return NULL;
    }
    return fs->path_bitmap;
}

// Getter para path_bloques
char *get_path_bloques(t_dialfs *fs)
{
    if (fs == NULL)
    {
        return NULL;
    }
    return fs->path_bloques;
}

char *get_path_base(t_dialfs *fs)
{
    if (fs == NULL)
    {
        return NULL;
    }
    return fs->path_base;
}


char *get_path_archivo(t_archivo_dialfs *archivo)
{
    if (archivo == NULL)
    {
        return NULL;
    }
    return archivo->path_archivo;
}


