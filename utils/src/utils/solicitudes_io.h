#ifndef SOLICITUDES_IO_H
#define SOLICITUDES_IO_H

#include "estructuras.h"
#include "instruccion.h"

//===============================================
// ESTRUCTURAS
//===============================================

// INTERFAZ GENERICA
typedef struct {
    char* nombre_interfaz;
    uint32_t tiempo_sleep;
    uint32_t pid;
} t_io_generica;

// INTERFAZ STDIN
typedef struct {
    uint32_t direccion_logica;  // Cambiado de direccion_fisica a direccion_logica
    uint32_t tamanio;
    uint32_t pid;
    char* nombre_interfaz;  // Añadido nombre_interfaz
} t_io_stdin;

// INTERFAZ STDOUT
typedef struct {
    uint32_t direccion_logica;  // Cambiado de direccion_fisica a direccion_logica
    uint32_t tamanio;
    uint32_t pid;
    char* nombre_interfaz;  // Añadido nombre_interfaz
} t_io_stdout;

// SOLICITUD GENERICA
typedef struct {
    t_PCB* pcb;
    char* nombre_interfaz;
    t_io_generica* generica;
} t_solicitud_io_generica;

// SOLICITUD STDIN
typedef struct {
    t_PCB* pcb;
    char* nombre_interfaz;
    t_io_stdin* io_stdin;
} t_solicitud_io_stdin;

// SOLICITUD STDOUT
typedef struct {
    t_PCB* pcb;
    char* nombre_interfaz;
    t_io_stdout* io_stdout;
} t_solicitud_io_stdout;

// RESPONSE
typedef struct {
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
typedef struct {
    char* nombre_interfaz;      // Nombre de la interfaz DialFS
    char* nombre_archivo;       // Nombre del archivo a operar
    uint32_t pid;               // PID del proceso que realiza la operación
    t_name_instruction operacion; // Tipo de operación a realizar
    uint32_t tamanio;           // Tamaño para operaciones de lectura/escritura/truncado
    uint32_t direccion_logica;  // Dirección lógica para operaciones de lectura/escritura
    uint32_t puntero_archivo;   // Puntero del archivo para operaciones de lectura/escritura
} t_io_dialfs;

// SOLICITUD DIALFS
typedef struct {
    t_PCB* pcb;
    char* nombre_interfaz;
    t_io_dialfs* io_dialfs;
} t_solicitud_io_dialfs;

//===============================================
// FUNCIONES DE IO GENERICA
//===============================================

// Crea una nueva estructura de E/S genérica.
// Parámetros:
//   - nombre_interfaz: Nombre de la interfaz de E/S.
//   - tiempo_sleep: Tiempo de espera para la operación.
//   - pid: Identificador del proceso.
// Retorna: Puntero a la nueva estructura t_io_generica o NULL si falla la asignación.
t_io_generica* crear_io_generica(char* nombre_interfaz, uint32_t tiempo_sleep, uint32_t pid);

// Libera la memoria de una estructura de E/S genérica.
// Parámetro:
//   - io_generica: Puntero a la estructura a liberar.
void destruir_io_generica(t_io_generica* io_generica);

// Obtiene el nombre de la interfaz de una E/S genérica.
// Parámetro:
//   - io_generica: Puntero a la estructura de E/S genérica.
// Retorna: Nombre de la interfaz.
char* obtener_nombre_interfaz_generica(t_io_generica* io_generica);

// Obtiene el PID asociado a una E/S genérica.
// Parámetro:
//   - io_generica: Puntero a la estructura de E/S genérica.
// Retorna: PID del proceso.
uint32_t obtener_pid_generica(t_io_generica* io_generica);

// Obtiene el tiempo de sleep de una E/S genérica.
// Parámetro:
//   - io_generica: Puntero a la estructura de E/S genérica.
// Retorna: Tiempo de sleep.
uint32_t obtener_tiempo_sleep(t_io_generica* io_generica);

// Calcula el tamaño en bytes de una estructura de E/S genérica.
// Parámetro:
//   - io_generica: Puntero a la estructura de E/S genérica.
// Retorna: Tamaño en bytes de la estructura.
uint32_t obtener_tamanio_io_generica(t_io_generica* io_generica);

//===============================================
// FUNCIONES DE IO STDIN
//===============================================

// Crea una nueva estructura de E/S STDIN.
// Parámetros:
//   - direccion_logica: Dirección lógica para la operación.
//   - tamanio: Tamaño de los datos a leer.
//   - pid: Identificador del proceso.
//   - nombre_interfaz: Nombre de la interfaz STDIN.
// Retorna: Puntero a la nueva estructura t_io_stdin o NULL si falla la asignación.
t_io_stdin* crear_io_stdin(uint32_t direccion_logica, uint32_t tamanio, uint32_t pid, char* nombre_interfaz);

// Libera la memoria de una estructura de E/S STDIN.
// Parámetro:
//   - io_stdin: Puntero a la estructura a liberar.
void destruir_io_stdin(t_io_stdin* io_stdin);

// Obtiene la dirección lógica de una E/S STDIN.
// Parámetro:
//   - io_stdin: Puntero a la estructura de E/S STDIN.
// Retorna: Dirección lógica.
uint32_t obtener_direccion_logica_stdin(t_io_stdin* io_stdin);

// Obtiene el tamaño de los datos a leer de una E/S STDIN.
// Parámetro:
//   - io_stdin: Puntero a la estructura de E/S STDIN.
// Retorna: Tamaño de los datos.
uint32_t obtener_tamanio_stdin(t_io_stdin* io_stdin);

// Obtiene el PID asociado a una E/S STDIN.
// Parámetro:
//   - io_stdin: Puntero a la estructura de E/S STDIN.
// Retorna: PID del proceso.
uint32_t obtener_pid_stdin(t_io_stdin* io_stdin);

// Obtiene el nombre de la interfaz de una E/S STDIN.
// Parámetro:
//   - io_stdin: Puntero a la estructura de E/S STDIN.
// Retorna: Nombre de la interfaz.
char* obtener_nombre_interfaz_stdin(t_io_stdin* io_stdin);

// Calcula el tamaño en bytes de una estructura de E/S STDIN.
// Parámetro:
//   - io_stdin: Puntero a la estructura de E/S STDIN.
// Retorna: Tamaño en bytes de la estructura.
uint32_t obtener_tamanio_io_stdin(t_io_stdin* io_stdin);

//===============================================
// FUNCIONES DE IO STDOUT
//===============================================

// Crea una nueva estructura de E/S STDOUT.
// Parámetros:
//   - direccion_logica: Dirección lógica para la operación.
//   - tamanio: Tamaño de los datos a escribir.
//   - pid: Identificador del proceso.
//   - nombre_interfaz: Nombre de la interfaz STDOUT.
// Retorna: Puntero a la nueva estructura t_io_stdout o NULL si falla la asignación.
t_io_stdout* crear_io_stdout(uint32_t direccion_logica, uint32_t tamanio, uint32_t pid, char* nombre_interfaz);

// Libera la memoria de una estructura de E/S STDOUT.
// Parámetro:
//   - io_stdout: Puntero a la estructura a liberar.
void destruir_io_stdout(t_io_stdout* io_stdout);

// Obtiene la dirección lógica de una E/S STDOUT.
// Parámetro:
//   - io_stdout: Puntero a la estructura de E/S STDOUT.
// Retorna: Dirección lógica.
uint32_t obtener_direccion_logica_stdout(t_io_stdout* io_stdout);

// Obtiene el tamaño de los datos a escribir de una E/S STDOUT.
// Parámetro:
//   - io_stdout: Puntero a la estructura de E/S STDOUT.
// Retorna: Tamaño de los datos.
uint32_t obtener_tamanio_stdout(t_io_stdout* io_stdout);

// Obtiene el PID asociado a una E/S STDOUT.
// Parámetro:
//   - io_stdout: Puntero a la estructura de E/S STDOUT.
// Retorna: PID del proceso.
uint32_t obtener_pid_stdout(t_io_stdout* io_stdout);

// Obtiene el nombre de la interfaz de una E/S STDOUT.
// Parámetro:
//   - io_stdout: Puntero a la estructura de E/S STDOUT.
// Retorna: Nombre de la interfaz.
char* obtener_nombre_interfaz_stdout(t_io_stdout* io_stdout);

// Calcula el tamaño en bytes de una estructura de E/S STDOUT.
// Parámetro:
//   - io_stdout: Puntero a la estructura de E/S STDOUT.
// Retorna: Tamaño en bytes de la estructura.
uint32_t obtener_tamanio_io_stdout(t_io_stdout* io_stdout);

//===============================================
// FUNCIONES DE SOLICITUD IO GENERICA
//===============================================

// Crea una nueva solicitud de E/S genérica.
// Parámetros:
//   - pcb: Puntero al PCB del proceso.
//   - nombre_interfaz: Nombre de la interfaz de E/S.
//   - generica: Puntero a la estructura de E/S genérica.
// Retorna: Puntero a la nueva estructura t_solicitud_io_generica o NULL si falla la asignación.
t_solicitud_io_generica* crear_solicitud_io_generica(t_PCB* pcb, char* nombre_interfaz, t_io_generica* generica);

// Libera la memoria de una solicitud de E/S genérica.
// Parámetro:
//   - solicitud: Puntero a la solicitud a liberar.
void destruir_solicitud_io_generica(t_solicitud_io_generica* solicitud);

// Obtiene el PCB asociado a una solicitud de E/S genérica.
// Parámetro:
//   - solicitud: Puntero a la solicitud de E/S genérica.
// Retorna: Puntero al PCB.
t_PCB* obtener_pcb_solicitud_generica(t_solicitud_io_generica* solicitud);

// Obtiene el nombre de la interfaz de una solicitud de E/S genérica.
// Parámetro:
//   - solicitud: Puntero a la solicitud de E/S genérica.
// Retorna: Nombre de la interfaz.
char* obtener_nombre_solicitud_generica(t_solicitud_io_generica* solicitud);

// Obtiene la estructura de E/S genérica de una solicitud.
// Parámetro:
//   - solicitud: Puntero a la solicitud de E/S genérica.
// Retorna: Puntero a la estructura de E/S genérica.
t_io_generica* obtener_io_solicitud_generica(t_solicitud_io_generica* solicitud);

// Calcula el tamaño en bytes de una solicitud de E/S genérica.
// Parámetro:
//   - solicitud: Puntero a la solicitud de E/S genérica.
// Retorna: Tamaño en bytes de la solicitud.
uint32_t obtener_tamanio_solicitud_generica(t_solicitud_io_generica* solicitud);

//===============================================
// FUNCIONES DE SOLICITUD IO STDIN
//===============================================

// Crea una nueva solicitud de E/S STDIN.
// Parámetros:
//   - pcb: Puntero al PCB del proceso.
//   - nombre_interfaz: Nombre de la interfaz de E/S.
//   - io_stdin: Puntero a la estructura de E/S STDIN.
// Retorna: Puntero a la nueva estructura t_solicitud_io_stdin o NULL si falla la asignación.
t_solicitud_io_stdin* crear_solicitud_io_stdin(t_PCB* pcb, char* nombre_interfaz, t_io_stdin* io_stdin);

// Libera la memoria de una solicitud de E/S STDIN.
// Parámetro:
//   - solicitud: Puntero a la solicitud a liberar.
void destruir_solicitud_io_stdin(t_solicitud_io_stdin* solicitud);

// Obtiene el PCB asociado a una solicitud de E/S STDIN.
// Parámetro:
//   - solicitud: Puntero a la solicitud de E/S STDIN.
// Retorna: Puntero al PCB.
t_PCB* obtener_pcb_solicitud_stdin(t_solicitud_io_stdin* solicitud);

// Obtiene el nombre de la interfaz de una solicitud de E/S STDIN.
// Parámetro:
//   - solicitud: Puntero a la solicitud de E/S STDIN.
// Retorna: Nombre de la interfaz.
char* obtener_nombre_solicitud_stdin(t_solicitud_io_stdin* solicitud);

// Obtiene la estructura de E/S STDIN de una solicitud.
// Parámetro:
//   - solicitud: Puntero a la solicitud de E/S STDIN.
// Retorna: Puntero a la estructura de E/S STDIN.
t_io_stdin* obtener_io_solicitud_stdin(t_solicitud_io_stdin* solicitud);

// Calcula el tamaño en bytes de una solicitud de E/S STDIN.
// Parámetro:
//   - solicitud: Puntero a la solicitud de E/S STDIN.
// Retorna: Tamaño en bytes de la solicitud.
uint32_t obtener_tamanio_solicitud_stdin(t_solicitud_io_stdin* solicitud);

//===============================================
// FUNCIONES DE SOLICITUD IO STDOUT
//===============================================

// Crea una nueva solicitud de E/S STDOUT.
// Parámetros:
//   - pcb: Puntero al PCB del proceso.
//   - nombre_interfaz: Nombre de la interfaz de E/S.
//   - io_stdout: Puntero a la estructura de E/S STDOUT.
// Retorna: Puntero a la nueva estructura t_solicitud_io_stdout o NULL si falla la asignación.
t_solicitud_io_stdout* crear_solicitud_io_stdout(t_PCB* pcb, char* nombre_interfaz, t_io_stdout* io_stdout);

// Libera la memoria de una solicitud de E/S STDOUT.
// Parámetro:
//   - solicitud: Puntero a la solicitud a liberar.
void destruir_solicitud_io_stdout(t_solicitud_io_stdout* solicitud);

// Obtiene el PCB asociado a una solicitud de E/S STDOUT.
// Parámetro:
//   - solicitud: Puntero a la solicitud de E/S STDOUT.
// Retorna: Puntero al PCB.
t_PCB* obtener_pcb_solicitud_stdout(t_solicitud_io_stdout* solicitud);

// Obtiene el nombre de la interfaz de una solicitud de E/S STDOUT.
// Parámetro:
//   - solicitud: Puntero a la solicitud de E/S STDOUT.
// Retorna: Nombre de la interfaz.
char* obtener_nombre_solicitud_stdout(t_solicitud_io_stdout* solicitud);

// Obtiene la estructura de E/S STDOUT de una solicitud.
// Parámetro:
//   - solicitud: Puntero a la solicitud de E/S STDOUT.
// Retorna: Puntero a la estructura de E/S STDOUT.
t_io_stdout* obtener_io_solicitud_stdout(t_solicitud_io_stdout* solicitud);

// Calcula el tamaño en bytes de una solicitud de E/S STDOUT.
// Parámetro:
//   - solicitud: Puntero a la solicitud de E/S STDOUT.
// Retorna: Tamaño en bytes de la solicitud.
uint32_t obtener_tamanio_solicitud_stdout(t_solicitud_io_stdout* solicitud);

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
t_io_dialfs* crear_io_dialfs(char* nombre_interfaz, char* nombre_archivo, uint32_t pid, t_name_instruction operacion, uint32_t tamanio, uint32_t direccion_logica, uint32_t puntero_archivo);

// Libera la memoria de una estructura de E/S DialFS.
// Parámetro:
//   - io_dialfs: Puntero a la estructura a liberar.
void destruir_io_dialfs(t_io_dialfs* io_dialfs);

// Crea una nueva solicitud de E/S DialFS.
// Parámetros:
//   - pcb: Puntero al PCB del proceso.
//   - nombre_interfaz: Nombre de la interfaz de E/S.
//   - io_dialfs: Puntero a la estructura de E/S DialFS.
// Retorna: Puntero a la nueva estructura t_solicitud_io_dialfs o NULL si falla la asignación.
t_solicitud_io_dialfs* crear_solicitud_io_dialfs(t_PCB* pcb, char* nombre_interfaz, t_io_dialfs* io_dialfs);

// Libera la memoria de una solicitud de E/S DialFS.
// Parámetro:
//   - solicitud: Puntero a la solicitud a liberar.
void destruir_solicitud_io_dialfs(t_solicitud_io_dialfs* solicitud);

// Calcula el tamaño en bytes de una estructura de E/S DialFS.
// Parámetro:
//   - io_dialfs: Puntero a la estructura de E/S DialFS.
// Retorna: Tamaño en bytes de la estructura.
uint32_t obtener_tamanio_io_dialfs(t_io_dialfs* io_dialfs);

// Calcula el tamaño en bytes de una solicitud de E/S DialFS.
// Parámetro:
//   - solicitud: Puntero a la solicitud de E/S DialFS.
// Retorna: Tamaño en bytes de la solicitud.
uint32_t obtener_tamanio_solicitud_dialfs(t_solicitud_io_dialfs* solicitud);

// Obtiene el PCB asociado a una solicitud de E/S DialFS.
// Parámetro:
//   - solicitud: Puntero a la solicitud de E/S DialFS.
// Retorna: Puntero al PCB.
t_PCB* obtener_pcb_solicitud_dialfs(t_solicitud_io_dialfs* solicitud);

// Obtiene el nombre de la interfaz de una solicitud de E/S DialFS.
// Parámetro:
//   - solicitud: Puntero a la solicitud de E/S DialFS.
// Retorna: Nombre de la interfaz.
char* obtener_nombre_solicitud_dialfs(t_solicitud_io_dialfs* solicitud);

// Obtiene la estructura de E/S DialFS de una solicitud.
// Parámetro:
//   - solicitud: Puntero a la solicitud de E/S DialFS.
// Retorna: Puntero a la estructura de E/S DialFS.
t_io_dialfs* obtener_io_solicitud_dialfs(t_solicitud_io_dialfs* solicitud);

//===============================================
// FUNCIONES DE RESPONSE
//===============================================

// Crea una nueva estructura de respuesta.
// Parámetros:
//   - process: Indica si el proceso fue exitoso.
//   - pid: Identificador del proceso.
// Retorna: Puntero a la nueva estructura t_response o NULL si falla la asignación.
t_response* create_response(bool process, uint32_t pid);

// Libera la memoria de una estructura de respuesta.
// Parámetro:
//   - response: Puntero a la estructura a liberar.
void delete_response(t_response* response);

// Obtiene el estado del proceso de una respuesta.
// Parámetro:
//   - response: Puntero a la estructura de respuesta.
// Retorna: true si el proceso fue exitoso, false en caso contrario.
bool get_process_response(t_response* response);

// Obtiene el PID asociado a una respuesta.
// Parámetro:
//   - response: Puntero a la estructura de respuesta.
// Retorna: PID del proceso.
uint32_t get_pid_response(t_response* response);

// Calcula el tamaño en bytes de una estructura de respuesta.
// Parámetro:
//   - response: Puntero a la estructura de respuesta.
// Retorna: Tamaño en bytes de la estructura.
uint32_t get_size_response(t_response* response);

//===============================================
// FUNCIONES AUXILIARES
//===============================================

// Obtiene el PCB de una solicitud específica basada en el tipo de interfaz.
// Parámetros:
//   - solicitud: Puntero a la solicitud.
//   - tipo_interfaz: Tipo de interfaz de la solicitud.
// Retorna: Puntero al PCB o NULL si el tipo de interfaz no es reconocido.
t_PCB* obtener_pcb_de_solicitud(void* solicitud, char* tipo_interfaz);

// Destruye una estructura de solicitud IO basada en su tipo.
// Parámetros:
//   - solicitud: Puntero a la solicitud a destruir.
//   - tipo_interfaz: Tipo de interfaz de la solicitud.
void destruir_solicitud_io(void* solicitud, char* tipo_interfaz);

#endif