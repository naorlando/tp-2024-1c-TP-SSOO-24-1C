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
// FUNCIONES PRINCIPALES
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

/**
 * Realiza la compactación del sistema de archivos DialFS.
 * 
 * @param fs Puntero a la instancia de t_dialfs.
 */
void compactar_fs(t_dialfs* fs);

/**
 * Envía los datos leídos al cliente.
 *
 * @param fd Descriptor de archivo para la comunicación.
 * @param buffer Buffer que contiene los datos leídos.
 * @param tamanio Tamaño de los datos a enviar.
 */
void enviar_datos_leidos(int fd, void* buffer, uint32_t tamanio);

//===============================================
// FUNCIONES AUXILIARES
//===============================================

/**
 * Busca un bloque libre en el sistema de archivos.
 * 
 * @param fs Puntero a la instancia de t_dialfs.
 * @param num_bloques Número de bloques contiguos necesarios.
 * @return Número del primer bloque libre encontrado, o -1 si no hay suficiente espacio contiguo.
 */
int buscar_bloque_libre(t_dialfs* fs, uint32_t num_bloques);

/**
 * Libera los bloques ocupados por un archivo.
 * 
 * @param fs Puntero a la instancia de t_dialfs.
 * @param bloque_inicial Número del primer bloque a liberar.
 * @param num_bloques Número de bloques a liberar.
 */
void liberar_bloques(t_dialfs* fs, uint32_t bloque_inicial, uint32_t num_bloques);

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

#endif // DIALFS_H