#include "solicitudes_io.h"
#include "protocolo.h"

//===============================================
// FUNCIONES DE SERIALIZACION
//===============================================
void serializar_solicitud_io_generica(t_buffer* buffer, t_solicitud_io_generica* solicitud) {
    serialize_pcb(buffer, solicitud->pcb);
    buffer_add_string(buffer, solicitud->nombre_interfaz);
    serializar_io_generica(buffer, solicitud->generica);
}

void serializar_io_generica(t_buffer* buffer, t_io_generica* io_generica) {
    buffer_add_string(buffer, io_generica->nombre_interfaz);
    buffer_add_uint32(buffer, io_generica->tiempo_sleep);
}

void serializar_solicitud_io_stdin(t_buffer* buffer, t_solicitud_io_stdin* solicitud) {
    serialize_pcb(buffer, solicitud->pcb);
    buffer_add_string(buffer, solicitud->nombre_interfaz);
    serializar_io_stdin(buffer, solicitud->stdin);
}

void serializar_io_stdin(t_buffer* buffer, t_io_stdin* io_stdin) {
    buffer_add_uint32(buffer, io_stdin->direccion_fisica);
    buffer_add_uint32(buffer, io_stdin->tamanio);
}

void serializar_solicitud_io_stdout(t_buffer* buffer, t_solicitud_io_stdout* solicitud) {
    serialize_pcb(buffer, solicitud->pcb);
    buffer_add_string(buffer, solicitud->nombre_interfaz);
    serializar_io_stdout(buffer, solicitud->stdout);
}

void serializar_io_stdout(t_buffer* buffer, t_io_stdout* io_stdout) {
    buffer_add_uint32(buffer, io_stdout->direccion_fisica);
    buffer_add_uint32(buffer, io_stdout->tamanio);
}

//===============================================
// FUNCIONES DE DESERIALIZACIÓN
//===============================================
t_solicitud_io_generica* deserializar_solicitud_io_generica(t_buffer* buffer) {
    t_PCB* pcb = deserialize_pcb(buffer);
    char* nombre_interfaz = extract_string_buffer(buffer);
    t_io_generica* generica = deserializar_io_generica(buffer);
    return crear_solicitud_io_generica(pcb, nombre_interfaz, generica);
}

t_io_generica* deserializar_io_generica(t_buffer* buffer) {
    char* nombre_interfaz = extract_string_buffer(buffer);
    uint32_t tiempo_sleep = buffer_read_uint32(buffer);
    return crear_io_generica(nombre_interfaz, tiempo_sleep);
}

t_solicitud_io_stdin* deserializar_solicitud_io_stdin(t_buffer* buffer) {
    t_PCB* pcb = deserialize_pcb(buffer);
    char* nombre_interfaz = extract_string_buffer(buffer);
    t_io_stdin* stdin = deserializar_io_stdin(buffer);
    return crear_solicitud_io_stdin(pcb, nombre_interfaz, stdin);
}

t_io_stdin* deserializar_io_stdin(t_buffer* buffer) {
    uint32_t direccion_fisica = buffer_read_uint32(buffer);
    uint32_t tamanio = buffer_read_uint32(buffer);
    return crear_io_stdin(direccion_fisica, tamanio);
}

t_solicitud_io_stdout* deserializar_solicitud_io_stdout(t_buffer* buffer) {
    t_PCB* pcb = deserialize_pcb(buffer);
    char* nombre_interfaz = extract_string_buffer(buffer);
    t_io_stdout* stdout = deserializar_io_stdout(buffer);
    return crear_solicitud_io_stdout(pcb, nombre_interfaz, stdout);
}

t_io_stdout* deserializar_io_stdout(t_buffer* buffer) {
    uint32_t direccion_fisica = buffer_read_uint32(buffer);
    uint32_t tamanio = buffer_read_uint32(buffer);
    return crear_io_stdout(direccion_fisica, tamanio);
}

//===============================================
// FUNCIONES DE CREACIÓN
//===============================================
t_solicitud_io_generica* crear_solicitud_io_generica(t_PCB* pcb, char* nombre_interfaz, t_io_generica* generica) {
    t_solicitud_io_generica* solicitud = malloc(sizeof(t_solicitud_io_generica));
    solicitud->pcb = pcb;
    solicitud->nombre_interfaz = strdup(nombre_interfaz);
    solicitud->generica = generica;
    return solicitud;
}

t_io_generica* crear_io_generica(char* nombre_interfaz, uint32_t tiempo_sleep) {
    t_io_generica* io_generica = malloc(sizeof(t_io_generica));
    io_generica->nombre_interfaz = strdup(nombre_interfaz);
    io_generica->tiempo_sleep = tiempo_sleep;
    return io_generica;
}

t_solicitud_io_stdin* crear_solicitud_io_stdin(t_PCB* pcb, char* nombre_interfaz, t_io_stdin* stdin) {
    t_solicitud_io_stdin* solicitud = malloc(sizeof(t_solicitud_io_stdin));
    solicitud->pcb = pcb;
    solicitud->nombre_interfaz = strdup(nombre_interfaz);
    solicitud->stdin = stdin;
    return solicitud;
}

t_io_stdin* crear_io_stdin(uint32_t direccion_fisica, uint32_t tamanio) {
    t_io_stdin* io_stdin = malloc(sizeof(t_io_stdin));
    io_stdin->direccion_fisica = direccion_fisica;
    io_stdin->tamanio = tamanio;
    return io_stdin;
}

t_solicitud_io_stdout* crear_solicitud_io_stdout(t_PCB* pcb, char* nombre_interfaz, t_io_stdout* stdout) {
    t_solicitud_io_stdout* solicitud = malloc(sizeof(t_solicitud_io_stdout));
    solicitud->pcb = pcb;
    solicitud->nombre_interfaz = strdup(nombre_interfaz);
    solicitud->stdout = stdout;
    return solicitud;
}

t_io_stdout* crear_io_stdout(uint32_t direccion_fisica, uint32_t tamanio) {
    t_io_stdout* io_stdout = malloc(sizeof(t_io_stdout));
    io_stdout->direccion_fisica = direccion_fisica;
    io_stdout->tamanio = tamanio;
    return io_stdout;
}

//===============================================
// FUNCIONES DE DESTRUCCIÓN
//===============================================
void destruir_solicitud_io_generica(t_solicitud_io_generica* solicitud) {
    free(solicitud->nombre_interfaz);
    destruir_io_generica(solicitud->generica);
    free(solicitud);
}

void destruir_io_generica(t_io_generica* io_generica) {
    free(io_generica->nombre_interfaz);
    free(io_generica);
}

void destruir_solicitud_io_stdin(t_solicitud_io_stdin* solicitud) {
    free(solicitud->nombre_interfaz);
    destruir_io_stdin(solicitud->stdin);
    free(solicitud);
}

void destruir_io_stdin(t_io_stdin* io_stdin) {
    free(io_stdin);
}

void destruir_solicitud_io_stdout(t_solicitud_io_stdout* solicitud) {
    free(solicitud->nombre_interfaz);
    destruir_io_stdout(solicitud->stdout);
    free(solicitud);
}

void destruir_io_stdout(t_io_stdout* io_stdout) {
    free(io_stdout);
}