#include "solicitudes_io.h"
#include "buffer.h"

// Serialización
void serializar_solicitud_io_generica(t_buffer* buffer, t_solicitud_io_generica* solicitud) {
    serializar_pcb(buffer, solicitud->pcb);
    buffer_add_string(buffer, solicitud->nombre_interfaz);
    serializar_io_generica(buffer, solicitud->generica);
}

void serializar_io_generica(t_buffer* buffer, t_io_generica* io_generica) {
    buffer_add_string(buffer, io_generica->nombre_interfaz);
    buffer_add_uint32(buffer, io_generica->tiempo_sleep);
}

// Deserialización
t_solicitud_io_generica* deserializar_solicitud_io_generica(t_buffer* buffer) {
    t_PCB* pcb = deserializar_pcb(buffer);
    char* nombre_interfaz = extract_string_buffer(buffer);
    t_io_generica* generica = deserializar_io_generica(buffer);
    return crear_solicitud_io_generica(pcb, nombre_interfaz, generica);
}

t_io_generica* deserializar_io_generica(t_buffer* buffer) {
    char* nombre_interfaz = extract_string_buffer(buffer);
    uint32_t tiempo_sleep = buffer_read_uint32(buffer);
    return crear_io_generica(nombre_interfaz, tiempo_sleep);
}

// Creación
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

// Destrucción
void destruir_solicitud_io_generica(t_solicitud_io_generica* solicitud) {
    free(solicitud->nombre_interfaz);
    destruir_io_generica(solicitud->generica);
    free(solicitud);
}

void destruir_io_generica(t_io_generica* io_generica) {
    free(io_generica->nombre_interfaz);
    free(io_generica);
}