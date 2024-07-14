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

// Inicializa el sistema de archivos DialFS, creando las estructuras necesarias
// y configurando los parámetros del sistema de archivos.
// Pre: Todos los parámetros deben ser válidos
// Post: Retorna un puntero a una estructura t_dialfs inicializada
t_dialfs* inicializar_dialfs(char* path_base, uint32_t block_size, uint32_t block_count, uint32_t retraso_compactacion);

// Destruye el sistema de archivos DialFS, liberando toda la memoria asociada
// a las estructuras y recursos utilizados por el sistema.
// Pre: fs debe ser un puntero válido a una estructura t_dialfs
// Post: Toda la memoria asociada a fs es liberada
void destruir_dialfs(t_dialfs* fs);

//===============================================
// FUNCIONES DE OPERACIONES DE ARCHIVO
//===============================================

// Crea un nuevo archivo en el sistema DialFS, asignando los bloques necesarios
// y actualizando las estructuras de control del sistema de archivos.
// Pre: fs debe ser un puntero válido a una estructura t_dialfs, nombre debe ser una cadena válida
// Post: Retorna true si el archivo se creó exitosamente, false en caso contrario
bool crear_archivo(t_dialfs* fs, char* nombre);

// Elimina un archivo del sistema DialFS, liberando los bloques ocupados
// y actualizando las estructuras de control del sistema de archivos.
// Pre: fs debe ser un puntero válido a una estructura t_dialfs, nombre debe ser una cadena válida
// Post: Retorna true si el archivo se eliminó exitosamente, false en caso contrario
bool eliminar_archivo(t_dialfs* fs, char* nombre);

// Trunca un archivo en el sistema DialFS, ajustando su tamaño y reasignando
// o liberando bloques según sea necesario.
// Pre: fs debe ser un puntero válido a una estructura t_dialfs, nombre debe ser una cadena válida
// Post: Retorna true si el archivo se truncó exitosamente, false en caso contrario
bool truncar_archivo(t_dialfs* fs, char* nombre, uint32_t nuevo_tamanio);

// Escribe datos en un archivo en el sistema DialFS, asignando nuevos bloques
// si es necesario y actualizando el contenido del archivo.
// Pre: fs debe ser un puntero válido a una estructura t_dialfs, nombre debe ser una cadena válida
// Post: Retorna true si los datos se escribieron exitosamente, false en caso contrario
bool escribir_archivo(t_dialfs* fs, char* nombre, void* datos, uint32_t tamanio, uint32_t offset);

// Lee datos de un archivo en el sistema DialFS, accediendo a los bloques
// correspondientes y copiando los datos al buffer proporcionado.
// Pre: fs debe ser un puntero válido a una estructura t_dialfs, nombre debe ser una cadena válida
// Post: Retorna true si los datos se leyeron exitosamente, false en caso contrario
bool leer_archivo(t_dialfs* fs, char* nombre, void* buffer, uint32_t tamanio, uint32_t offset);

//===============================================
// FUNCIONES DE MANTENIMIENTO
//===============================================

// Compacta el sistema de archivos DialFS, reorganizando los bloques para
// eliminar la fragmentación y optimizar el uso del espacio.
// Pre: fs debe ser un puntero válido a una estructura t_dialfs
// Post: El sistema de archivos es compactado
void compactar_fs(t_dialfs* fs);

// Verifica si es necesario compactar el sistema de archivos, analizando
// la fragmentación y el espacio libre disponible.
// Pre: fs debe ser un puntero válido a una estructura t_dialfs
// Post: Retorna true si es necesario compactar, false en caso contrario
bool es_necesario_compactar(t_dialfs* fs);

//===============================================
// FUNCIONES AUXILIARES
//===============================================

// Busca un bloque libre en el sistema de archivos DialFS para asignar a un nuevo
// archivo o para ampliar uno existente.
// Pre: fs debe ser un puntero válido a una estructura t_dialfs
// Post: Retorna el número del bloque libre encontrado, o -1 si no hay bloques libres
uint32_t buscar_bloque_libre(t_dialfs* fs);

// Libera los bloques ocupados por un archivo, marcándolos como disponibles en el
// bitmap del sistema de archivos.
// Pre: fs debe ser un puntero válido a una estructura t_dialfs
// Post: Los bloques especificados son marcados como libres en el bitmap
void liberar_bloques(t_dialfs* fs, uint32_t bloque_inicial, uint32_t tamanio);

// Busca un archivo en el sistema DialFS por su nombre, recorriendo la lista de
// archivos almacenados.
// Pre: fs debe ser un puntero válido a una estructura t_dialfs, nombre debe ser una cadena válida
// Post: Retorna un puntero al archivo encontrado, o NULL si no existe
t_archivo_dialfs* buscar_archivo(t_dialfs* fs, char* nombre);

// Amplía el tamaño de un archivo existente, asignando nuevos bloques si es necesario
// y actualizando las estructuras de control.
// Pre: fs y archivo deben ser punteros válidos, nuevo_tamanio debe ser mayor que el tamaño actual
// Post: Retorna true si el archivo se amplió exitosamente, false en caso contrario
bool ampliar_archivo(t_dialfs* fs, t_archivo_dialfs* archivo, uint32_t nuevo_tamanio);

// Escribe datos en los bloques correspondientes del sistema de archivos, manejando
// la fragmentación si es necesario.
// Pre: fs debe ser un puntero válido, datos debe contener la información a escribir
// Post: Los datos son escritos en los bloques especificados
void escribir_bloques(t_dialfs* fs, uint32_t bloque_inicio, uint32_t offset_bloque, void* datos, uint32_t tamanio);

// Lee datos de los bloques correspondientes del sistema de archivos, manejando
// la fragmentación si es necesario.
// Pre: fs debe ser un puntero válido, buffer debe tener espacio suficiente para los datos
// Post: Los datos son leídos de los bloques especificados y almacenados en el buffer
void leer_bloques(t_dialfs* fs, uint32_t bloque_inicio, uint32_t offset_bloque, void* buffer, uint32_t tamanio);

// Mueve bloques de datos de una ubicación a otra en el sistema de archivos,
// utilizado durante la compactación.
// Pre: fs debe ser un puntero válido, los bloques de origen y destino deben ser válidos
// Post: Los bloques son movidos de la ubicación de origen a la de destino
void mover_bloques(t_dialfs* fs, uint32_t bloque_origen, uint32_t bloque_destino, uint32_t cantidad_bloques);

// Compara dos archivos por su bloque inicial, utilizado para ordenar la lista de
// archivos durante la compactación.
// Pre: a y b deben ser punteros válidos a estructuras t_archivo_dialfs
// Post: Retorna un valor negativo si a < b, 0 si a == b, o positivo si a > b
int comparar_bloques_iniciales(t_archivo_dialfs* a, t_archivo_dialfs* b);

// Libera la memoria asociada a una estructura de archivo, utilizado al eliminar
// un archivo del sistema.
// Pre: archivo debe ser un puntero válido a una estructura t_archivo_dialfs
// Post: La memoria asociada al archivo es liberada
void destruir_archivo_dialfs(t_archivo_dialfs* archivo);

// Envía los datos leídos al descriptor de archivo especificado, utilizando
// el protocolo de comunicación definido para el sistema.
// Pre: fd debe ser un descriptor de archivo válido, buffer debe contener los datos a enviar
// Post: Los datos son enviados al descriptor de archivo
void enviar_datos_leidos(int fd, void* buffer, uint32_t tamanio);

#endif // DIALFS_H