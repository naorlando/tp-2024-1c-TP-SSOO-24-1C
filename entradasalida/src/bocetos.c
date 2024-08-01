//===============================================
// FUNCIONES DE LECTURA Y ESCRITURA DE MEMORIA
//===============================================

/* EMPIEZO COMENTARIO de funciones de lectura y escritura de memoria
Modificar cuando se implemente la memoria

void* obtener_datos_de_memoria(uint32_t direccion_logica, uint32_t tamanio) {
    void* datos = malloc(tamanio);
    if (datos == NULL) {
        log_error(logger_entradasalida, "Error al asignar memoria para obtener datos");
        return NULL;
    }

    uint32_t pagina_inicial = direccion_logica / dialfs->block_size;
    uint32_t offset_inicial = direccion_logica % dialfs->block_size;
    uint32_t bytes_copiados = 0;

    while (bytes_copiados < tamanio) {
        uint32_t pagina_actual = pagina_inicial + bytes_copiados / dialfs->block_size;
        uint32_t offset_actual = (bytes_copiados == 0) ? offset_inicial : 0;
        uint32_t bytes_restantes = tamanio - bytes_copiados;
        uint32_t bytes_en_pagina = dialfs->block_size - offset_actual;
        uint32_t bytes_a_copiar = (bytes_restantes < bytes_en_pagina) ? bytes_restantes : bytes_en_pagina;

        t_entrada_tabla_de_paginas* entrada = get_page_data(dialfs->pid_actual, pagina_actual);
        if (entrada == NULL || !entrada->presencia) {
            log_error(logger_entradasalida, "Error: página no presente en memoria");
            free(datos);
            return NULL;
        }

        void* direccion_fisica = get_memory_address(entrada->frame, offset_actual);
        memcpy(datos + bytes_copiados, direccion_fisica, bytes_a_copiar);

        bytes_copiados += bytes_a_copiar;
    }

    return datos;
}


bool escribir_datos_en_memoria(uint32_t direccion_logica, void* datos, uint32_t tamanio) {
    uint32_t pagina_inicial = direccion_logica / dialfs->block_size;
    uint32_t offset_inicial = direccion_logica % dialfs->block_size;
    uint32_t bytes_escritos = 0;

    while (bytes_escritos < tamanio) {
        uint32_t pagina_actual = pagina_inicial + bytes_escritos / dialfs->block_size;
        uint32_t offset_actual = (bytes_escritos == 0) ? offset_inicial : 0;
        uint32_t bytes_restantes = tamanio - bytes_escritos;
        uint32_t bytes_en_pagina = dialfs->block_size - offset_actual;
        uint32_t bytes_a_escribir = (bytes_restantes < bytes_en_pagina) ? bytes_restantes : bytes_en_pagina;

        t_entrada_tabla_de_paginas* entrada = get_page_data(dialfs->pid_actual, pagina_actual);
        if (entrada == NULL || !entrada->presencia) {
            log_error(logger_entradasalida, "Error: página no presente en memoria");
            return false;
        }

        void* direccion_fisica = get_memory_address(entrada->frame, offset_actual);
        memcpy(direccion_fisica, datos + bytes_escritos, bytes_a_escribir);

        bytes_escritos += bytes_a_escribir;
    }

    return true;
}
TERMINO COMENTARIO*/



// bool truncar_archivo(t_dialfs *fs, char *nombre, uint32_t nuevo_tamanio)
// {
//     t_archivo_dialfs *archivo = buscar_archivo(fs, nombre); // Buscar archivo
//     if (archivo == NULL)
//         return false; // Verificar si existe

//     if (nuevo_tamanio > archivo->tamanio)
//     {                                                                                            // Verificar si se debe ampliar
//         uint32_t espacio_necesario = nuevo_tamanio - archivo->tamanio;                           // Calcular espacio necesario
//         uint32_t bloques_necesarios = (espacio_necesario + fs->block_size - 1) / fs->block_size; // Calcular bloques necesarios

//         if (!hay_espacio_contiguo(fs, bloques_necesarios))
//         {                     // Verificar si hay espacio contiguo
//             compactar_fs(fs); // Compactar FS si no hay espacio contiguo
//         }

//         if (!ampliar_archivo(fs, archivo, nuevo_tamanio))
//         { // Ampliar archivo
//             return false;
//         }
//     }
//     else if (nuevo_tamanio < archivo->tamanio)
//     {                                                                                                                                                            // Verificar si se debe reducir
//         liberar_bloques(fs, archivo->bloque_inicial + nuevo_tamanio / fs->block_size, (archivo->tamanio - nuevo_tamanio + fs->block_size - 1) / fs->block_size); // Liberar bloques
//     }

//     archivo->tamanio = nuevo_tamanio; // Actualizar tamaño
//     // guardar_metadata_archivo(fs, archivo);
//     // guardar_estado_fs(fs);
//     return true;
// }

// bool escribir_archivo(t_dialfs *fs, char *nombre, void *datos, uint32_t tamanio, uint32_t offset)
// {
//     t_archivo_dialfs *archivo = buscar_archivo(fs, nombre); // Buscar archivo
//     if (archivo == NULL)
//         return false; // Verificar si existe

//     if (offset + tamanio > archivo->tamanio)
//     { // Verificar si se debe truncar, en el caso que se escriba más allá del tamaño actual
//         if (!truncar_archivo(fs, nombre, offset + tamanio))
//         { // Truncar archivo
//             return false;
//         }
//     }

//     uint32_t bloque_inicio = archivo->bloque_inicial + offset / fs->block_size; // Calcular bloque de inicio
//     uint32_t offset_bloque = offset % fs->block_size;                           // Calcular offset en bloque

//     char *bloques_path = string_from_format("%s/bloques.dat", fs->path_base); // Crear ruta de bloques
//     FILE *file = fopen(bloques_path, "r+b");                                  // Abrir archivo de bloques
//     free(bloques_path);

//     uint32_t bytes_escritos = 0; // Inicializar bytes escritos
//     while (bytes_escritos < tamanio)
//     {                                                                                                                          // Escribir datos en bloques
//         uint32_t bytes_en_bloque = fs->block_size - offset_bloque;                                                             // Calcular bytes en bloque
//         uint32_t bytes_a_escribir = (tamanio - bytes_escritos < bytes_en_bloque) ? tamanio - bytes_escritos : bytes_en_bloque; // Calcular bytes a escribir

//         fseek(file, bloque_inicio * fs->block_size + offset_bloque, SEEK_SET); // Mover cursor a posición
//         fwrite(datos + bytes_escritos, 1, bytes_a_escribir, file);             // Escribir datos en bloque

//         bytes_escritos += bytes_a_escribir; // Actualizar bytes escritos
//         bloque_inicio++;                    // Actualizar bloque de inicio
//         offset_bloque = 0;                  // Reiniciar offset en bloque
//     }

//     fclose(file);
//     return true;
// }

// bool leer_archivo(t_dialfs *fs, char *nombre, void *buffer, uint32_t tamanio, uint32_t offset)
// {
//     t_archivo_dialfs *archivo = buscar_archivo(fs, nombre); // Buscar archivo
//     if (archivo == NULL)
//         return false; // Verificar si existe

//     if (offset >= archivo->tamanio)
//         return false; // Verificar si se puede leer

//     uint32_t tamanio_real = (offset + tamanio > archivo->tamanio) ? (archivo->tamanio - offset) : tamanio; // Calcular tamanio real
//     uint32_t bloque_inicio = archivo->bloque_inicial + offset / fs->block_size;                            // Calcular bloque de inicio
//     uint32_t offset_bloque = offset % fs->block_size;                                                      // Calcular offset en bloque

//     char *bloques_path = string_from_format("%s/bloques.dat", fs->path_base); // Crear ruta de bloques
//     FILE *file = fopen(bloques_path, "rb");                                   // Abrir archivo de bloques
//     free(bloques_path);                                                       // Liberar memoria

//     uint32_t bytes_leidos = 0; // Inicializar bytes leídos
//     while (bytes_leidos < tamanio_real)
//     {                                                                                                                            // Leer datos de bloques
//         uint32_t bytes_en_bloque = fs->block_size - offset_bloque;                                                               // Calcular bytes en bloque
//         uint32_t bytes_a_leer = (tamanio_real - bytes_leidos < bytes_en_bloque) ? tamanio_real - bytes_leidos : bytes_en_bloque; // Calcular bytes a leer

//         fseek(file, bloque_inicio * fs->block_size + offset_bloque, SEEK_SET); // Mover cursor a posición
//         fread(buffer + bytes_leidos, 1, bytes_a_leer, file);                   // Leer datos de bloque

//         bytes_leidos += bytes_a_leer; // Actualizar bytes leídos
//         bloque_inicio++;              // Actualizar bloque de inicio
//         offset_bloque = 0;            // Reiniciar offset en bloque
//     }

//     fclose(file);
//     return true;
// }

//===============================================
// FUNCIONES DE MANTENIMIENTO
//===============================================

// void compactar_fs(t_dialfs *fs)
// {
//     list_sort(fs->archivos, (void *)comparar_bloques_iniciales); // Ordenar archivos por bloque inicial

//     uint32_t bloque_actual = 0; // Inicializar bloque actual
//     for (int i = 0; i < list_size(fs->archivos); i++)
//     {                                                          // Recorrer lista de archivos
//         t_archivo_dialfs *archivo = list_get(fs->archivos, i); // Obtener archivo
//         if (archivo->bloque_inicial != bloque_actual)
//         {                                                                                                                        // Verificar si es necesario mover bloques, si no están contiguos
//             mover_bloques(fs, archivo->bloque_inicial, bloque_actual, (archivo->tamanio + fs->block_size - 1) / fs->block_size); // Mover bloques
//             archivo->bloque_inicial = bloque_actual;                                                                             // Actualizar bloque inicial
//         }
//         bloque_actual += (archivo->tamanio + fs->block_size - 1) / fs->block_size; // Actualizar bloque actual
//     }

//     for (uint32_t i = bloque_actual; i < fs->block_count; i++)
//     { // Liberar bloques restantes
//       // bitarray_clean_bit(fs->bitmap, i); // Liberar bloque
//     }

//     usleep(fs->retraso_compactacion * 1000); // Convertir a microsegundos

//     //    guardar_estado_fs(fs);
// }

// void guardar_estado_fs(t_dialfs *fs)
// {
//     char *bitmap_path = string_from_format("%s/bitmap.dat", fs->path_base); // Crear ruta de bitmap desde base
//     FILE *bitmap_file = fopen(bitmap_path, "wb");                           // Abrir archivo de bitmap

//    // fwrite(fs->bitmap->bitarray, 1, fs->bitmap->size, bitmap_file); // Escribir bitmap en archivo
//     fclose(bitmap_file);
//     free(bitmap_path);

//     for (int i = 0; i < list_size(fs->archivos); i++)
//     { // Recorrer lista de archivos, y guardar metadata de cada uno
//         t_archivo_dialfs *archivo = list_get(fs->archivos, i);
//         guardar_metadata_archivo(fs, archivo);
//     }
// }

// void cargar_estado_fs(t_dialfs *fs)
// {
//     DIR *dir = opendir(fs->path_base); // Abrir directorio base
//     struct dirent *entry;              // Crear entrada de directorio
//     while ((entry = readdir(dir)) != NULL)
//     { // Recorrer directorio
//         if (strstr(entry->d_name, ".metadata") != NULL)
//         {                                               // Verificar si es un archivo de metadata
//             cargar_metadata_archivo(fs, entry->d_name); // Cargar metadata de archivo
//         }
//     }
//     closedir(dir); // Cerrar directorio
// }

//===============================================
// FUNCIONES AUXILIARES
//===============================================

// void liberar_bloques(t_dialfs *fs, uint32_t bloque_inicial, uint32_t num_bloques)
// {
//     for (uint32_t i = 0; i < num_bloques; i++)
//     {
//         //   bitarray_clean_bit(fs->bitmap, bloque_inicial + i);
//     }
// }

// t_archivo_dialfs *buscar_archivo(t_dialfs *fs, char *nombre)
// {
//     for (int i = 0; i < list_size(fs->archivos); i++)
//     {
//         t_archivo_dialfs *archivo = list_get(fs->archivos, i);
//         if (strcmp(archivo->nombre, nombre) == 0)
//         {
//             return archivo;
//         }
//     }
//     return NULL;
// }

// bool hay_espacio_contiguo(t_dialfs *fs, uint32_t bloques_necesarios)
// {
//     uint32_t bloques_contiguos = 0;
//     for (uint32_t i = 0; i < fs->block_count; i++)
//     {
//         // if (!bitarray_test_bit(fs->bitmap, i))
//         // {
//         //     bloques_contiguos++;
//         //     if (bloques_contiguos == bloques_necesarios)
//         //     {
//         //         return true;
//         //     }
//         // }
//         // else
//         // {
//         //     bloques_contiguos = 0;
//         // }
//     }
//     return false;
// }

// bool ampliar_archivo(t_dialfs *fs, t_archivo_dialfs *archivo, uint32_t nuevo_tamanio)
// {
//     uint32_t bloques_actuales = (archivo->tamanio + fs->block_size - 1) / fs->block_size;
//     uint32_t bloques_necesarios = (nuevo_tamanio + fs->block_size - 1) / fs->block_size;
//     uint32_t bloques_adicionales = bloques_necesarios - bloques_actuales;

//     for (uint32_t i = 0; i < bloques_adicionales; i++)
//     {
//         // uint32_t nuevo_bloque = buscar_bloque_libre(fs);
//         // if (nuevo_bloque == (uint32_t)-1)
//         // {
//         //     return false;
//         // }
//         // bitarray_set_bit(fs->bitmap, nuevo_bloque);
//     }

//     return true;
// }

// int comparar_bloques_iniciales(t_archivo_dialfs *a, t_archivo_dialfs *b)
// {
//     return a->bloque_inicial - b->bloque_inicial;
// }


// char *get_path_metadata(t_archivo_dialfs *archivo)
// {
//     if (archivo == NULL)
//     {
//         return NULL;
//     }
//     return archivo->path_metadata;
// }


// void mover_bloques(t_dialfs *fs, uint32_t origen, uint32_t destino, uint32_t cantidad)
// {
//     char *bloques_path = string_from_format("%s/bloques.dat", fs->path_base);
//     FILE *file = fopen(bloques_path, "r+b");
//     free(bloques_path);

//     void *buffer = malloc(fs->block_size);

//     for (uint32_t i = 0; i < cantidad; i++)
//     {
//         fseek(file, (origen + i) * fs->block_size, SEEK_SET);
//         fread(buffer, 1, fs->block_size, file);

//         fseek(file, (destino + i) * fs->block_size, SEEK_SET);
//         fwrite(buffer, 1, fs->block_size, file);

//         // bitarray_clean_bit(fs->bitmap, origen + i);
//         // bitarray_set_bit(fs->bitmap, destino + i);
//     }

//     free(buffer);
//     fclose(file);
// }