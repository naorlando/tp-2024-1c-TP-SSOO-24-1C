#ifndef SOLICITUDES_IO_H
#define SOLICITUDES_IO_H

#include "estructuras.h"

// Definición de estructuras
typedef struct {
    char* nombre_interfaz;
    uint32_t tiempo_sleep;
    uint32_t pid;
} t_io_generica;

typedef struct {
    uint32_t direccion_fisica;
    uint32_t tamanio;
    uint32_t pid;
} t_io_stdin;

typedef struct {
    uint32_t direccion_fisica;
    uint32_t tamanio;
    uint32_t pid;
} t_io_stdout;

// INTERFAZ GENERICA
typedef struct {
    t_PCB* pcb;
    char* nombre_interfaz;
    t_io_generica* generica;
} t_solicitud_io_generica;

// INTERFAZ STDIN
typedef struct {
    t_PCB* pcb;
    char* nombre_interfaz;
    t_io_stdin* io_stdin;
} t_solicitud_io_stdin;

// INTERFAZ STDOUT
typedef struct {
    t_PCB* pcb;
    char* nombre_interfaz;
    t_io_stdout* io_stdout;
} t_solicitud_io_stdout;

//===============================================
// FUNCIONES DE CREACIÓN Y DESTRUCCIÓN
//===============================================

// Función para crear una solicitud de E/S genérica.
// Pre: El PCB, el nombre de interfaz y la E/S genérica deben ser válidos y no NULL.
// Post: Retorna un puntero a una estructura t_solicitud_io_generica creada.
t_solicitud_io_generica* crear_solicitud_io_generica(t_PCB* pcb, char* nombre_interfaz, t_io_generica* generica);

// Función para crear una E/S genérica.
// Pre: El nombre de interfaz, el tiempo de sleep y el pid deben ser válidos.
// Post: Retorna un puntero a una estructura t_io_generica creada.
t_io_generica* crear_io_generica(char* nombre_interfaz, uint32_t tiempo_sleep, uint32_t pid);

// Función para destruir una solicitud de E/S genérica.
// Pre: La solicitud debe ser válida y no NULL.
// Post: La memoria asociada a la solicitud se libera.
void destruir_solicitud_io_generica(t_solicitud_io_generica* solicitud);

// Función para destruir una E/S genérica.
// Pre: La E/S genérica debe ser válida y no NULL.
// Post: La memoria asociada a la E/S genérica se libera.
void destruir_io_generica(t_io_generica* io_generica);

// Función para crear una solicitud de E/S STDIN.
// Pre: El PCB, el nombre de interfaz y la E/S STDIN deben ser válidos y no NULL.
// Post: Retorna un puntero a una estructura t_solicitud_io_stdin creada.
t_solicitud_io_stdin* crear_solicitud_io_stdin(t_PCB* pcb, char* nombre_interfaz, t_io_stdin* io_stdin);

// Función para crear una E/S STDIN.
// Pre: La dirección física, el tamaño y el pid deben ser válidos.
// Post: Retorna un puntero a una estructura t_io_stdin creada.
t_io_stdin* crear_io_stdin(uint32_t direccion_fisica, uint32_t tamanio, uint32_t pid);

// Función para destruir una solicitud de E/S STDIN.
// Pre: La solicitud debe ser válida y no NULL.
// Post: La memoria asociada a la solicitud se libera.
void destruir_solicitud_io_stdin(t_solicitud_io_stdin* solicitud);

// Función para destruir una E/S STDIN.
// Pre: La E/S STDIN debe ser válida y no NULL.
// Post: La memoria asociada a la E/S STDIN se libera.
void destruir_io_stdin(t_io_stdin* io_stdin);

// Función para crear una solicitud de E/S STDOUT.
// Pre: El PCB, el nombre de interfaz y la E/S STDOUT deben ser válidos y no NULL.
// Post: Retorna un puntero a una estructura t_solicitud_io_stdout creada.
t_solicitud_io_stdout* crear_solicitud_io_stdout(t_PCB* pcb, char* nombre_interfaz, t_io_stdout* io_stdout);

// Función para crear una E/S STDOUT.
// Pre: La dirección física, el tamaño y el pid deben ser válidos.
// Post: Retorna un puntero a una estructura t_io_stdout creada.
t_io_stdout* crear_io_stdout(uint32_t direccion_fisica, uint32_t tamanio, uint32_t pid);

// Función para destruir una solicitud de E/S STDOUT.
// Pre: La solicitud debe ser válida y no NULL.
// Post: La memoria asociada a la solicitud se libera.
void destruir_solicitud_io_stdout(t_solicitud_io_stdout* solicitud);

// Función para destruir una E/S STDOUT.
// Pre: La E/S STDOUT debe ser válida y no NULL.
// Post: La memoria asociada a la E/S STDOUT se libera.
void destruir_io_stdout(t_io_stdout* io_stdout);

//===============================================
// FUNCIONES DE ACCESO A CAMPOS
//===============================================

// Obtiene el nombre de la interfaz genérica de una estructura t_io_generica.
// Pre: El puntero io_generica debe apuntar a una estructura t_io_generica válida y no debe ser NULL.
// Post: Retorna un puntero a una cadena de caracteres que contiene el nombre de la interfaz.
char* obtener_nombre_interfaz_generica(t_io_generica* io_generica);

// Obtiene el tiempo de sleep de una estructura t_io_generica.
// Pre: El puntero io_generica debe apuntar a una estructura t_io_generica válida y no debe ser NULL.
// Post: Retorna el tiempo de sleep como un valor uint32_t.
uint32_t obtener_tiempo_sleep(t_io_generica* io_generica);

// Obtiene el PID asociado a una io_generica.
// Pre: El puntero io_generica debe apuntar a una estructura io_generica válida y no debe ser NULL.
// Post: Retorna un valor de tipo uint32_t que contiene el PID asociado.
uint32_t obtener_pid_generica(t_io_generica* io_generica);

// Obtiene la dirección física del stdin de una estructura t_io_stdin.
// Pre: El puntero io_stdin debe apuntar a una estructura t_io_stdin válida y no debe ser NULL.
// Post: Retorna la dirección física del stdin como un valor uint32_t.
uint32_t obtener_direccion_fisica_stdin(t_io_stdin* io_stdin);

// Obtiene el tamaño del stdin de una estructura t_io_stdin.
// Pre: El puntero io_stdin debe apuntar a una estructura t_io_stdin válida y no debe ser NULL.
// Post: Retorna el tamaño del stdin como un valor uint32_t.
uint32_t obtener_tamanio_stdin(t_io_stdin* io_stdin);

// Obtiene el PID asociado a una io_stdin.
// Pre: El puntero io_stdin debe apuntar a una estructura io_stdin válida y no debe ser NULL.
// Post: Retorna un valor de tipo uint32_t que contiene el PID asociado.
uint32_t obtener_pid_stdin(t_io_stdin* io_stdin);

// Obtiene la dirección física del stdout de una estructura t_io_stdout.
// Pre: El puntero io_stdout debe apuntar a una estructura t_io_stdout válida y no debe ser NULL.
// Post: Retorna la dirección física del stdout como un valor uint32_t.
uint32_t obtener_direccion_fisica_stdout(t_io_stdout* io_stdout);

// Obtiene el tamaño del stdout de una estructura t_io_stdout.
// Pre: El puntero io_stdout debe apuntar a una estructura t_io_stdout válida y no debe ser NULL.
// Post: Retorna el tamaño del stdout como un valor uint32_t.
uint32_t obtener_tamanio_stdout(t_io_stdout* io_stdout);

// Obtiene el PID asociado a una io_stdout.
// Pre: El puntero io_stdout debe apuntar a una estructura t_io_stdout válida y no debe ser NULL.
// Post: Retorna un valor de tipo uint32_t que contiene el PID asociado.
uint32_t obtener_pid_stdout(t_io_stdout* io_stdout);

// Obtiene el PCB de una solicitud de I/O genérica.
// Pre: El puntero solicitud debe apuntar a una estructura t_solicitud_io_generica válida y no debe ser NULL.
// Post: Retorna un puntero a una estructura t_PCB.
t_PCB* obtener_pcb_solicitud_generica(t_solicitud_io_generica* solicitud);

// Obtiene el nombre de la solicitud de I/O genérica.
// Pre: El puntero solicitud debe apuntar a una estructura t_solicitud_io_generica válida y no debe ser NULL.
// Post: Retorna un puntero a una cadena de caracteres que contiene el nombre de la solicitud.
char* obtener_nombre_solicitud_generica(t_solicitud_io_generica* solicitud);

// Obtiene la interfaz de I/O genérica de una solicitud.
// Pre: El puntero solicitud debe apuntar a una estructura t_solicitud_io_generica válida y no debe ser NULL.
// Post: Retorna un puntero a una estructura t_io_generica.
t_io_generica* obtener_io_solicitud_generica(t_solicitud_io_generica* solicitud);

// Obtiene el PCB de una solicitud de stdin.
// Pre: El puntero solicitud debe apuntar a una estructura t_solicitud_io_stdin válida y no debe ser NULL.
// Post: Retorna un puntero a una estructura t_PCB.
t_PCB* obtener_pcb_solicitud_stdin(t_solicitud_io_stdin* solicitud);

// Obtiene el nombre de la solicitud de stdin.
// Pre: El puntero solicitud debe apuntar a una estructura t_solicitud_io_stdin válida y no debe ser NULL.
// Post: Retorna un puntero a una cadena de caracteres que contiene el nombre de la solicitud.
char* obtener_nombre_solicitud_stdin(t_solicitud_io_stdin* solicitud);

// Obtiene la interfaz de stdin de una solicitud.
// Pre: El puntero solicitud debe apuntar a una estructura t_solicitud_io_stdin válida y no debe ser NULL.
// Post: Retorna un puntero a una estructura t_io_stdin.
t_io_stdin* obtener_io_solicitud_stdin(t_solicitud_io_stdin* solicitud);

// Obtiene el PCB de una solicitud de stdout.
// Pre: El puntero solicitud debe apuntar a una estructura t_solicitud_io_stdout válida y no debe ser NULL.
// Post: Retorna un puntero a una estructura t_PCB.
t_PCB* obtener_pcb_solicitud_stdout(t_solicitud_io_stdout* solicitud);

// Obtiene el nombre de la solicitud de stdout.
// Pre: El puntero solicitud debe apuntar a una estructura t_solicitud_io_stdout válida y no debe ser NULL.
// Post: Retorna un puntero a una cadena de caracteres que contiene el nombre de la solicitud.
char* obtener_nombre_solicitud_stdout(t_solicitud_io_stdout* solicitud);

// Obtiene la interfaz de stdout de una solicitud.
// Pre: El puntero solicitud debe apuntar a una estructura t_solicitud_io_stdout válida y no debe ser NULL.
// Post: Retorna un puntero a una estructura t_io_stdout.
t_io_stdout* obtener_io_solicitud_stdout(t_solicitud_io_stdout* solicitud);

// Obtiene el tamaño de una solicitud de stdout.
// Pre: El puntero solicitud debe apuntar a una estructura t_solicitud_io_stdout válida y no debe ser NULL.
// Post: Retorna el tamaño de la solicitud de stdout como un valor uint32_t.
uint32_t obtener_tamanio_solicitud_stdout(t_solicitud_io_stdout* solicitud);

// Obtiene el tamaño de una solicitud de stdin.
// Pre: El puntero solicitud debe apuntar a una estructura t_solicitud_io_stdin válida y no debe ser NULL.
// Post: Retorna el tamaño de la solicitud de stdin como un valor uint32_t.
uint32_t obtener_tamanio_solicitud_stdin(t_solicitud_io_stdin* solicitud);

// Obtiene el tamaño de una solicitud de I/O genérica.
// Pre: El puntero solicitud debe apuntar a una estructura t_solicitud_io_generica válida y no debe ser NULL.
// Post: Retorna el tamaño de la solicitud de I/O genérica como un valor uint32_t.
uint32_t obtener_tamanio_solicitud_generica(t_solicitud_io_generica* solicitud);

// Obtiene el tamaño de una interfaz de I/O stdout.
// Pre: El puntero io_stdout debe apuntar a una estructura t_io_stdout válida y no debe ser NULL.
// Post: Retorna el tamaño de la interfaz de I/O stdout como un valor uint32_t.
uint32_t obtener_tamanio_io_stdout(t_io_stdout* io_stdout);

// Obtiene el tamaño de una interfaz de I/O stdin.
// Pre: El puntero io_stdin debe apuntar a una estructura t_io_stdin válida y no debe ser NULL.
// Post: Retorna el tamaño de la interfaz de I/O stdin como un valor uint32_t.
uint32_t obtener_tamanio_io_stdin(t_io_stdin* io_stdin);

// Obtiene el tamaño de una interfaz de I/O genérica.
// Pre: El puntero io_generica debe apuntar a una estructura t_io_generica válida y no debe ser NULL.
// Post: Retorna el tamaño de la interfaz de I/O genérica como un valor uint32_t.
uint32_t obtener_tamanio_io_generica(t_io_generica* io_generica);

#endif