#include "solicitudes_io.h"
#include <stdlib.h>
#include <string.h>

#include "solicitudes_io.h"
#include <stdlib.h>
#include <string.h>

//===============================================
// FUNCIONES DE IO GENERICA
//===============================================

t_io_generica* crear_io_generica(char* nombre_interfaz, uint32_t tiempo_sleep, uint32_t pid) {
    t_io_generica* io_generica = malloc(sizeof(t_io_generica));
    if(io_generica == NULL) return NULL;
    io_generica->nombre_interfaz = strdup(nombre_interfaz);
    io_generica->tiempo_sleep = tiempo_sleep;
    io_generica->pid = pid;
    return io_generica;
}

void destruir_io_generica(t_io_generica* io_generica) {
    free(io_generica->nombre_interfaz);
    free(io_generica);
}

char* obtener_nombre_interfaz_generica(t_io_generica* io_generica) {
    return io_generica->nombre_interfaz;
}

uint32_t obtener_pid_generica(t_io_generica* io_generica) {
    return io_generica->pid;
}

uint32_t obtener_tiempo_sleep(t_io_generica* io_generica) {
    return io_generica->tiempo_sleep;
}

uint32_t obtener_tamanio_io_generica(t_io_generica* io_generica) {
    return (sizeof(uint32_t) + strlen(io_generica->nombre_interfaz) + 1) + sizeof(io_generica->tiempo_sleep) + sizeof(io_generica->pid);
}

//===============================================
// FUNCIONES DE IO STDIN
//===============================================

t_io_stdin* crear_io_stdin(uint32_t direccion_logica, uint32_t tamanio, uint32_t pid, char* nombre_interfaz) {
    t_io_stdin* io_stdin = malloc(sizeof(t_io_stdin));
    if(io_stdin == NULL) return NULL;
    io_stdin->direccion_logica = direccion_logica;
    io_stdin->tamanio = tamanio;
    io_stdin->pid = pid;
    io_stdin->nombre_interfaz = strdup(nombre_interfaz);
    return io_stdin;
}

void destruir_io_stdin(t_io_stdin* io_stdin) {
    if (io_stdin != NULL) {
        free(io_stdin->nombre_interfaz);
        free(io_stdin);
    }
}

uint32_t obtener_direccion_logica_stdin(t_io_stdin* io_stdin) {
    return io_stdin->direccion_logica;
}

uint32_t obtener_tamanio_stdin(t_io_stdin* io_stdin) {
    return io_stdin->tamanio;
}

uint32_t obtener_pid_stdin(t_io_stdin* io_stdin) {
    return io_stdin->pid;
}

char* obtener_nombre_interfaz_stdin(t_io_stdin* io_stdin) {
    return io_stdin->nombre_interfaz;
}

uint32_t obtener_tamanio_io_stdin(t_io_stdin* io_stdin) {
    return sizeof(io_stdin->direccion_logica) + sizeof(io_stdin->tamanio) + 
           sizeof(io_stdin->pid) + strlen(io_stdin->nombre_interfaz) + 1;
}

//===============================================
// FUNCIONES DE IO STDOUT
//===============================================

t_io_stdout* crear_io_stdout(uint32_t direccion_logica, uint32_t tamanio, uint32_t pid, char* nombre_interfaz) {
    t_io_stdout* io_stdout = malloc(sizeof(t_io_stdout));
    if(io_stdout == NULL) return NULL;
    io_stdout->direccion_logica = direccion_logica;
    io_stdout->tamanio = tamanio;
    io_stdout->pid = pid;
    io_stdout->nombre_interfaz = strdup(nombre_interfaz);
    return io_stdout;
}

void destruir_io_stdout(t_io_stdout* io_stdout) {
    if (io_stdout != NULL) {
        free(io_stdout->nombre_interfaz);
        free(io_stdout);
    }
}

uint32_t obtener_direccion_logica_stdout(t_io_stdout* io_stdout) {
    return io_stdout->direccion_logica;
}

uint32_t obtener_tamanio_stdout(t_io_stdout* io_stdout) {
    return io_stdout->tamanio;
}

uint32_t obtener_pid_stdout(t_io_stdout* io_stdout) {
    return io_stdout->pid;
}

char* obtener_nombre_interfaz_stdout(t_io_stdout* io_stdout) {
    return io_stdout->nombre_interfaz;
}

uint32_t obtener_tamanio_io_stdout(t_io_stdout* io_stdout) {
    return sizeof(io_stdout->direccion_logica) + sizeof(io_stdout->tamanio) + 
           sizeof(io_stdout->pid) + strlen(io_stdout->nombre_interfaz) + 1;
}

//===============================================
// FUNCIONES DE SOLICITUD IO GENERICA
//===============================================

t_solicitud_io_generica* crear_solicitud_io_generica(t_PCB* pcb, char* nombre_interfaz, t_io_generica* generica) {
    t_solicitud_io_generica* solicitud = malloc(sizeof(t_solicitud_io_generica));
    if(solicitud == NULL) return NULL;
    solicitud->pcb = pcb;
    solicitud->nombre_interfaz = strdup(nombre_interfaz);
    solicitud->generica = generica;
    return solicitud;
}

void destruir_solicitud_io_generica(t_solicitud_io_generica* solicitud) {
    free(solicitud->nombre_interfaz);
    destruir_io_generica(solicitud->generica);
    free(solicitud);
}

t_PCB* obtener_pcb_solicitud_generica(t_solicitud_io_generica* solicitud) {
    return solicitud->pcb;
}

char* obtener_nombre_solicitud_generica(t_solicitud_io_generica* solicitud) {
    return solicitud->nombre_interfaz;
}

t_io_generica* obtener_io_solicitud_generica(t_solicitud_io_generica* solicitud) {
    return solicitud->generica;
}

uint32_t obtener_tamanio_solicitud_generica(t_solicitud_io_generica* solicitud) {
    uint32_t size_serialize_pcb = get_pcb_size(solicitud->pcb);
    uint32_t size_serialize_name_io = sizeof(uint32_t) + strlen(solicitud->nombre_interfaz) + 1;
    uint32_t size_serialize_io_gen = obtener_tamanio_io_generica(solicitud->generica);
    return size_serialize_pcb + size_serialize_name_io + size_serialize_io_gen;
}

//===============================================
// FUNCIONES DE SOLICITUD IO STDIN
//===============================================

t_solicitud_io_stdin* crear_solicitud_io_stdin(t_PCB* pcb, char* nombre_interfaz, t_io_stdin* io_stdin) {
    t_solicitud_io_stdin* solicitud = malloc(sizeof(t_solicitud_io_stdin));
    if(solicitud == NULL) return NULL;
    solicitud->pcb = pcb;
    solicitud->nombre_interfaz = strdup(nombre_interfaz);
    solicitud->io_stdin = io_stdin;
    return solicitud;
}

void destruir_solicitud_io_stdin(t_solicitud_io_stdin* solicitud) {
    if (solicitud != NULL) {
        free(solicitud->nombre_interfaz);
        destruir_io_stdin(solicitud->io_stdin);
        free(solicitud);
    }
}

t_PCB* obtener_pcb_solicitud_stdin(t_solicitud_io_stdin* solicitud) {
    return solicitud->pcb;
}

char* obtener_nombre_solicitud_stdin(t_solicitud_io_stdin* solicitud) {
    return solicitud->nombre_interfaz;
}

t_io_stdin* obtener_io_solicitud_stdin(t_solicitud_io_stdin* solicitud) {
    return solicitud->io_stdin;
}

uint32_t obtener_tamanio_solicitud_stdin(t_solicitud_io_stdin* solicitud) {
    uint32_t size_serialize_pcb = get_pcb_size(solicitud->pcb);
    uint32_t size_serialize_name_io = sizeof(uint32_t) + strlen(solicitud->nombre_interfaz) + 1;
    uint32_t size_serialize_io_stdin = obtener_tamanio_io_stdin(solicitud->io_stdin);
    return size_serialize_pcb + size_serialize_name_io + size_serialize_io_stdin;
}

//===============================================
// FUNCIONES DE SOLICITUD IO STDOUT
//===============================================

t_solicitud_io_stdout* crear_solicitud_io_stdout(t_PCB* pcb, char* nombre_interfaz, t_io_stdout* io_stdout) {
    t_solicitud_io_stdout* solicitud = malloc(sizeof(t_solicitud_io_stdout));
    if(solicitud == NULL) return NULL;
    solicitud->pcb = pcb;
    solicitud->nombre_interfaz = strdup(nombre_interfaz);
    solicitud->io_stdout = io_stdout;
    return solicitud;
}

void destruir_solicitud_io_stdout(t_solicitud_io_stdout* solicitud) {
    if (solicitud != NULL) {
        free(solicitud->nombre_interfaz);
        destruir_io_stdout(solicitud->io_stdout);
        free(solicitud);
    }
}

t_PCB* obtener_pcb_solicitud_stdout(t_solicitud_io_stdout* solicitud) {
    return solicitud->pcb;
}

char* obtener_nombre_solicitud_stdout(t_solicitud_io_stdout* solicitud) {
    return solicitud->nombre_interfaz;
}

t_io_stdout* obtener_io_solicitud_stdout(t_solicitud_io_stdout* solicitud) {
    return solicitud->io_stdout;
}

uint32_t obtener_tamanio_solicitud_stdout(t_solicitud_io_stdout* solicitud) {
    uint32_t size_serialize_pcb = get_pcb_size(solicitud->pcb);
    uint32_t size_serialize_name_io = sizeof(uint32_t) + strlen(solicitud->nombre_interfaz) + 1;
    uint32_t size_serialize_io_stdout = obtener_tamanio_io_stdout(solicitud->io_stdout);
    return size_serialize_pcb + size_serialize_name_io + size_serialize_io_stdout;
}

//===============================================
// FUNCIONES DE IO DIALFS
//===============================================

t_io_dialfs* crear_io_dialfs(char* nombre_interfaz, char* nombre_archivo, uint32_t pid, t_name_instruction operacion, uint32_t tamanio, uint32_t direccion_logica, uint32_t puntero_archivo) {
    t_io_dialfs* io_dialfs = malloc(sizeof(t_io_dialfs));
    if (io_dialfs == NULL) return NULL;
    io_dialfs->nombre_interfaz = strdup(nombre_interfaz);
    io_dialfs->nombre_archivo = strdup(nombre_archivo);
    io_dialfs->pid = pid;
    io_dialfs->operacion = operacion;
    io_dialfs->tamanio = tamanio;
    io_dialfs->direccion_logica = direccion_logica;
    io_dialfs->puntero_archivo = puntero_archivo;
    return io_dialfs;
}

void destruir_io_dialfs(t_io_dialfs* io_dialfs) {
    if (io_dialfs != NULL) {
        free(io_dialfs->nombre_interfaz);
        free(io_dialfs->nombre_archivo);
        free(io_dialfs);
    }
}

t_solicitud_io_dialfs* crear_solicitud_io_dialfs(t_PCB* pcb, char* nombre_interfaz, t_io_dialfs* io_dialfs) {
    t_solicitud_io_dialfs* solicitud = malloc(sizeof(t_solicitud_io_dialfs));
    if (solicitud == NULL) return NULL;
    solicitud->pcb = pcb;
    solicitud->nombre_interfaz = strdup(nombre_interfaz);
    solicitud->io_dialfs = io_dialfs;
    return solicitud;
}

void destruir_solicitud_io_dialfs(t_solicitud_io_dialfs* solicitud) {
    if (solicitud != NULL) {
        free(solicitud->nombre_interfaz);
        destruir_io_dialfs(solicitud->io_dialfs);
        free(solicitud);
    }
}

uint32_t obtener_tamanio_io_dialfs(t_io_dialfs* io_dialfs) {
    uint32_t size = 0;
    size += sizeof(uint32_t) + strlen(io_dialfs->nombre_interfaz) + 1;
    size += sizeof(uint32_t) + strlen(io_dialfs->nombre_archivo) + 1;
    size += sizeof(uint32_t);
    size += sizeof(t_name_instruction);
    size += sizeof(uint32_t);
    size += sizeof(uint32_t);
    size += sizeof(uint32_t);
    return size;
}

uint32_t obtener_tamanio_solicitud_dialfs(t_solicitud_io_dialfs* solicitud) {
    uint32_t size = 0;
    size += get_pcb_size(solicitud->pcb);
    size += sizeof(uint32_t) + strlen(solicitud->nombre_interfaz) + 1;
    size += obtener_tamanio_io_dialfs(solicitud->io_dialfs);
    return size;
}

t_PCB* obtener_pcb_solicitud_dialfs(t_solicitud_io_dialfs* solicitud) {
    return solicitud->pcb;
}

char* obtener_nombre_solicitud_dialfs(t_solicitud_io_dialfs* solicitud) {
    return solicitud->nombre_interfaz;
}

t_io_dialfs* obtener_io_solicitud_dialfs(t_solicitud_io_dialfs* solicitud) {
    return solicitud->io_dialfs;
}

//===============================================
// FUNCIONES DE RESPONSE
//===============================================

t_response* create_response(bool process, uint32_t pid) {
    t_response* new_response = (t_response*)malloc(sizeof(t_response));
    if (new_response == NULL) {
        return NULL;
    }
    new_response->process = process;
    new_response->pid = pid;
    return new_response;
}

void delete_response(t_response* response) {
    free(response);
}

bool get_process_response(t_response* response) {
    return response->process;
}

uint32_t get_pid_response(t_response* response) {
    return response->pid;
}

uint32_t get_size_response(t_response* response) {
    return sizeof(get_process_response(response)) + sizeof(get_pid_response(response));
}

//===============================================
// FUNCIONES AUXILIARES
//===============================================

t_PCB* obtener_pcb_de_solicitud(void* solicitud, char* tipo_interfaz) {    
    if(strcmp(tipo_interfaz, "GENERICA") == 0) {
        return obtener_pcb_solicitud_generica((t_solicitud_io_generica*)solicitud);
    } else if(strcmp(tipo_interfaz, "STDIN") == 0) {
        return obtener_pcb_solicitud_stdin((t_solicitud_io_stdin*)solicitud);
    } else if(strcmp(tipo_interfaz, "STDOUT") == 0) {
        return obtener_pcb_solicitud_stdout((t_solicitud_io_stdout*)solicitud);
    } else if(strcmp(tipo_interfaz, "DIALFS") == 0) {
        return obtener_pcb_solicitud_dialfs((t_solicitud_io_dialfs*)solicitud);
    }
    return NULL;
}

void destruir_solicitud_io(void* solicitud, char* tipo_interfaz) {
    if (strcmp(tipo_interfaz, "GENERICA") == 0) {
        destruir_solicitud_io_generica((t_solicitud_io_generica*)solicitud);
    } else if (strcmp(tipo_interfaz, "STDIN") == 0) {
        destruir_solicitud_io_stdin((t_solicitud_io_stdin*)solicitud);
    } else if (strcmp(tipo_interfaz, "STDOUT") == 0) {
        destruir_solicitud_io_stdout((t_solicitud_io_stdout*)solicitud);
    } else if(strcmp(tipo_interfaz, "DIALFS") == 0) {
        destruir_solicitud_io_dialfs((t_solicitud_io_dialfs*)solicitud);
    }
}