#ifndef COMPORTAMIENTOS_DIALFS_H
#define COMPORTAMIENTOS_DIALFS_H

#include "variables_globales.h"
#include <stdint.h>
#include <stdbool.h>
#include <commons/bitarray.h>
#include <commons/collections/list.h>
#include <commons/string.h>
#include <commons/config.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // Librería para usleep
#include <sys/stat.h> // Librería para manejo de archivos
#include "utils/utils.h"
#include "bitmap.h"
#include "bloques.h"
#include "metadata.h"
#include "archivos.h"

//===============================================
// FUNCIONES DE MANEJO DE ARCHIVOS
//===============================================

/**
 * Crea un nuevo archivo en el sistema DialFS.
 * @param nombre Nombre del archivo a crear.
 * @return true si el archivo se creó exitosamente, false en caso contrario.
 */
bool crear_archivo_dialfs(char* nombre);

/**
 * Elimina un archivo del sistema DialFS.
 * @param nombre Nombre del archivo a eliminar.
 * @return true si el archivo se eliminó exitosamente, false en caso contrario.
 */
bool eliminar_archivo_dialfs(char* nombre);

/**
 * Modifica el tamaño de un archivo existente en DialFS.
 * @param nombre Nombre del archivo a modificar.
 * @param nuevo_tamanio Nuevo tamaño del archivo en bytes.
 * @return true si el archivo se truncó exitosamente, false en caso contrario.
 */
bool truncar_archivo_dialfs(char* nombre, uint32_t nuevo_tamanio);

/**
 * Escribe datos en un archivo existente en DialFS.
 * @param nombre Nombre del archivo en el que escribir.
 * @param datos Puntero a los datos a escribir.
 * @param tamanio Cantidad de bytes a escribir.
 * @param offset Posición en el archivo donde comenzar a escribir.
 * @return true si la escritura fue exitosa, false en caso contrario.
 */
// bool escribir_archivo_dialfs(char* nombre, void* datos, uint32_t tamanio, uint32_t offset);

/**
 * Lee datos de un archivo existente en DialFS.
 * @param nombre Nombre del archivo del que leer.
 * @param buffer Buffer donde se almacenarán los datos leídos.
 * @param tamanio Cantidad de bytes a leer.
 * @param offset Posición en el archivo donde comenzar a leer.
 * @return true si la lectura fue exitosa, false en caso contrario.
 */
// bool leer_archivo_dialfs(char* nombre, void* buffer, uint32_t tamanio, uint32_t offset);

//===============================================
// FUNCIONES DE MANTENIMIENTO
//===============================================

/**
 * Realiza la compactación del sistema de archivos DialFS.
 */
// void compactar_fs_dialfs();

//===============================================
// FUNCIONES DE LECTURA Y ESCRITURA DE MEMORIA
//===============================================

/**
 * Obtiene datos de la memoria para una operación de escritura.
 * @param direccion_logica Dirección lógica de inicio en la memoria.
 * @param tamanio Cantidad de bytes a obtener.
 * @return Puntero a los datos obtenidos de la memoria o NULL si hay un error.
 */
// void* obtener_datos_de_memoria(uint32_t direccion_logica, uint32_t tamanio);

/**
 * Escribe datos en la memoria después de una operación de lectura.
 * @param direccion_logica Dirección lógica de inicio en la memoria.
 * @param datos Puntero a los datos a escribir.
 * @param tamanio Cantidad de bytes a escribir.
 * @return true si la escritura fue exitosa, false en caso contrario.
 */
// bool escribir_datos_en_memoria(uint32_t direccion_logica, void* datos, uint32_t tamanio);


//===============================================
// FUNCIONES DE INICIALIZACIÓN Y DESTRUCCIÓN
//===============================================

/**
 * Verifica si un archivo existe.
 * @param filename Nombre del archivo a verificar.
 * @return true si el archivo existe, false en caso contrario.
 */
bool file_exists(const char *filename);

/**
 * Crea el archivo de bloques para el sistema de archivos.
 * @param path Ruta donde crear el archivo de bloques.
 * @param block_size Tamaño de cada bloque en bytes.
 * @param block_count Número total de bloques.
 */
void crear_archivo_bloques(const char* path, uint32_t block_size, uint32_t block_count);

/**
 * Crea el archivo de bitmap para el sistema de archivos.
 * @param path Ruta donde crear el archivo de bitmap.
 * @param block_count Número total de bloques.
 */
void crear_bitmap(const char* path, uint32_t block_count);

/**
 * Inicializa una nueva instancia de DialFS.
 * @param path_base Directorio base donde se almacenarán los archivos del FS.
 * @param block_size Tamaño de cada bloque en bytes.
 * @param block_count Número total de bloques en el FS.
 * @param retraso_compactacion Tiempo de espera después de la compactación en ms.
 * @return Puntero a la nueva instancia de t_dialfs o NULL si falla la inicialización.
 */
t_dialfs* crear_dialfs(char* path_base, uint32_t block_size, uint32_t block_count, uint32_t retraso_compactacion);

/**
 * Libera los recursos asociados con una instancia de DialFS.
 * @param fs Puntero a la instancia de t_dialfs a destruir.
 */
void destruir_dialfs(t_dialfs* fs);


//===============================================
// FUNCIONES DE MANEJO DE ARCHIVOS
//===============================================

/**
 * Crea un nuevo archivo en el sistema DialFS.
 * @param fs Puntero a la instancia de t_dialfs.
 * @param nombre Nombre del archivo a crear.
 * @return true si el archivo se creó exitosamente, false en caso contrario.
 */
bool crear_archivo(t_dialfs* fs, char* nombre);

/**
 * Elimina un archivo del sistema DialFS.
 * @param fs Puntero a la instancia de t_dialfs.
 * @param nombre Nombre del archivo a eliminar.
 * @return true si el archivo se eliminó exitosamente, false en caso contrario.
 */
bool eliminar_archivo(t_dialfs* fs, char* nombre);

/**
 * Modifica el tamaño de un archivo existente en DialFS.
 * @param fs Puntero a la instancia de t_dialfs.
 * @param nombre Nombre del archivo a modificar.
 * @param nuevo_tamanio Nuevo tamaño del archivo en bytes.
 * @return true si el archivo se truncó exitosamente, false en caso contrario.
 */
// bool truncar_archivo(t_dialfs* fs, char* nombre, uint32_t nuevo_tamanio);

/**
 * Escribe datos en un archivo existente en DialFS.
 * @param fs Puntero a la instancia de t_dialfs.
 * @param nombre Nombre del archivo en el que escribir.
 * @param datos Puntero a los datos a escribir.
 * @param tamanio Cantidad de bytes a escribir.
 * @param offset Posición en el archivo donde comenzar a escribir.
 * @return true si la escritura fue exitosa, false en caso contrario.
 */
// bool escribir_archivo(t_dialfs* fs, char* nombre, void* datos, uint32_t tamanio, uint32_t offset);

/**
 * Lee datos de un archivo existente en DialFS.
 * @param fs Puntero a la instancia de t_dialfs.
 * @param nombre Nombre del archivo del que leer.
 * @param buffer Buffer donde se almacenarán los datos leídos.
 * @param tamanio Cantidad de bytes a leer.
 * @param offset Posición en el archivo donde comenzar a leer.
 * @return true si la lectura fue exitosa, false en caso contrario.
 */
// bool leer_archivo(t_dialfs* fs, char* nombre, void* buffer, uint32_t tamanio, uint32_t offset);


//===============================================
// FUNCIONES DE MANTENIMIENTO
//===============================================

/**
 * Realiza la compactación del sistema de archivos DialFS.
 * @param fs Puntero a la instancia de t_dialfs.
 */
// void compactar_fs(t_dialfs* fs);

/**
 * Guarda el estado actual del sistema de archivos DialFS.
 * @param fs Puntero a la instancia de t_dialfs.
 */
// void guardar_estado_fs(t_dialfs* fs);

/**
 * Carga el estado del sistema de archivos DialFS.
 * @param fs Puntero a la instancia de t_dialfs.
 */
// void cargar_estado_fs(t_dialfs* fs);


//===============================================
// FUNCIONES AUXILIARES
//===============================================

/**
 * Busca un bloque libre en el sistema de archivos.
 * @param fs Puntero a la instancia de t_dialfs.
 * @return Número del primer bloque libre encontrado, o -1 si no hay bloques libres.
 */
// uint32_t buscar_bloque_libre(t_dialfs* fs);

/**
 * Libera los bloques ocupados por un archivo.
 * @param fs Puntero a la instancia de t_dialfs.
 * @param bloque_inicial Número del primer bloque a liberar.
 * @param num_bloques Número de bloques a liberar.
 */
// void liberar_bloques(t_dialfs* fs, uint32_t bloque_inicial, uint32_t num_bloques);

/**
 * Busca un archivo en el sistema DialFS.
 * @param fs Puntero a la instancia de t_dialfs.
 * @param nombre Nombre del archivo a buscar.
 * @return Puntero al t_archivo_dialfs si se encuentra, NULL en caso contrario.
 */
// t_archivo_dialfs* buscar_archivo(t_dialfs* fs, char* nombre);

/**
 * Verifica si hay espacio contiguo disponible en el sistema de archivos.
 * @param fs Puntero a la instancia de t_dialfs.
 * @param bloques_necesarios Número de bloques contiguos necesarios.
 * @return true si hay espacio contiguo disponible, false en caso contrario.
 */
// bool hay_espacio_contiguo(t_dialfs* fs, uint32_t bloques_necesarios);

/**
 * Amplía un archivo existente.
 * @param fs Puntero a la instancia de t_dialfs.
 * @param archivo Puntero al t_archivo_dialfs a ampliar.
 * @param nuevo_tamanio Nuevo tamaño del archivo en bytes.
 * @return true si la ampliación fue exitosa, false en caso contrario.
 */
// bool ampliar_archivo(t_dialfs* fs, t_archivo_dialfs* archivo, uint32_t nuevo_tamanio);

/**
 * Mueve bloques de una posición a otra en el sistema de archivos.
 * @param fs Puntero a la instancia de t_dialfs.
 * @param origen Número de bloque de origen.
 * @param destino Número de bloque de destino.
 * @param cantidad Cantidad de bloques a mover.
 */
// void mover_bloques(t_dialfs* fs, uint32_t origen, uint32_t destino, uint32_t cantidad);

/**
 * Guarda los metadatos de un archivo en el sistema DialFS.
 * @param fs Puntero a la instancia de t_dialfs.
 * @param archivo Puntero al t_archivo_dialfs cuyos metadatos se guardarán.
 */
// t_config*  guardar_metadata_archivo(t_dialfs* fs, t_archivo_dialfs* archivo);

/**
 * Carga los metadatos de un archivo en el sistema DialFS.
 * @param fs Puntero a la instancia de t_dialfs.
 * @param metadata_filename Nombre del archivo de metadatos a cargar.
 */
// void cargar_metadata_archivo(t_dialfs* fs, char* metadata_filename);

/**
 * Compara dos archivos por su bloque inicial para ordenarlos.
 * @param a Puntero al primer t_archivo_dialfs a comparar.
 * @param b Puntero al segundo t_archivo_dialfs a comparar.
 * @return Diferencia entre los bloques iniciales de los archivos.
 */
// int comparar_bloques_iniciales(t_archivo_dialfs* a, t_archivo_dialfs* b);

/**
 * Destruye una estructura t_archivo_dialfs y libera su memoria.
 * @param archivo Puntero al t_archivo_dialfs a destruir.
 */
void destruir_archivo_dialfs(t_archivo_dialfs* archivo);

char* get_path_bitmap(t_dialfs* fs);
char* get_path_bloques(t_dialfs* fs);
char* get_path_base(t_dialfs* fs);
// char* get_path_metadata(t_archivo_dialfs* archivo);
char* get_path_archivo(t_archivo_dialfs* archivo);

bool compactar(t_dialfs *fs,t_archivo_dialfs *archivo , uint32_t bloque_inicial, uint32_t tamanio_actual);
void copiar_bloque(t_dialfs *fs, uint32_t bloque_origen, uint32_t bloque_destino);
bool truncar_archivo(t_dialfs *fs, char *nombre, uint32_t nuevo_tamanio);
void* contenido_archivo_truncar(t_dialfs *fs, uint32_t bloque_inicial, uint32_t tamanio_actual);
uint32_t mover_bloques_archivo(t_dialfs* fs, uint32_t primer_bloque_arc_ant, uint32_t primer_bloque_arc_sig);
bool copiar_bloques(t_dialfs *fs, uint32_t bloque_origen, uint32_t bloque_destino, uint32_t tamanio);
void copiar_bloque_desde_buffer(t_dialfs *fs, void *buffer, uint32_t bloque_destino, uint32_t offset, size_t size);
t_archivo_dialfs* obtener_archivo_por_bloque_inicial(t_dialfs *fs, uint32_t bloque_inicial);

#endif // COMPORTAMIENTOS_DIALFS_H