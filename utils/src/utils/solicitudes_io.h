#ifndef SOLICITUDES_IO_H
#define SOLICITUDES_IO_H

#include "estructuras.h"

// INTERFAZ GENERICA
typedef struct {
    t_PCB* pcb;
    char* nombre_interfaz;
    t_io_generica* generica;
} t_solicitud_io_generica;

typedef struct {
    char* nombre_interfaz;
    uint32_t tiempo_sleep;
} t_io_generica;

// INTERFAZ STDIN
typedef struct {
    t_PCB* pcb;
    char* nombre_interfaz;
    t_io_stdin* stdin;
} t_solicitud_io_stdin;

typedef struct {
    uint32_t direccion_fisica;
    uint32_t tamanio;
} t_io_stdin;

// INTERFAZ STDOUT
typedef struct {
    t_PCB* pcb;
    char* nombre_interfaz;
    t_io_stdout* stdout;
} t_solicitud_io_stdout;

typedef struct {
    uint32_t direccion_fisica;
    uint32_t tamanio;
} t_io_stdout;


//===============================================
// FUNCIONES DE SERIALIZACION Y DESERIALIZACIÓN
//===============================================

void serializar_solicitud_io_generica(t_buffer* buffer, t_solicitud_io_generica* solicitud);
void serializar_io_generica(t_buffer* buffer, t_io_generica* io_generica);
t_solicitud_io_generica* deserializar_solicitud_io_generica(t_buffer* buffer);
t_io_generica* deserializar_io_generica(t_buffer* buffer);

void serializar_solicitud_io_stdin(t_buffer* buffer, t_solicitud_io_stdin* solicitud);
void serializar_io_stdin(t_buffer* buffer, t_io_stdin* io_stdin);
t_solicitud_io_stdin* deserializar_solicitud_io_stdin(t_buffer* buffer);
t_io_stdin* deserializar_io_stdin(t_buffer* buffer);

void serializar_solicitud_io_stdout(t_buffer* buffer, t_solicitud_io_stdout* solicitud);
void serializar_io_stdout(t_buffer* buffer, t_io_stdout* io_stdout);
t_solicitud_io_stdout* deserializar_solicitud_io_stdout(t_buffer* buffer);
t_io_stdout* deserializar_io_stdout(t_buffer* buffer);

//===============================================
//     FUNCIONES DE CREACIÓN Y DESTRUCCIÓN
//===============================================

t_solicitud_io_generica* crear_solicitud_io_generica(t_PCB* pcb, char* nombre_interfaz, t_io_generica* generica);
t_io_generica* crear_io_generica(char* nombre_interfaz, uint32_t tiempo_sleep);
void destruir_solicitud_io_generica(t_solicitud_io_generica* solicitud);
void destruir_io_generica(t_io_generica* io_generica);

t_solicitud_io_stdin* crear_solicitud_io_stdin(t_PCB* pcb, char* nombre_interfaz, t_io_stdin* stdin);
t_io_stdin* crear_io_stdin(uint32_t direccion_fisica, uint32_t tamanio);
void destruir_solicitud_io_stdin(t_solicitud_io_stdin* solicitud);
void destruir_io_stdin(t_io_stdin* io_stdin);

t_solicitud_io_stdout* crear_solicitud_io_stdout(t_PCB* pcb, char* nombre_interfaz, t_io_stdout* stdout);
t_io_stdout* crear_io_stdout(uint32_t direccion_fisica, uint32_t tamanio);
void destruir_solicitud_io_stdout(t_solicitud_io_stdout* solicitud);
void destruir_io_stdout(t_io_stdout* io_stdout);

#endif