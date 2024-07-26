#ifndef COMPORTAMIENTOS_DIALFS_H
#define COMPORTAMIENTOS_DIALFS_H

#include "DIALFS.h"
#include "variables_globales.h"

//===============================================
// FUNCIONES DE MANEJO DE ARCHIVOS
//===============================================

/**
 * Crea un nuevo archivo en el sistema DialFS.
 * @param nombre Nombre del archivo a crear.
 * @return true si el archivo se creó exitosamente, false en caso contrario.
 */
bool crear_archivo(char* nombre);

/**
 * Elimina un archivo del sistema DialFS.
 * @param nombre Nombre del archivo a eliminar.
 * @return true si el archivo se eliminó exitosamente, false en caso contrario.
 */
bool eliminar_archivo(char* nombre);

/**
 * Modifica el tamaño de un archivo existente en DialFS.
 * @param nombre Nombre del archivo a modificar.
 * @param nuevo_tamanio Nuevo tamaño del archivo en bytes.
 * @return true si el archivo se truncó exitosamente, false en caso contrario.
 */
bool truncar_archivo(char* nombre, uint32_t nuevo_tamanio);

/**
 * Escribe datos en un archivo existente en DialFS.
 * @param nombre Nombre del archivo en el que escribir.
 * @param datos Puntero a los datos a escribir.
 * @param tamanio Cantidad de bytes a escribir.
 * @param offset Posición en el archivo donde comenzar a escribir.
 * @return true si la escritura fue exitosa, false en caso contrario.
 */
bool escribir_archivo(char* nombre, void* datos, uint32_t tamanio, uint32_t offset);

/**
 * Lee datos de un archivo existente en DialFS.
 * @param nombre Nombre del archivo del que leer.
 * @param buffer Buffer donde se almacenarán los datos leídos.
 * @param tamanio Cantidad de bytes a leer.
 * @param offset Posición en el archivo donde comenzar a leer.
 * @return true si la lectura fue exitosa, false en caso contrario.
 */
bool leer_archivo(char* nombre, void* buffer, uint32_t tamanio, uint32_t offset);

//===============================================
// FUNCIONES DE MANTENIMIENTO
//===============================================

/**
 * Realiza la compactación del sistema de archivos DialFS.
 */
void compactar_fs();

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

#endif // COMPORTAMIENTOS_DIALFS_H