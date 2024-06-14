// solicitudes_io.h

#ifndef SOLICITUDES_IO_H
#define SOLICITUDES_IO_H

#include "estructuras.h"

typedef struct {
    t_PCB* pcb;
    char* nombre_interfaz;
    t_io_generica* generica;
} t_solicitud_io_generica;

typedef struct {
    char* nombre_interfaz;
    uint32_t tiempo_sleep;
} t_io_generica;

// Funciones para serializar y deserializar las estructuras
void serializar_solicitud_io_generica(t_buffer* buffer, t_solicitud_io_generica* solicitud);
void serializar_io_generica(t_buffer* buffer, t_io_generica* io_generica);
t_solicitud_io_generica* deserializar_solicitud_io_generica(t_buffer* buffer);
t_io_generica* deserializar_io_generica(t_buffer* buffer);

// Funciones para crear y destruir las estructuras
t_solicitud_io_generica* crear_solicitud_io_generica(t_PCB* pcb, char* nombre_interfaz, t_io_generica* generica);
t_io_generica* crear_io_generica(char* nombre_interfaz, uint32_t tiempo_sleep);
void destruir_solicitud_io_generica(t_solicitud_io_generica* solicitud);
void destruir_io_generica(t_io_generica* io_generica);

#endif