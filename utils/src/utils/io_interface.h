#ifndef IO_INTERFACE_H
#define IO_INTERFACE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <commons/collections/queue.h>
#include <semaphore.h>

// Definición del enum tipo_interfaz_t
typedef enum {
    GENERICA,
    STDIN,
    STDOUT,
    DIALFS
} tipo_interfaz_t;

// Definición de la estructura para gestionar interfaces de E/S
typedef struct {
    char* nombre_interfaz;
    tipo_interfaz_t tipo_interfaz;
} t_IO_interface;

// Definición de la estructura para representar la conexión de una interfaz de E/S
typedef struct {
    char* nombre_interfaz;
    tipo_interfaz_t tipo_interfaz;
    int file_descriptor;
    t_queue* cola_procesos_bloqueados;
    sem_t sem_cola_bloqueados;
} t_IO_connection;


//===============================================
// FUNCIONES DE TIPO_INTERFAZ_T
//===============================================

// Convierte un string en mayúsculas al valor correspondiente del enum tipo_interfaz_t.
// Pre: El parámetro str debe ser un string en mayúsculas que representa un tipo de interfaz válido.
// Post: La función retorna el valor correspondiente del enum tipo_interfaz_t.
//       Si el string no coincide con ningún tipo de interfaz, se retorna -1.
tipo_interfaz_t string_to_tipo_interfaz(char*);

//===============================================
// FUNCIONES DE T_IO_INTERFACE
//===============================================

// Función para crear una interfaz de E/S
// Pre: El puntero nombre debe apuntar a una cadena válida y no debe ser NULL.
//      El tipo debe ser un valor válido de tipo_interfaz_t.
// Post: Se crea una nueva interfaz de E/S y se retorna un puntero a la misma.
//       Si hay un error en la asignación de memoria, se retorna NULL.
t_IO_interface* crear_IO_interface(const char* , tipo_interfaz_t);

// Función para liberar la memoria de una interfaz de E/S
// Pre: El puntero debe apuntar a una estructura de interfaz de E/S válida y no debe ser NULL.
// Post: La memoria asociada a la interfaz de E/S es liberada.
void liberar_IO_interface(t_IO_interface*);

// Función para obtener el nombre de una interfaz de E/S
// Pre: El puntero debe apuntar a una estructura de interfaz de E/S válida y no debe ser NULL.
// Post: Se retorna el nombre de la interfaz de E/S.
char* obtener_nombre_IO_interface(t_IO_interface*);

// Función para obtener el tipo de una interfaz de E/S
// Pre: El puntero debe apuntar a una estructura de interfaz de E/S válida y no debe ser NULL.
// Post: Se retorna el tipo de la interfaz de E/S.
tipo_interfaz_t obtener_tipo_IO_interface(t_IO_interface*);

uint32_t obtener_size_IO_interface(t_IO_interface*);

//===============================================
// FUNCIONES DE T_IO_CONNECTION
//===============================================

// Función para crear una conexión de interfaz de E/S
// Pre: El puntero nombre debe apuntar a una cadena válida y no debe ser NULL.
//      El tipo debe ser un valor válido de tipo_interfaz_t.
//      El file_descriptor debe ser un descriptor de archivo válido.
// Post: Se crea una nueva conexión de interfaz de E/S y se retorna un puntero a la misma.
//       Si hay un error en la asignación de memoria, se retorna NULL.
t_IO_connection* crear_IO_connection(const char* nombre, tipo_interfaz_t tipo, int fd);

// Función para liberar la memoria de una conexión de interfaz de E/S
// Pre: El puntero conexión debe apuntar a una estructura de conexión de interfaz de E/S válida y no debe ser NULL.
// Post: La memoria asociada a la conexión de E/S es liberada.
void liberar_IO_connection(t_IO_connection* conexion);

// Función para obtener el nombre de una conexión de E/S
// Pre: El puntero conexión debe apuntar a una estructura de conexión de E/S válida y no debe ser NULL.
// Post: Se retorna el nombre de la conexión de E/S.
char* obtener_nombre_conexion(t_IO_connection* conexion);

// Función para obtener el tipo de una conexión de E/S
// Pre: El puntero conexión debe apuntar a una estructura de conexión de E/S válida y no debe ser NULL.
// Post: Se retorna el tipo de la conexión de E/S.
tipo_interfaz_t obtener_tipo_conexion(t_IO_connection* conexion);

// Función para obtener el descriptor de archivo de una conexión de E/S
// Pre: El puntero conexión debe apuntar a una estructura de conexión de E/S válida y no debe ser NULL.
// Post: Se retorna el descriptor de archivo de la conexión de E/S.
int obtener_file_descriptor(t_IO_connection* conexion);

// Función para obtener la cola de procesos bloqueados de una conexión de E/S
// Pre: El puntero conexión debe apuntar a una estructura de conexión de E/S válida y no debe ser NULL.
// Post: Se retorna el puntero a la cola de procesos bloqueados de la conexión de E/S.
t_queue* obtener_cola_procesos_bloqueados(t_IO_connection* conexion);

sem_t* obtener_semaforo_cola_bloqueados(t_IO_connection* conexion);

#endif // IO_INTERFACE_H
