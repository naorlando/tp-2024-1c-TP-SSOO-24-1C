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
    return truncar_archivo(dialfs, nombre, nuevo_tamanio);
    //return true;
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
    nuevo_archivo->path_archivo = my_strdup(path_archivo);

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

    free(path_archivo);

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


bool truncar_archivo(t_dialfs *fs, char *nombre, uint32_t nuevo_tamanio) {
    t_archivo_dialfs *archivo = buscar_archivo(fs, nombre);
    if (archivo == NULL) {
        log_error(logger_entradasalida, "Archivo %s no encontrado en el sistema de archivos.", nombre);
        return false;
    }

    uint32_t tamanio_actual = tamano_archivo(archivo->path_archivo);
    if (tamanio_actual == (uint32_t)-1) {
        return false;
    }

    if (nuevo_tamanio == tamanio_actual) {
        log_info(logger_entradasalida, "El tamaño solicitado es igual al actual. No se requiere truncado.");
        return true;
    }

    uint32_t bloque_inicial = bloque_inicial_archivo(archivo->path_archivo);
    if (bloque_inicial == (uint32_t)-1) {
        return false;
    }

    uint32_t bloques_actuales = (tamanio_actual > 0) ? ((tamanio_actual + fs->block_size - 1) / fs->block_size) : 1; 
    uint32_t nuevos_bloques = (nuevo_tamanio + fs->block_size - 1) / fs->block_size;

    if (nuevos_bloques < bloques_actuales) { // Caso cuando se quiere achicar el archivo
        for (uint32_t i = nuevos_bloques; i < bloques_actuales; i++) {
            set_block_as_free(fs->path_bitmap, bloque_inicial + i);
        }

        //TODO: Se debe compactar en este caso
    } else if (nuevos_bloques > bloques_actuales) { // Caso cuando se quiere agrandar el archivo
        uint32_t bloque_final = bloque_inicial + bloques_actuales - 1;
        uint32_t siguiente_bloque = bloque_final + 1;

        // Verificar si los bloques después del archivo están libres
        bool espacio_contiguo = true;
        for (uint32_t i = siguiente_bloque; i < bloque_final + (nuevos_bloques - bloques_actuales) + 1; i++) {
            if (is_block_used(fs->path_bitmap, i)) {
                espacio_contiguo = false;
                break;
            }
        }

        if (espacio_contiguo) {
            for (uint32_t i = bloques_actuales; i < nuevos_bloques; i++) {
                set_block_as_used(fs->path_bitmap, bloque_inicial + i);
            }
        } else {
            // Compactar los bloques para crear espacio contiguo
            if (!compactar(fs,archivo, bloque_inicial, tamanio_actual)) {
                log_error(logger_entradasalida, "No se pudo compactar el archivo %s.", nombre);
                return false;
            }
            uint32_t nuevo_bloque_inicial = bloque_inicial_archivo(archivo->path_archivo);
            if (nuevo_bloque_inicial == (uint32_t)-1) {
                return false;
            } 
            
            write_metadata(archivo->path_archivo, nuevo_bloque_inicial, nuevo_tamanio);


            for (uint32_t i = bloques_actuales; i < nuevos_bloques; i++) {
                set_block_as_used(fs->path_bitmap, nuevo_bloque_inicial + i);
            }
                         
        }
    }



    // Actualizar el tamaño en la metadata del archivo
    // FILE *metadata_file = fopen(archivo->path_archivo, "r+");
    // if (metadata_file == NULL) {
    //     perror("No se pudo abrir el archivo de metadata");
    //     return false;
    // }

    // fprintf(metadata_file, "TAMANIO_ARCHIVO=%u\n", nuevo_tamanio);
    // fclose(metadata_file);

    return true;
}

bool compactar(t_dialfs *fs,t_archivo_dialfs *archivo , uint32_t bloque_inicial, uint32_t tamanio_actual) {

    usleep(fs->retraso_compactacion * 1000);
    // Almaceno en un buffer el contenido del archivo a truncar
    void* buffer = contenido_archivo_truncar(fs, bloque_inicial, tamanio_actual);
    uint32_t primer_bloque_arc_sig = bloque_inicial + (tamanio_actual / fs->block_size) + 1;

    uint32_t primer_bloque_libre = find_first_free_block(fs->path_bitmap);
    
    if (primer_bloque_libre == (uint32_t)-1) {
        log_error(logger_entradasalida, "No hay bloques libres disponibles para compactar.");
        return false;
    }

    uint32_t bloque_a_pegar = bloque_inicial;
    uint32_t cant_bloques_movidos = 0;

    while (primer_bloque_arc_sig < primer_bloque_libre) {
        // Llamo a la función mover_bloques_archivo
        cant_bloques_movidos = mover_bloques_archivo(fs, bloque_a_pegar, primer_bloque_arc_sig);
        if (cant_bloques_movidos == 0) {
            log_error(logger_entradasalida, "Error al mover bloques del archivo.");
            return false;
        }

        primer_bloque_arc_sig += cant_bloques_movidos + 1;
        bloque_a_pegar += cant_bloques_movidos + 1;
    }

    bloque_a_pegar--;

    uint32_t bloques_actuales = (tamanio_actual + fs->block_size - 1) / fs->block_size;

    copiar_bloque_desde_buffer(fs, buffer, bloque_a_pegar, bloques_actuales * fs->block_size, fs->block_size);
    
    //ACTUALIZAR METADATA ARCHIVO A TRUNCAR
    write_metadata(archivo->path_archivo, bloque_a_pegar, tamanio_actual);

    free(buffer);

    return true;
}

bool copiar_bloques(t_dialfs *fs, uint32_t bloque_origen, uint32_t bloque_destino, uint32_t tamanio) {
    FILE *bloques_file = fopen(fs->path_bloques, "rb+");
    if (bloques_file == NULL) {
        perror("No se pudo abrir el archivo de bloques");
        return false;
    }

    void *buffer = malloc(tamanio);
    if (buffer == NULL) {
        perror("No se pudo asignar memoria para el buffer");
        fclose(bloques_file);
        return false;
    }

    // Leer los bloques origen
    fseek(bloques_file, bloque_origen * fs->block_size, SEEK_SET);
    fread(buffer, tamanio, 1, bloques_file);

    // Escribir en los bloques destino
    fseek(bloques_file, bloque_destino * fs->block_size, SEEK_SET);
    fwrite(buffer, tamanio, 1, bloques_file);

    free(buffer);
    fclose(bloques_file);
    return true;
}

uint32_t mover_bloques_archivo(t_dialfs* fs, uint32_t primer_bloque_arc_ant, uint32_t primer_bloque_arc_sig) {
    t_archivo_dialfs* archivo_actual = obtener_archivo_por_bloque_inicial(fs, primer_bloque_arc_sig);
    if (archivo_actual == NULL) {
        log_error(logger_entradasalida, "No se encontró el archivo asociado con el bloque inicial %u.", primer_bloque_arc_sig);
        return 0;
    }

    uint32_t tamanio = tamano_archivo(archivo_actual->path_archivo);

    uint32_t bloques_necesarios = (tamanio > 0) ? ((tamanio + fs->block_size - 1) / fs->block_size) : 1;

    // Utilizar la función copiar_bloques
    if (!copiar_bloques(fs, primer_bloque_arc_sig, primer_bloque_arc_ant, tamanio)) {
        log_error(logger_entradasalida, "Error al copiar bloques del archivo.");
        return 0;
    }

    // Actualizar el bitmap
    // for (uint32_t i = 0; i < bloques_necesarios; i++) {
    //     set_block_as_free(fs->path_bitmap, primer_bloque_arc_sig + i);
    //     set_block_as_used(fs->path_bitmap, primer_bloque_arc_ant + i);
    // }

    // Actualizar la metadata del archivo
    write_metadata(archivo_actual->path_archivo, primer_bloque_arc_ant, tamanio);

    return bloques_necesarios;
}

void* contenido_archivo_truncar(t_dialfs *fs, uint32_t bloque_inicial, uint32_t tamanio_actual) {
    FILE *bloques_file = fopen(fs->path_bloques, "rb");
    if (bloques_file == NULL) {
        perror("No se pudo abrir el archivo de bloques");
        return NULL;
    }

    size_t block_size = fs->block_size;
    uint32_t bloques_necesarios = (tamanio_actual + block_size - 1) / block_size;

    void *buffer = malloc(bloques_necesarios * block_size);
    if (buffer == NULL) {
        perror("No se pudo asignar memoria para el buffer");
        fclose(bloques_file);
        return NULL;
    }

    // Leer el contenido de los bloques del archivo a truncar
    for (uint32_t i = 0; i < bloques_necesarios; i++) {
        fseek(bloques_file, (bloque_inicial + i) * block_size, SEEK_SET);
        fread(buffer + i * block_size, block_size, 1, bloques_file);
    }

    fclose(bloques_file);
    return buffer;
}

void copiar_bloque_desde_buffer(t_dialfs *fs, void *buffer, uint32_t bloque_destino, uint32_t offset, size_t size) {
    FILE *bloques_file = fopen(fs->path_bloques, "rb+"); // Abre el archivo en modo lectura/escritura binario
    if (bloques_file == NULL) {
        perror("No se pudo abrir el archivo de bloques");
        return;
    }

    // Escribir desde el buffer en el bloque destino
    fseek(bloques_file, bloque_destino * fs->block_size, SEEK_SET);
    fwrite(buffer + offset, size, 1, bloques_file);

    fclose(bloques_file);
}

t_archivo_dialfs* obtener_archivo_por_bloque_inicial(t_dialfs *fs, uint32_t bloque_inicial) {
    void* archivo_encontrado = NULL;

    bool es_bloque_inicial(void* archivo_ptr) {
        t_archivo_dialfs* archivo = (t_archivo_dialfs*)archivo_ptr;
        t_config *metadata = config_create(archivo->path_archivo);

        if (metadata == NULL) {
            log_error(logger_entradasalida, "No se pudo abrir el archivo de metadata: %s", archivo->path_archivo);
            return false;
        }

        uint32_t bloque_inicial_archivo = config_get_int_value(metadata, "BLOQUE_INICIAL");
        config_destroy(metadata);

        return bloque_inicial_archivo == bloque_inicial;
    }

    archivo_encontrado = list_find(fs->archivos, es_bloque_inicial);

    if(archivo_encontrado == NULL) return NULL;

    return (t_archivo_dialfs*)archivo_encontrado;
}