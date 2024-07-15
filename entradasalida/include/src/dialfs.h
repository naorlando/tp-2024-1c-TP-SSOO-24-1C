#ifndef DIALFS_H
#define DIALFS_H

#include <stdint.h>
#include <stdbool.h>
#include <commons/bitarray.h>
#include "utils/estructuras.h"
#include "utils/buffer.h"
#include "utils/protocolo.h"
#include "variables_globales.h"

typedef struct {
    char* nombre;
    uint32_t bloque_inicial;
    uint32_t tamanio;
} t_archivo_dialfs;

typedef struct {
    char* path_base;
    uint32_t block_size;
    uint32_t block_count;
    uint32_t retraso_compactacion;
    t_bitarray* bitmap;
    t_list* archivos;
} t_dialfs;

//===============================================
// FUNCIONES DE INICIALIZACIÓN Y DESTRUCCIÓN
//===============================================

/**
 * Inicializa una nueva instancia de DialFS.
 * 
 * @param path_base Directorio base donde se almacenarán los archivos del FS.
 * @param block_size Tamaño de cada bloque en bytes.
 * @param block_count Número total de bloques en el FS.
 * @param retraso_compactacion Tiempo de espera después de la compactación en ms.
 * @return Puntero a la nueva instancia de t_dialfs o NULL si falla la inicialización.
 */
t_dialfs* inicializar_dialfs(char* path_base, uint32_t block_size, uint32_t block_count, uint32_t retraso_compactacion);

/**
 * Libera los recursos asociados con una instancia de DialFS.
 * 
 * @param fs Puntero a la instancia de t_dialfs a destruir.
 */
void destruir_dialfs(t_dialfs* fs);

//===============================================
// FUNCIONES DE OPERACIONES DE ARCHIVO
//===============================================

/**
 * Crea un nuevo archivo en el sistema DialFS.
 * 
 * @param fs Puntero a la instancia de t_dialfs.
 * @param nombre Nombre del archivo a crear.
 * @return true si el archivo se creó exitosamente, false en caso contrario.
 */
bool crear_archivo(t_dialfs* fs, char* nombre);

/**
 * Elimina un archivo del sistema DialFS.
 * 
 * @param fs Puntero a la instancia de t_dialfs.
 * @param nombre Nombre del archivo a eliminar.
 * @return true si el archivo se eliminó exitosamente, false en caso contrario.
 */
bool eliminar_archivo(t_dialfs* fs, char* nombre);

/**
 * Modifica el tamaño de un archivo existente en DialFS.
 * Si es necesario, realiza la compactación del sistema de archivos.
 * 
 * @param fs Puntero a la instancia de t_dialfs.
 * @param nombre Nombre del archivo a modificar.
 * @param nuevo_tamanio Nuevo tamaño del archivo en bytes.
 * @return true si el archivo se truncó exitosamente, false en caso contrario.
 */
bool truncar_archivo(t_dialfs* fs, char* nombre, uint32_t nuevo_tamanio);

/**
 * Escribe datos en un archivo existente en DialFS.
 * 
 * @param fs Puntero a la instancia de t_dialfs.
 * @param nombre Nombre del archivo en el que escribir.
 * @param datos Puntero a los datos a escribir.
 * @param tamanio Cantidad de bytes a escribir.
 * @param offset Posición en el archivo donde comenzar a escribir.
 * @return true si la escritura fue exitosa, false en caso contrario.
 */
bool escribir_archivo(t_dialfs* fs, char* nombre, void* datos, uint32_t tamanio, uint32_t offset);

/**
 * Lee datos de un archivo existente en DialFS.
 * 
 * @param fs Puntero a la instancia de t_dialfs.
 * @param nombre Nombre del archivo del que leer.
 * @param buffer Buffer donde se almacenarán los datos leídos.
 * @param tamanio Cantidad de bytes a leer.
 * @param offset Posición en el archivo donde comenzar a leer.
 * @return true si la lectura fue exitosa, false en caso contrario.
 */
bool leer_archivo(t_dialfs* fs, char* nombre, void* buffer, uint32_t tamanio, uint32_t offset);

//===============================================
// FUNCIONES DE MANTENIMIENTO
//===============================================

/**
 * Realiza la compactación del sistema de archivos DialFS.
 * 
 * @param fs Puntero a la instancia de t_dialfs.
 */
void compactar_fs(t_dialfs* fs);

/**
 * Verifica si es necesario realizar una compactación del sistema de archivos.
 * 
 * @param fs Puntero a la instancia de t_dialfs.
 * @return true si es necesario compactar, false en caso contrario.
 */
bool es_necesario_compactar(t_dialfs* fs);

//===============================================
// FUNCIONES AUXILIARES
//===============================================

/**
 * Busca un bloque libre en el sistema de archivos.
 * 
 * @param fs Puntero a la instancia de t_dialfs.
 * @return Número del primer bloque libre encontrado, o (uint32_t)-1 si no hay bloques libres.
 */
uint32_t buscar_bloque_libre(t_dialfs* fs);

/**
 * Libera los bloques ocupados por un archivo.
 * 
 * @param fs Puntero a la instancia de t_dialfs.
 * @param bloque_inicial Número del primer bloque a liberar.
 * @param tamanio Tamaño en bytes del espacio a liberar.
 */
void liberar_bloques(t_dialfs* fs, uint32_t bloque_inicial, uint32_t tamanio);

/**
 * Busca un archivo en el sistema DialFS.
 * 
 * @param fs Puntero a la instancia de t_dialfs.
 * @param nombre Nombre del archivo a buscar.
 * @return Puntero al t_archivo_dialfs si se encuentra, NULL en caso contrario.
 */
t_archivo_dialfs* buscar_archivo(t_dialfs* fs, char* nombre);

/**
 * Amplía un archivo existente.
 * 
 * @param fs Puntero a la instancia de t_dialfs.
 * @param archivo Puntero al t_archivo_dialfs a ampliar.
 * @param nuevo_tamanio Nuevo tamaño del archivo en bytes.
 * @return true si la ampliación fue exitosa, false en caso contrario.
 */
bool ampliar_archivo(t_dialfs* fs, t_archivo_dialfs* archivo, uint32_t nuevo_tamanio);

/**
 * Escribe datos en bloques del sistema de archivos.
 * 
 * @param fs Puntero a la instancia de t_dialfs.
 * @param bloque_inicio Número del bloque inicial.
 * @param offset_bloque Offset dentro del bloque inicial.
 * @param datos Puntero a los datos a escribir.
 * @param tamanio Cantidad de bytes a escribir.
 */
void escribir_bloques(t_dialfs* fs, uint32_t bloque_inicio, uint32_t offset_bloque, void* datos, uint32_t tamanio);

/**
 * Lee datos de bloques del sistema de archivos.
 * 
 * @param fs Puntero a la instancia de t_dialfs.
 * @param bloque_inicio Número del bloque inicial.
 * @param offset_bloque Offset dentro del bloque inicial.
 * @param buffer Buffer donde se almacenarán los datos leídos.
 * @param tamanio Cantidad de bytes a leer.
 */
void leer_bloques(t_dialfs* fs, uint32_t bloque_inicio, uint32_t offset_bloque, void* buffer, uint32_t tamanio);

/**
 * Mueve bloques de una ubicación a otra en el sistema de archivos.
 * 
 * @param fs Puntero a la instancia de t_dialfs.
 * @param bloque_origen Número del bloque de origen.
 * @param bloque_destino Número del bloque de destino.
 * @param cantidad_bloques Cantidad de bloques a mover.
 */
void mover_bloques(t_dialfs* fs, uint32_t bloque_origen, uint32_t bloque_destino, uint32_t cantidad_bloques);

/**
 * Compara dos archivos por sus bloques iniciales.
 * 
 * @param a Puntero al primer t_archivo_dialfs.
 * @param b Puntero al segundo t_archivo_dialfs.
 * @return Diferencia entre los bloques iniciales de a y b.
 */
int comparar_bloques_iniciales(t_archivo_dialfs* a, t_archivo_dialfs* b);

/**
 * Destruye una estructura t_archivo_dialfs.
 * 
 * @param archivo Puntero al t_archivo_dialfs a destruir.
 */
void destruir_archivo_dialfs(t_archivo_dialfs* archivo);

/**
 * Envía los datos leídos al cliente.
 *
 * @param fd Descriptor de archivo para la comunicación.
 * @param buffer Buffer que contiene los datos leídos.
 * @param tamanio Tamaño de los datos a enviar.
 */
void enviar_datos_leidos(int fd, void* buffer, uint32_t tamanio);

/**
 * Verifica si hay espacio contiguo suficiente en el sistema de archivos.
 * 
 * @param fs Puntero a la instancia de t_dialfs.
 * @param bloques_necesarios Cantidad de bloques contiguos necesarios.
 * @return true si hay espacio contiguo suficiente, false en caso contrario.
 */
bool hay_espacio_contiguo(t_dialfs* fs, uint32_t bloques_necesarios);

#endif // DIALFS_H