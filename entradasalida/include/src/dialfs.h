#ifndef DIALFS_H
#define DIALFS_H

#include <stdint.h>
#include <stdbool.h>
#include <commons/bitarray.h>
#include "utils/estructuras.h"
#include "utils/buffer.h"
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

// Inicializa el sistema de archivos DialFS
// Pre: Todos los parámetros deben ser válidos
// Post: Retorna un puntero a una estructura t_dialfs inicializada
t_dialfs* inicializar_dialfs(char* path_base, uint32_t block_size, uint32_t block_count, uint32_t retraso_compactacion);

// Destruye el sistema de archivos DialFS y libera toda la memoria asociada
// Pre: fs debe ser un puntero válido a una estructura t_dialfs
// Post: Toda la memoria asociada a fs es liberada
void destruir_dialfs(t_dialfs* fs);

//===============================================
// FUNCIONES DE OPERACIONES DE ARCHIVO
//===============================================

// Crea un nuevo archivo en el sistema DialFS
// Pre: fs debe ser un puntero válido a una estructura t_dialfs, nombre debe ser una cadena válida
// Post: Retorna true si el archivo se creó exitosamente, false en caso contrario
bool crear_archivo(t_dialfs* fs, char* nombre);

// Elimina un archivo del sistema DialFS
// Pre: fs debe ser un puntero válido a una estructura t_dialfs, nombre debe ser una cadena válida
// Post: Retorna true si el archivo se eliminó exitosamente, false en caso contrario
bool eliminar_archivo(t_dialfs* fs, char* nombre);

// Trunca un archivo en el sistema DialFS
// Pre: fs debe ser un puntero válido a una estructura t_dialfs, nombre debe ser una cadena válida
// Post: Retorna true si el archivo se truncó exitosamente, false en caso contrario
bool truncar_archivo(t_dialfs* fs, char* nombre, uint32_t nuevo_tamanio);

// Escribe datos en un archivo en el sistema DialFS
// Pre: fs debe ser un puntero válido a una estructura t_dialfs, nombre debe ser una cadena válida
// Post: Retorna true si los datos se escribieron exitosamente, false en caso contrario
bool escribir_archivo(t_dialfs* fs, char* nombre, void* datos, uint32_t tamanio, uint32_t offset);

// Lee datos de un archivo en el sistema DialFS
// Pre: fs debe ser un puntero válido a una estructura t_dialfs, nombre debe ser una cadena válida
// Post: Retorna true si los datos se leyeron exitosamente, false en caso contrario
bool leer_archivo(t_dialfs* fs, char* nombre, void* buffer, uint32_t tamanio, uint32_t offset);

//===============================================
// FUNCIONES DE MANTENIMIENTO
//===============================================

// Compacta el sistema de archivos DialFS
// Pre: fs debe ser un puntero válido a una estructura t_dialfs
// Post: El sistema de archivos es compactado
void compactar_fs(t_dialfs* fs);

// Verifica si es necesario compactar el sistema de archivos
// Pre: fs debe ser un puntero válido a una estructura t_dialfs
// Post: Retorna true si es necesario compactar, false en caso contrario
bool es_necesario_compactar(t_dialfs* fs);

//===============================================
// FUNCIONES AUXILIARES
//===============================================

// Envía los datos leídos al descriptor de archivo especificado
// Pre: fd debe ser un descriptor de archivo válido, buffer debe contener los datos a enviar
// Post: Los datos son enviados al descriptor de archivo
void enviar_datos_leidos(int fd, void* buffer, uint32_t tamanio);

#endif // DIALFS_H