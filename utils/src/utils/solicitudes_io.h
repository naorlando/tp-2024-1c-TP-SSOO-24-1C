#ifndef SOLICITUDES_IO_H
#define SOLICITUDES_IO_H

#include <stdlib.h>
#include <string.h>
#include "estructuras.h"
#include "instruccion.h"
#include "io_frames_lib.h"

//===============================================
// ESTRUCTURAS
//===============================================

// INTERFAZ GENERICA
typedef struct
{
    char *nombre_interfaz;
    uint32_t tiempo_sleep;
    uint32_t pid;
} t_io_generica;

// INTERFAZ STDIN
typedef struct
{
    t_io_frames *frames_data;
} t_io_stdin;

// INTERFAZ STDOUT
typedef struct
{
    t_io_frames *frames_data;
} t_io_stdout;

// SOLICITUD GENERICA
typedef struct
{
    t_PCB *pcb;
    char *nombre_interfaz;
    t_io_generica *generica;
} t_solicitud_io_generica;

// SOLICITUD STDIN
typedef struct
{
    t_PCB *pcb;
    char *nombre_interfaz;
    t_io_stdin *io_stdin;
} t_solicitud_io_stdin;

// SOLICITUD STDOUT

typedef struct
{
    t_PCB *pcb;
    char *nombre_interfaz;
    t_io_stdout *io_stdout;
} t_solicitud_io_stdout;

// RESPONSE
typedef struct
{
    bool process;
    uint32_t pid;
} t_response;

/* PARÁMETROS DE UNA OPERACIÓN DE DIALFS
Esta estructura ajusta a lo que el enunciado requiere para las solicitudes del Kernel al IO DialFS:
 *IO_FS_CREATE: Usa nombre_interfaz, nombre_archivo, pid.
 *IO_FS_DELETE: Usa nombre_interfaz, nombre_archivo, pid.
 *IO_FS_TRUNCATE: Usa nombre_interfaz, nombre_archivo, tamanio, pid.
 *IO_FS_WRITE: Usa nombre_interfaz, nombre_archivo, direccion_logica, tamanio, puntero_archivo, pid.
 *IO_FS_READ: Usa nombre_interfaz, nombre_archivo, direccion_logica, tamanio, puntero_archivo, pid.
*/
typedef struct
{
    void * dialfs_generic;
    uint32_t pid;                 // PID del proceso que realiza la operación
    t_name_instruction operacion; // Tipo de operación a realizar
} t_io_dialfs;

typedef struct
{
    char *nombre_interfaz; // Nombre de la interfaz DialFS
    char *nombre_archivo;  // Nombre del archivo a operar
                           // Puntero del archivo para operaciones de lectura/escritura
} t_io_dialfs_cd;

typedef struct
{
    char *nombre_interfaz; // Nombre de la interfaz DialFS
    char *nombre_archivo;  // Nombre del archivo a operar
    uint32_t tamanio;      // Tamaño para operaciones de lectura/escritura/truncado
} t_io_dialfs_truncate;

typedef struct
{
    char *nombre_interfaz; // Nombre de la interfaz DialFS
    char *nombre_archivo;  // Nombre del archivo a operar
    uint32_t tamanio;      // Tamaño para operaciones de lectura/escritura/truncado
    t_io_frames *frames_data;
    uint32_t puntero_archivo; // Puntero del archivo para operaciones de lectura/escritura
} t_io_dialfs_rw;

// SOLICITUD DIALFS
typedef struct
{
    t_PCB *pcb;
    char *nombre_interfaz;
    t_io_dialfs *io_dialfs;
} t_solicitud_io_dialfs;

//===============================================
// FUNCIONES DE IO GENERICA
//===============================================

// Función para crear una solicitud de E/S genérica.
// Pre: El PCB, el nombre de interfaz y la E/S genérica deben ser válidos y no NULL.
// Post: Retorna un puntero a una estructura t_solicitud_io_generica creada.
t_solicitud_io_generica *crear_solicitud_io_generica(t_PCB *pcb, char *nombre_interfaz, t_io_generica *generica);

// Función para crear una E/S genérica.
// Pre: El nombre de interfaz, el tiempo de sleep y el pid deben ser válidos.
// Post: Retorna un puntero a una estructura t_io_generica creada.
t_io_generica *crear_io_generica(char *nombre_interfaz, uint32_t tiempo_sleep, uint32_t pid);

// Función para destruir una solicitud de E/S genérica.
// Pre: La solicitud debe ser válida y no NULL.
// Post: La memoria asociada a la solicitud se libera.
void destruir_solicitud_io_generica(t_solicitud_io_generica *solicitud);

// Función para destruir una E/S genérica.
// Pre: La E/S genérica debe ser válida y no NULL.
// Post: La memoria asociada a la E/S genérica se libera.
void destruir_io_generica(t_io_generica *io_generica);

// Obtiene el nombre de la interfaz genérica de una estructura t_io_generica.
// Pre: El puntero io_generica debe apuntar a una estructura t_io_generica válida y no debe ser NULL.
// Post: Retorna un puntero a una cadena de caracteres que contiene el nombre de la interfaz.
char *obtener_nombre_interfaz_generica(t_io_generica *io_generica);

// Obtiene el tiempo de sleep de una estructura t_io_generica.
// Pre: El puntero io_generica debe apuntar a una estructura t_io_generica válida y no debe ser NULL.
// Post: Retorna el tiempo de sleep como un valor uint32_t.
uint32_t obtener_tiempo_sleep(t_io_generica *io_generica);

// Obtiene el tamaño de una interfaz de I/O genérica.
// Pre: El puntero io_generica debe apuntar a una estructura t_io_generica válida y no debe ser NULL.
// Post: Retorna el tamaño de la interfaz de I/O genérica como un valor uint32_t.
uint32_t obtener_tamanio_io_generica(t_io_generica *io_generica);

//===============================================
// FUNCIONES DE IO STDIN
//===============================================

// Función para crear una solicitud de E/S STDIN.
// Pre: El PCB, el nombre de interfaz y la E/S STDIN deben ser válidos y no NULL.
// Post: Retorna un puntero a una estructura t_solicitud_io_stdin creada.
t_solicitud_io_stdin *crear_solicitud_io_stdin(t_PCB *pcb, char *nombre_interfaz, t_io_stdin *io_stdin);

// Función para crear una E/S STDIN.
// Pre: La dirección física, el tamaño y el pid deben ser válidos.
// Post: Retorna un puntero a una estructura t_io_stdin creada.
t_io_stdin *crear_io_stdin(t_io_frames *);

// Función para destruir una solicitud de E/S STDIN.
// Pre: La solicitud debe ser válida y no NULL.
// Post: La memoria asociada a la solicitud se libera.
void destruir_solicitud_io_stdin(t_solicitud_io_stdin *solicitud);

// Función para destruir una E/S STDIN.
// Pre: La E/S STDIN debe ser válida y no NULL.
// Post: La memoria asociada a la E/S STDIN se libera.
void destruir_io_stdin(t_io_stdin *io_stdin);

// Obtiene la dirección física del stdin de una estructura t_io_stdin.
// Pre: El puntero io_stdin debe apuntar a una estructura t_io_stdin válida y no debe ser NULL.
// Post: Retorna la dirección física del stdin como un valor uint32_t.
t_frame_data *obtener_frame_stdin(t_io_stdin *io_stdin, int index);

// Obtiene el tamaño de una interfaz de I/O stdin.
// Pre: El puntero io_stdin debe apuntar a una estructura t_io_stdin válida y no debe ser NULL.
// Post: Retorna el tamaño de la interfaz de I/O stdin como un valor uint32_t.
uint32_t obtener_tamanio_io_stdin(t_io_stdin *io_stdin);

//===============================================
// FUNCIONES DE IO STDOUT
//===============================================

// Función para crear una solicitud de E/S STDOUT.
// Pre: El PCB, el nombre de interfaz y la E/S STDOUT deben ser válidos y no NULL.
// Post: Retorna un puntero a una estructura t_solicitud_io_stdout creada.
t_solicitud_io_stdout *crear_solicitud_io_stdout(t_PCB *pcb, char *nombre_interfaz, t_io_stdout *io_stdout);

// Función para crear una E/S STDOUT.
// Pre: La dirección física, el tamaño y el pid deben ser válidos.
// Post: Retorna un puntero a una estructura t_io_stdout creada.
t_io_stdout *crear_io_stdout(t_io_frames *);

// Función para destruir una solicitud de E/S STDOUT.
// Pre: La solicitud debe ser válida y no NULL.
// Post: La memoria asociada a la solicitud se libera.
void destruir_solicitud_io_stdout(t_solicitud_io_stdout *solicitud);

// Función para destruir una E/S STDOUT.
// Pre: La E/S STDOUT debe ser válida y no NULL.
// Post: La memoria asociada a la E/S STDOUT se libera.
void destruir_io_stdout(t_io_stdout *io_stdout);

t_frame_data *obtener_frame_stdout(t_io_stdout *io_stdout, int index);

// Crea una nueva estructura t_response con los valores especificados.
// Pre: Ninguna.
// Post: Retorna un puntero a una estructura t_response inicializada con los valores de 'process' y 'pid'.
//       Si no se puede asignar memoria, retorna NULL.
t_response *create_response(bool, uint32_t);

// Libera la memoria asignada para una estructura t_response.
// Pre: 'response' debe ser un puntero válido a una estructura t_response.
// Post: La memoria asignada para 'response' se libera.
void delete_response(t_response *);

//===============================================
// FUNCIONES DE ACCESO A CAMPOS
//===============================================

// Obtiene el nombre de la interfaz genérica de una estructura t_io_generica.
// Pre: El puntero io_generica debe apuntar a una estructura t_io_generica válida y no debe ser NULL.
// Post: Retorna un puntero a una cadena de caracteres que contiene el nombre de la interfaz.
char *obtener_nombre_interfaz_generica(t_io_generica *io_generica);

// Obtiene el tiempo de sleep de una estructura t_io_generica.
// Pre: El puntero io_generica debe apuntar a una estructura t_io_generica válida y no debe ser NULL.
// Post: Retorna el tiempo de sleep como un valor uint32_t.
uint32_t obtener_tiempo_sleep(t_io_generica *io_generica);

// Obtiene el PID asociado a una io_generica.
// Pre: El puntero io_generica debe apuntar a una estructura io_generica válida y no debe ser NULL.
// Post: Retorna un valor de tipo uint32_t que contiene el PID asociado.
uint32_t obtener_pid_generica(t_io_generica *io_generica);

// Obtiene el tamaño del stdin de una estructura t_io_stdin.
// Pre: El puntero io_stdin debe apuntar a una estructura t_io_stdin válida y no debe ser NULL.
// Post: Retorna el tamaño del stdin como un valor uint32_t.
uint32_t obtener_tamanio_stdin(t_io_stdin *io_stdin);

// Obtiene el PID asociado a una io_stdin.
// Pre: El puntero io_stdin debe apuntar a una estructura io_stdin válida y no debe ser NULL.
// Post: Retorna un valor de tipo uint32_t que contiene el PID asociado.
uint32_t obtener_pid_stdin(t_io_stdin *io_stdin);

// Obtiene la dirección física del stdout de una estructura t_io_stdout.
// Pre: El puntero io_stdout debe apuntar a una estructura t_io_stdout válida y no debe ser NULL.
// Post: Retorna la dirección física del stdout como un valor uint32_t.
uint32_t obtener_direccion_fisica_stdout(t_io_stdout *io_stdout);

// Obtiene el tamaño del stdout de una estructura t_io_stdout.
// Pre: El puntero io_stdout debe apuntar a una estructura t_io_stdout válida y no debe ser NULL.
// Post: Retorna el tamaño del stdout como un valor uint32_t.
uint32_t obtener_tamanio_stdout(t_io_stdout *io_stdout);

// Obtiene el PID asociado a una io_stdout.
// Pre: El puntero io_stdout debe apuntar a una estructura t_io_stdout válida y no debe ser NULL.
// Post: Retorna un valor de tipo uint32_t que contiene el PID asociado.
uint32_t obtener_pid_stdout(t_io_stdout *io_stdout);

// Obtiene el PCB de una solicitud de I/O genérica.
// Pre: El puntero solicitud debe apuntar a una estructura t_solicitud_io_generica válida y no debe ser NULL.
// Post: Retorna un puntero a una estructura t_PCB.
t_PCB *obtener_pcb_solicitud_generica(t_solicitud_io_generica *solicitud);

// Obtiene el nombre de la solicitud de I/O genérica.
// Pre: El puntero solicitud debe apuntar a una estructura t_solicitud_io_generica válida y no debe ser NULL.
// Post: Retorna un puntero a una cadena de caracteres que contiene el nombre de la solicitud.
char *obtener_nombre_solicitud_generica(t_solicitud_io_generica *solicitud);

// Obtiene la interfaz de I/O genérica de una solicitud.
// Pre: El puntero solicitud debe apuntar a una estructura t_solicitud_io_generica válida y no debe ser NULL.
// Post: Retorna un puntero a una estructura t_io_generica.
t_io_generica *obtener_io_solicitud_generica(t_solicitud_io_generica *solicitud);

// Obtiene el PCB de una solicitud de stdin.
// Pre: El puntero solicitud debe apuntar a una estructura t_solicitud_io_stdin válida y no debe ser NULL.
// Post: Retorna un puntero a una estructura t_PCB.
t_PCB *obtener_pcb_solicitud_stdin(t_solicitud_io_stdin *solicitud);

// Obtiene el nombre de la solicitud de stdin.
// Pre: El puntero solicitud debe apuntar a una estructura t_solicitud_io_stdin válida y no debe ser NULL.
// Post: Retorna un puntero a una cadena de caracteres que contiene el nombre de la solicitud.
char *obtener_nombre_solicitud_stdin(t_solicitud_io_stdin *solicitud);

// Obtiene la interfaz de stdin de una solicitud.
// Pre: El puntero solicitud debe apuntar a una estructura t_solicitud_io_stdin válida y no debe ser NULL.
// Post: Retorna un puntero a una estructura t_io_stdin.
t_io_stdin *obtener_io_solicitud_stdin(t_solicitud_io_stdin *solicitud);

// Obtiene el PCB de una solicitud de stdout.
// Pre: El puntero solicitud debe apuntar a una estructura t_solicitud_io_stdout válida y no debe ser NULL.
// Post: Retorna un puntero a una estructura t_PCB.
t_PCB *obtener_pcb_solicitud_stdout(t_solicitud_io_stdout *solicitud);

// Obtiene el nombre de la solicitud de stdout.
// Pre: El puntero solicitud debe apuntar a una estructura t_solicitud_io_stdout válida y no debe ser NULL.
// Post: Retorna un puntero a una cadena de caracteres que contiene el nombre de la solicitud.
char *obtener_nombre_solicitud_stdout(t_solicitud_io_stdout *solicitud);

// Obtiene la interfaz de stdout de una solicitud.
// Pre: El puntero solicitud debe apuntar a una estructura t_solicitud_io_stdout válida y no debe ser NULL.
// Post: Retorna un puntero a una estructura t_io_stdout.
t_io_stdout *obtener_io_solicitud_stdout(t_solicitud_io_stdout *solicitud);

// Obtiene el tamaño de una solicitud de stdout.
// Pre: El puntero solicitud debe apuntar a una estructura t_solicitud_io_stdout válida y no debe ser NULL.
// Post: Retorna el tamaño de la solicitud de stdout como un valor uint32_t.
uint32_t obtener_tamanio_solicitud_stdout(t_solicitud_io_stdout *solicitud);

// Obtiene el tamaño de una solicitud de stdin.
// Pre: El puntero solicitud debe apuntar a una estructura t_solicitud_io_stdin válida y no debe ser NULL.
// Post: Retorna el tamaño de la solicitud de stdin como un valor uint32_t.
uint32_t obtener_tamanio_solicitud_stdin(t_solicitud_io_stdin *solicitud);

// Obtiene el tamaño de una solicitud de I/O genérica.
// Pre: El puntero solicitud debe apuntar a una estructura t_solicitud_io_generica válida y no debe ser NULL.
// Post: Retorna el tamaño de la solicitud de I/O genérica como un valor uint32_t.
uint32_t obtener_tamanio_solicitud_generica(t_solicitud_io_generica *solicitud);

// Obtiene el tamaño de una interfaz de I/O stdout.
// Pre: El puntero io_stdout debe apuntar a una estructura t_io_stdout válida y no debe ser NULL.
// Post: Retorna el tamaño de la interfaz de I/O stdout como un valor uint32_t.
uint32_t obtener_tamanio_io_stdout(t_io_stdout *io_stdout);

//===============================================
// FUNCIONES DE IO DIALFS
//===============================================

// Crea una nueva estructura de E/S DialFS.
// Parámetros:
//   - nombre_interfaz: Nombre de la interfaz DialFS.
//   - nombre_archivo: Nombre del archivo a operar.
//   - pid: Identificador del proceso.
//   - operacion: Tipo de operación a realizar.
//   - tamanio: Tamaño para operaciones de lectura/escritura/truncado.
//   - direccion_logica: Dirección lógica para operaciones de lectura/escritura.
//   - puntero_archivo: Puntero del archivo para operaciones de lectura/escritura.
// Retorna: Puntero a la nueva estructura t_io_dialfs o NULL si falla la asignación.
t_io_dialfs* crear_io_dialfs(uint32_t pid, t_name_instruction operacion, void * generic); 
// Libera la memoria de una estructura de E/S DialFS.
// Parámetro:
//   - io_dialfs: Puntero a la estructura a liberar.
void destruir_io_dialfs(t_io_dialfs *io_dialfs);

// Crea una nueva solicitud de E/S DialFS.
// Parámetros:
//   - pcb: Puntero al PCB del proceso.
//   - nombre_interfaz: Nombre de la interfaz de E/S.
//   - io_dialfs: Puntero a la estructura de E/S DialFS.
// Retorna: Puntero a la nueva estructura t_solicitud_io_dialfs o NULL si falla la asignación.
t_solicitud_io_dialfs *crear_solicitud_io_dialfs(t_PCB *pcb, char *nombre_interfaz, t_io_dialfs *io_dialfs);

// Libera la memoria de una solicitud de E/S DialFS.
// Parámetro:
//   - solicitud: Puntero a la solicitud a liberar.
void destruir_solicitud_io_dialfs(t_solicitud_io_dialfs *solicitud);

// Calcula el tamaño en bytes de una estructura de E/S DialFS.
// Parámetro:
//   - io_dialfs: Puntero a la estructura de E/S DialFS.
// Retorna: Tamaño en bytes de la estructura.
uint32_t obtener_tamanio_io_dialfs(t_io_dialfs *io_dialfs);

// Calcula el tamaño en bytes de una solicitud de E/S DialFS.
// Parámetro:
//   - solicitud: Puntero a la solicitud de E/S DialFS.
// Retorna: Tamaño en bytes de la solicitud.
uint32_t obtener_tamanio_solicitud_dialfs(t_solicitud_io_dialfs *solicitud);

// Obtiene el PCB asociado a una solicitud de E/S DialFS.
// Parámetro:
//   - solicitud: Puntero a la solicitud de E/S DialFS.
// Retorna: Puntero al PCB.
t_PCB *obtener_pcb_solicitud_dialfs(t_solicitud_io_dialfs *solicitud);

// Obtiene el nombre de la interfaz de una solicitud de E/S DialFS.
// Parámetro:
//   - solicitud: Puntero a la solicitud de E/S DialFS.
// Retorna: Nombre de la interfaz.
char *obtener_nombre_solicitud_dialfs(t_solicitud_io_dialfs *solicitud);

// Obtiene la estructura de E/S DialFS de una solicitud.
// Parámetro:
//   - solicitud: Puntero a la solicitud de E/S DialFS.
// Retorna: Puntero a la estructura de E/S DialFS.
t_io_dialfs *obtener_io_solicitud_dialfs(t_solicitud_io_dialfs *solicitud);

//===============================================
// FUNCIONES DE RESPONSE
//===============================================

// Crea una nueva estructura de respuesta.
// Parámetros:
//   - process: Indica si el proceso fue exitoso.
//   - pid: Identificador del proceso.
// Retorna: Puntero a la nueva estructura t_response o NULL si falla la asignación.
t_response *create_response(bool process, uint32_t pid);

// Libera la memoria de una estructura de respuesta.
// Parámetro:
//   - response: Puntero a la estructura a liberar.
void delete_response(t_response *response);

// Obtiene el estado del proceso de una respuesta.
// Parámetro:
//   - response: Puntero a la estructura de respuesta.
// Retorna: true si el proceso fue exitoso, false en caso contrario.
bool get_process_response(t_response *response);

// Obtiene el PID asociado a una respuesta.
// Parámetro:
//   - response: Puntero a la estructura de respuesta.
// Retorna: PID del proceso.
uint32_t get_pid_response(t_response *response);

// Calcula el tamaño en bytes de una estructura de respuesta.
// Parámetro:
//   - response: Puntero a la estructura de respuesta.
// Retorna: Tamaño en bytes de la estructura.
uint32_t get_size_response(t_response *response);

//===============================================
// FUNCIONES AUXILIARES
//===============================================

// Obtiene el PCB de una solicitud específica basada en el tipo de interfaz.
// Pre: El puntero solicitud debe apuntar a una solicitud válida y no debe ser NULL.
//      El puntero tipo_interfaz debe apuntar a una cadena de caracteres válida que representa el tipo de interfaz y no debe ser NULL.
// Post: Retorna un puntero a un t_PCB si se encuentra uno correspondiente al tipo de interfaz especificado.
//       Si el tipo de interfaz no es reconocido, retorna NULL.
t_PCB *obtener_pcb_de_solicitud(void *, char *);

// Destruye una estructura de solicitud IO basada en su tipo.
// Pre: El puntero solicitud debe apuntar a una estructura de solicitud IO válida y no debe ser NULL.
//      El puntero tipo_interfaz debe apuntar a una cadena de caracteres válida que representa el tipo de interfaz y no debe ser NULL.
// Post: La estructura de solicitud IO se destruye utilizando la función de destrucción correspondiente.
void destruir_solicitud_io(void *, char *);

/**
 * Obtiene el nombre de una operación de entrada/salida.
 * @param operacion Operación de entrada/salida.
 * @return Nombre de la operación.
 * @note Esta función es utilizada para mostrar información en los logs.
 */
char *get_operation_name(t_name_instruction operacion);

t_io_dialfs_cd* crear_io_dialfs_cd(char *nombre_interfaz, char *nombre_archivo);
t_io_dialfs_truncate* crear_io_dialfs_truncate(char *nombre_interfaz, char *nombre_archivo, uint32_t tamanio);
t_io_dialfs_rw* crear_io_dialfs_rw(char *nombre_interfaz, char *nombre_archivo, uint32_t tamanio, t_io_frames *frames_data, uint32_t puntero_archivo);
void destruir_io_dialfs_cd(t_io_dialfs_cd* cd);
void destruir_io_dialfs_truncate(t_io_dialfs_truncate* truncate);
void destruir_io_dialfs_rw(t_io_dialfs_rw* rw);

t_PCB *obtener_pcb_solicitud_dialfs(t_solicitud_io_dialfs *solicitud);
char *obtener_nombre_solicitud_dialfs(t_solicitud_io_dialfs *solicitud);

t_io_dialfs *obtener_io_solicitud_dialfs(t_solicitud_io_dialfs *solicitud);
uint32_t obtener_tamanio_solicitud_io_dialfs(t_solicitud_io_dialfs *solicitud);

void* get_dialfs_generic(t_io_dialfs *io_dialfs);

#endif