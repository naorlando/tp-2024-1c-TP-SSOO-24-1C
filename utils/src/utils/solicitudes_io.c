#include "solicitudes_io.h"
#include <stdlib.h>
#include <string.h>

//===============================================
// FUNCIONES DE IO GENERICA
//===============================================

t_solicitud_io_generica* crear_solicitud_io_generica(t_PCB* pcb, char* nombre_interfaz, t_io_generica* generica) {
    t_solicitud_io_generica* solicitud = malloc(sizeof(t_solicitud_io_generica));
    if(solicitud == NULL) return NULL;
    solicitud->pcb = pcb;
    solicitud->nombre_interfaz = strdup(nombre_interfaz);
    solicitud->generica = generica;
    return solicitud;
}

t_io_generica* crear_io_generica(char* nombre_interfaz, uint32_t tiempo_sleep) {
    t_io_generica* io_generica = malloc(sizeof(t_io_generica));
    if(io_generica == NULL) return NULL;
    io_generica->nombre_interfaz = strdup(nombre_interfaz);
    io_generica->tiempo_sleep = tiempo_sleep;
    return io_generica;
}

void destruir_solicitud_io_generica(t_solicitud_io_generica* solicitud) {
    free(solicitud->nombre_interfaz);
    destruir_io_generica(solicitud->generica);
    free(solicitud);
}

void destruir_io_generica(t_io_generica* io_generica) {
    free(io_generica->nombre_interfaz);
    free(io_generica);
}

char* obtener_nombre_interfaz_generica(t_io_generica* io_generica) {
    return io_generica->nombre_interfaz;
}

uint32_t obtener_tiempo_sleep(t_io_generica* io_generica) {
    return io_generica->tiempo_sleep;
}

uint32_t obtener_tamanio_io_generica(t_io_generica* io_generica) {
    return (sizeof(uint32_t) + strlen(io_generica->nombre_interfaz) + 1) + sizeof(io_generica->tiempo_sleep);
}

//===============================================
// FUNCIONES DE IO STDIN
//===============================================

t_solicitud_io_stdin* crear_solicitud_io_stdin(t_PCB* pcb, char* nombre_interfaz, t_io_stdin* io_stdin) {
    t_solicitud_io_stdin* solicitud = malloc(sizeof(t_solicitud_io_stdin));
    if(solicitud == NULL) return NULL;
    solicitud->pcb = pcb;
    solicitud->nombre_interfaz = strdup(nombre_interfaz);
    solicitud->io_stdin = io_stdin;
    return solicitud;
}

t_io_stdin* crear_io_stdin(uint32_t direccion_fisica, uint32_t tamanio) {
    t_io_stdin* io_stdin = malloc(sizeof(t_io_stdin));
    if(io_stdin == NULL) return NULL;
    io_stdin->direccion_fisica = direccion_fisica;
    io_stdin->tamanio = tamanio;
    return io_stdin;
}

void destruir_solicitud_io_stdin(t_solicitud_io_stdin* solicitud) {
    free(solicitud->nombre_interfaz);
    destruir_io_stdin(solicitud->io_stdin);
    free(solicitud);
}

void destruir_io_stdin(t_io_stdin* io_stdin) {
    free(io_stdin);
}

uint32_t obtener_direccion_fisica_stdin(t_io_stdin* io_stdin) {
    return io_stdin->direccion_fisica;
}

uint32_t obtener_tamanio_io_stdin(t_io_stdin* io_stdin) {
    return sizeof(io_stdin->direccion_fisica) + sizeof(io_stdin->tamanio);
}

//===============================================
// FUNCIONES DE IO STDOUT
//===============================================

t_solicitud_io_stdout* crear_solicitud_io_stdout(t_PCB* pcb, char* nombre_interfaz, t_io_stdout* io_stdout) {
    t_solicitud_io_stdout* solicitud = malloc(sizeof(t_solicitud_io_stdout));
    if(solicitud == NULL) return NULL;
    solicitud->pcb = pcb;
    solicitud->nombre_interfaz = strdup(nombre_interfaz);
    solicitud->io_stdout = io_stdout;
    return solicitud;
}

t_io_stdout* crear_io_stdout(uint32_t direccion_fisica, uint32_t tamanio) {
    t_io_stdout* io_stdout = malloc(sizeof(t_io_stdout));
    if(io_stdout == NULL) return NULL;
    io_stdout->direccion_fisica = direccion_fisica;
    io_stdout->tamanio = tamanio;
    return io_stdout;
}

void destruir_solicitud_io_stdout(t_solicitud_io_stdout* solicitud) {
    free(solicitud->nombre_interfaz);
    destruir_io_stdout(solicitud->io_stdout);
    free(solicitud);
}

void destruir_io_stdout(t_io_stdout* io_stdout) {
    free(io_stdout);
}

uint32_t obtener_direccion_fisica_stdout(t_io_stdout* io_stdout) {
    return io_stdout->direccion_fisica;
}

uint32_t obtener_tamanio_io_stdout(t_io_stdout* io_stdout) {
    return sizeof(io_stdout->direccion_fisica) + sizeof(io_stdout->tamanio);
}

//===============================================
// FUNCIONES DE IO DIALFS
//===============================================

t_solicitud_io_dialfs* crear_solicitud_io_dialfs(t_PCB* pcb, char* nombre_interfaz, t_io_dialfs* io_dialfs) {
    t_solicitud_io_dialfs* solicitud = malloc(sizeof(t_solicitud_io_dialfs));
    if (solicitud == NULL) return NULL;
    solicitud->pcb = pcb;
    solicitud->nombre_interfaz = strdup(nombre_interfaz);
    solicitud->io_dialfs = io_dialfs;
    return solicitud;
}

t_io_dialfs* crear_io_dialfs(char* nombre_archivo, uint32_t tamanio, uint32_t offset, void* datos, t_name_instruction operacion) {
    t_io_dialfs* io_dialfs = malloc(sizeof(t_io_dialfs));
    if (io_dialfs == NULL) return NULL;
    io_dialfs->nombre_archivo = strdup(nombre_archivo);
    io_dialfs->tamanio = tamanio;
    io_dialfs->offset = offset;
    io_dialfs->datos = datos;
    io_dialfs->operacion = operacion;
    return io_dialfs;
}

void destruir_solicitud_io_dialfs(t_solicitud_io_dialfs* solicitud) {
    if (solicitud != NULL) {
        free(solicitud->nombre_interfaz);
        destruir_io_dialfs(solicitud->io_dialfs);
        free(solicitud);
    }
}

void destruir_io_dialfs(t_io_dialfs* io_dialfs) {
    if (io_dialfs != NULL) {
        free(io_dialfs->nombre_archivo);
        free(io_dialfs->datos);
        free(io_dialfs);
    }
}

uint32_t obtener_tamanio_io_dialfs(t_io_dialfs* io_dialfs) {
    uint32_t size = 0;
    
    size += sizeof(uint32_t) + strlen(io_dialfs->nombre_archivo) + 1; // Nombre del archivo
    size += sizeof(uint32_t); // Tamaño
    size += sizeof(uint32_t); // Offset
    size += sizeof(uint32_t); // Operación
    
    // Si es una operación de escritura, incluimos el tamaño de los datos
    if (io_dialfs->operacion == IO_FS_WRITE) {
        size += io_dialfs->tamanio;
    }
    
    return size;
}