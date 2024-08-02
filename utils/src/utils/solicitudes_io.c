#include "solicitudes_io.h"

#include <stdlib.h>
#include <string.h>

//===============================================
// FUNCIONES DE IO GENERICA
//===============================================

t_solicitud_io_generica *crear_solicitud_io_generica(t_PCB *pcb, char *nombre_interfaz, t_io_generica *generica)
{
    t_solicitud_io_generica *solicitud = malloc(sizeof(t_solicitud_io_generica));
    if (solicitud == NULL)
        return NULL;
    solicitud->pcb = pcb;
    solicitud->nombre_interfaz = strdup(nombre_interfaz);
    solicitud->generica = generica;
    return solicitud;
}

t_io_generica *crear_io_generica(char *nombre_interfaz, uint32_t tiempo_sleep, uint32_t pid)
{
    t_io_generica *io_generica = malloc(sizeof(t_io_generica));
    if (io_generica == NULL)
        return NULL;
    io_generica->nombre_interfaz = strdup(nombre_interfaz);
    io_generica->tiempo_sleep = tiempo_sleep;
    io_generica->pid = pid;

    return io_generica;
}

t_solicitud_io_stdin *crear_solicitud_io_stdin(t_PCB *pcb, char *nombre_interfaz, t_io_stdin *io_stdin)
{
    t_solicitud_io_stdin *solicitud = malloc(sizeof(t_solicitud_io_stdin));

    if (solicitud == NULL)
        return NULL;

    solicitud->pcb = pcb;
    solicitud->nombre_interfaz = strdup(nombre_interfaz);
    solicitud->io_stdin = io_stdin;
    return solicitud;
}

t_io_stdin *crear_io_stdin(t_io_frames *io_frames_stdin)
{
    t_io_stdin *io_stdin = malloc(sizeof(t_io_stdin));

    if (io_stdin == NULL)
        return NULL;
    io_stdin->frames_data = io_frames_stdin;

    return io_stdin;
}

t_solicitud_io_stdout *crear_solicitud_io_stdout(t_PCB *pcb, char *nombre_interfaz, t_io_stdout *io_stdout)
{
    t_solicitud_io_stdout *solicitud = malloc(sizeof(t_solicitud_io_stdout));

    if (solicitud == NULL)
        return NULL;

    solicitud->pcb = pcb;
    solicitud->nombre_interfaz = strdup(nombre_interfaz);
    solicitud->io_stdout = io_stdout;
    return solicitud;
}

t_io_stdout *crear_io_stdout(t_io_frames *io_frames_stdout)
{
    t_io_stdout *io_stdout = malloc(sizeof(t_io_stdout));

    if (io_stdout == NULL)
        return NULL;
    io_stdout->frames_data = io_frames_stdout;

    return io_stdout;
}

t_response *create_response(bool process, uint32_t pid)
{
    t_response *new_response = (t_response *)malloc(sizeof(t_response));
    if (new_response == NULL)
    {
        return NULL;
    }
    new_response->process = process;
    new_response->pid = pid;
    return new_response;
}

//===============================================
// FUNCIONES DE ACCESO A CAMPOS
//===============================================

/*********** Functiones 't_io_generica' ***********/
char *obtener_nombre_interfaz_generica(t_io_generica *io_generica)
{
    return io_generica->nombre_interfaz;
}

uint32_t obtener_pid_generica(t_io_generica *io_generica)
{
    return io_generica->pid;
}

uint32_t obtener_tiempo_sleep(t_io_generica *io_generica)
{
    return io_generica->tiempo_sleep;
}

uint32_t obtener_tamanio_io_generica(t_io_generica *io_generica)
{
    return (sizeof(uint32_t) + strlen(io_generica->nombre_interfaz) + 1) + sizeof(io_generica->tiempo_sleep) + sizeof(io_generica->pid);
}

/*********** Functiones 't_io_stdin' ***********/
t_frame_data *obtener_frame_stdin(t_io_stdin *io_stdin, int index)
{
    return get_frame_data(io_stdin->frames_data, index);
}

uint32_t obtener_tamanio_stdin(t_io_stdin *io_stdin)
{
    return get_tamano_total_io_frames(io_stdin->frames_data);
}

uint32_t obtener_pid_stdin(t_io_stdin *io_stdin)
{
    return get_pid_io_frames(io_stdin->frames_data);
}

uint32_t obtener_tamanio_io_stdin(t_io_stdin *io_stdin)
{
    return get_bytes_io_frames(io_stdin->frames_data);
}

/*********** Functiones 't_io_stdout' ***********/

t_frame_data *obtener_frame_stdout(t_io_stdout *io_stdout, int index)
{
    return get_frame_data(io_stdout->frames_data, index);
}

uint32_t obtener_tamanio_stdout(t_io_stdout *io_stdout)
{
    return get_tamano_total_io_frames(io_stdout->frames_data);
}

uint32_t obtener_pid_stdout(t_io_stdout *io_stdout)
{
    return get_pid_io_frames(io_stdout->frames_data);
}

uint32_t obtener_tamanio_io_stdout(t_io_stdout *io_stdout)
{
    return get_bytes_io_frames(io_stdout->frames_data);
}

/*********** Functiones 't_solicitud_io_generica' ***********/
t_PCB *obtener_pcb_solicitud_generica(t_solicitud_io_generica *solicitud)
{
    return solicitud->pcb;
}

char *obtener_nombre_solicitud_generica(t_solicitud_io_generica *solicitud)
{
    return solicitud->nombre_interfaz;
}

t_io_generica *obtener_io_solicitud_generica(t_solicitud_io_generica *solicitud)
{
    return solicitud->generica;
}

uint32_t obtener_tamanio_solicitud_generica(t_solicitud_io_generica *solicitud)
{
    uint32_t size_serialize_pcb = get_pcb_size(solicitud->pcb);
    uint32_t size_serialize_name_io = sizeof(uint32_t) + strlen(solicitud->nombre_interfaz) + 1; // Para el tamaño y la cadena
    uint32_t size_serialize_io_gen = obtener_tamanio_io_generica(solicitud->generica);

    return size_serialize_pcb + size_serialize_name_io + size_serialize_io_gen;
}

/*********** Functiones 't_solicitud_io_stdin' ***********/
t_PCB *obtener_pcb_solicitud_stdin(t_solicitud_io_stdin *solicitud)
{
    return solicitud->pcb;
}

char *obtener_nombre_solicitud_stdin(t_solicitud_io_stdin *solicitud)
{
    return solicitud->nombre_interfaz;
}

t_io_stdin *obtener_io_solicitud_stdin(t_solicitud_io_stdin *solicitud)
{
    return solicitud->io_stdin;
}

uint32_t obtener_tamanio_solicitud_stdin(t_solicitud_io_stdin *solicitud)
{
    uint32_t size_serialize_pcb = get_pcb_size(solicitud->pcb);
    uint32_t size_serialize_name_io = sizeof(uint32_t) + strlen(solicitud->nombre_interfaz) + 1; // Para el tamaño y la cadena
    uint32_t size_serialize_io_stdin = obtener_tamanio_io_stdin(solicitud->io_stdin);

    return size_serialize_pcb + size_serialize_name_io + size_serialize_io_stdin;
}

/*********** Functiones 't_solicitud_io_stdout' ***********/
t_PCB *obtener_pcb_solicitud_stdout(t_solicitud_io_stdout *solicitud)
{
    return solicitud->pcb;
}

char *obtener_nombre_solicitud_stdout(t_solicitud_io_stdout *solicitud)
{
    return solicitud->nombre_interfaz;
}

t_io_stdout *obtener_io_solicitud_stdout(t_solicitud_io_stdout *solicitud)
{
    return solicitud->io_stdout;
}

uint32_t obtener_tamanio_solicitud_stdout(t_solicitud_io_stdout *solicitud)
{
    uint32_t size_serialize_pcb = get_pcb_size(solicitud->pcb);
    uint32_t size_serialize_name_io = sizeof(uint32_t) + strlen(solicitud->nombre_interfaz) + 1; // Para el tamaño y la cadena
    uint32_t size_serialize_io_stdout = obtener_tamanio_io_stdout(solicitud->io_stdout);

    return size_serialize_pcb + size_serialize_name_io + size_serialize_io_stdout;
}

/*********** Functiones 't_response' ***********/
bool get_process_response(t_response *response)
{
    return response->process;
}

uint32_t get_pid_response(t_response *response)
{
    return response->pid;
}

uint32_t get_size_response(t_response *response)
{
    return sizeof(get_process_response(response)) + sizeof(get_pid_response(response));
}

//===============================================
// FUNCIONES DE DESTRUCCIÓN
//===============================================
void destruir_solicitud_io_generica(t_solicitud_io_generica *solicitud)
{
    free(solicitud->nombre_interfaz);
    destruir_io_generica(solicitud->generica);
    free(solicitud);
}

void destruir_io_generica(t_io_generica *io_generica)
{
    free(io_generica->nombre_interfaz);
    free(io_generica);
}

void destruir_solicitud_io_stdin(t_solicitud_io_stdin *solicitud)
{
    free(solicitud->nombre_interfaz);
    destruir_io_stdin(solicitud->io_stdin);
    free(solicitud);
}

void destruir_io_stdin(t_io_stdin *io_stdin)
{
    destroy_io_frames(io_stdin->frames_data);
}

void destruir_solicitud_io_stdout(t_solicitud_io_stdout *solicitud)
{
    free(solicitud->nombre_interfaz);
    destruir_io_stdout(solicitud->io_stdout);
    free(solicitud);
}

void destruir_io_stdout(t_io_stdout *io_stdout)
{
    destroy_io_frames(io_stdout->frames_data);
}

void delete_response(t_response *response)
{
    free(response);
}

//===============================================
// FUNCIONES AUXILIARES
//===============================================

t_PCB *obtener_pcb_de_solicitud(void *solicitud, char *tipo_interfaz)
{
    if (strcmp(tipo_interfaz, "GENERICA") == 0)
    {
        return obtener_pcb_solicitud_generica((t_solicitud_io_generica *)solicitud);
    }
    else if (strcmp(tipo_interfaz, "STDIN") == 0)
    {
        return obtener_pcb_solicitud_stdin((t_solicitud_io_stdin *)solicitud);
    }
    else if (strcmp(tipo_interfaz, "STDOUT") == 0)
    {
        return obtener_pcb_solicitud_stdout((t_solicitud_io_stdout *)solicitud);
    }
    else if (strcmp(tipo_interfaz, "DIALFS") == 0)
    {
        return obtener_pcb_solicitud_dialfs(solicitud);
    }

    return NULL;
}

void destruir_solicitud_io(void *solicitud, char *tipo_interfaz)
{
    if (strcmp(tipo_interfaz, "GENERICA") == 0)
    {
        destruir_solicitud_io_generica((t_solicitud_io_generica *)solicitud);
    }
    else if (strcmp(tipo_interfaz, "STDIN") == 0)
    {
        destruir_solicitud_io_stdin((t_solicitud_io_stdin *)solicitud);
    }
    else if (strcmp(tipo_interfaz, "STDOUT") == 0)
    {
        destruir_solicitud_io_stdout((t_solicitud_io_stdout *)solicitud);
    } /*else if(strcmp(tipo_interfaz, "DIALFS") == 0) {
         destruir_solicitud_io_dialfs((t_solicitud_io_dialfs*)solicitud)
     }*/
}

//===============================================
// FUNCIONES DE IO DIALFS
//===============================================

t_PCB *obtener_pcb_solicitud_dialfs(t_solicitud_io_dialfs *solicitud)
{
    return solicitud->pcb;
}

char *obtener_nombre_solicitud_dialfs(t_solicitud_io_dialfs *solicitud)
{
    return solicitud->nombre_interfaz;
}

t_io_dialfs *obtener_io_solicitud_dialfs(t_solicitud_io_dialfs *solicitud)
{
    return solicitud->io_dialfs;
}

t_solicitud_io_dialfs *crear_solicitud_io_dialfs(t_PCB *pcb, char *nombre_interfaz, t_io_dialfs *io_dialfs)
{
    t_solicitud_io_dialfs *solicitud = malloc(sizeof(t_solicitud_io_dialfs));
    if (solicitud == NULL)
        return NULL;
    solicitud->pcb = pcb;
    solicitud->nombre_interfaz = strdup(nombre_interfaz);
    solicitud->io_dialfs = io_dialfs;
    return solicitud;
}

t_io_dialfs *crear_io_dialfs(uint32_t pid, t_name_instruction operacion, void *generic)
{
    t_io_dialfs *io_dialfs = malloc(sizeof(t_io_dialfs));
    if (io_dialfs == NULL)
        return NULL;

    io_dialfs->pid = pid;
    io_dialfs->operacion = operacion;
    io_dialfs->dialfs_generic = generic;
    return io_dialfs;
}

void destruir_io_dialfs(t_io_dialfs *io_dialfs)
{
    if (io_dialfs != NULL)
    {
        switch (io_dialfs->operacion)
        {
        case IO_FS_CREATE:
        case IO_FS_DELETE:
            destruir_io_dialfs_cd((t_io_dialfs_cd *)io_dialfs->dialfs_generic);
            break;
        case IO_FS_TRUNCATE:
            destruir_io_dialfs_truncate((t_io_dialfs_truncate *)io_dialfs->dialfs_generic);
            break;
        case IO_FS_WRITE:
        case IO_FS_READ:
            destruir_io_dialfs_rw((t_io_dialfs_rw *)io_dialfs->dialfs_generic);
            break;
        default:
            break;
        }
        free(io_dialfs);
    }
}

void destruir_io_dialfs_cd(t_io_dialfs_cd *cd)
{
    if (cd != NULL)
    {
        free(cd->nombre_interfaz);
        free(cd->nombre_archivo);
        free(cd);
    }
}

void destruir_io_dialfs_truncate(t_io_dialfs_truncate *truncate)
{
    if (truncate != NULL)
    {
        free(truncate->nombre_interfaz);
        free(truncate->nombre_archivo);
        free(truncate);
    }
}

void destruir_io_dialfs_rw(t_io_dialfs_rw *rw)
{
    if (rw != NULL)
    {
        free(rw->nombre_interfaz);
        free(rw->nombre_archivo);
        destroy_io_frames(rw->frames_data); // Supongamos que tienes una función para destruir t_io_frames
        free(rw);
    }
}

void destruir_solicitud_io_dialfs(t_solicitud_io_dialfs *solicitud)
{
    if (solicitud != NULL)
    {
        free(solicitud->nombre_interfaz);
        destruir_io_dialfs(solicitud->io_dialfs);
        free(solicitud);
    }
}

uint32_t obtener_tamanio_io_dialfs(t_io_dialfs *dialfs)
{
    uint32_t size = sizeof(dialfs->pid) + sizeof(dialfs->operacion);

    switch (dialfs->operacion)
    {
    case IO_FS_CREATE:
    case IO_FS_DELETE:
        size += sizeof(uint32_t) + strlen(((t_io_dialfs_cd *)dialfs->dialfs_generic)->nombre_interfaz) + 1;
        size += sizeof(uint32_t) + strlen(((t_io_dialfs_cd *)dialfs->dialfs_generic)->nombre_archivo) + 1;
        break;
    case IO_FS_TRUNCATE:
        size += sizeof(uint32_t) + strlen(((t_io_dialfs_truncate *)dialfs->dialfs_generic)->nombre_interfaz) + 1;
        size += sizeof(uint32_t) + strlen(((t_io_dialfs_truncate *)dialfs->dialfs_generic)->nombre_archivo) + 1;
        size += sizeof(uint32_t);
        break;
    case IO_FS_WRITE:
    case IO_FS_READ:
        size += sizeof(uint32_t) + strlen(((t_io_dialfs_rw *)dialfs->dialfs_generic)->nombre_interfaz) + 1;
        size += sizeof(uint32_t) + strlen(((t_io_dialfs_rw *)dialfs->dialfs_generic)->nombre_archivo) + 1;
        size += sizeof(uint32_t);
        size += get_bytes_io_frames(((t_io_dialfs_rw *)dialfs->dialfs_generic)->frames_data);
        size += sizeof(uint32_t);
        break;
    default:
        break;
    }

    return size;
}

uint32_t obtener_tamanio_solicitud_io_dialfs(t_solicitud_io_dialfs *solicitud)
{
    uint32_t size_serialize_pcb = get_pcb_size(solicitud->pcb);
    uint32_t size_serialize_name_io = sizeof(uint32_t) + strlen(solicitud->nombre_interfaz) + 1; // Para el tamaño y la cadena
    uint32_t size_serialize_io_dialfs = obtener_tamanio_io_dialfs(solicitud->io_dialfs);

    return size_serialize_pcb + size_serialize_name_io + size_serialize_io_dialfs;
}

char *get_operation_name(t_name_instruction operacion)
{
    switch (operacion)
    {
        case IO_FS_CREATE:
            return "CREATE";
        case IO_FS_DELETE:
            return "DELETE";
        case IO_FS_TRUNCATE:
            return "TRUNCATE";
        case IO_FS_WRITE:
            return "WRITE";
        case IO_FS_READ:
            return "READ";
        default:
            return "UNKNOWN";
    }
}

t_io_dialfs_cd *crear_io_dialfs_cd(char *nombre_interfaz, char *nombre_archivo)
{
    t_io_dialfs_cd *cd = malloc(sizeof(t_io_dialfs_cd));
    if (cd == NULL)
        return NULL;

    cd->nombre_interfaz = my_strdup(nombre_interfaz);
    cd->nombre_archivo = my_strdup(nombre_archivo);
    return cd;
}

t_io_dialfs_truncate *crear_io_dialfs_truncate(char *nombre_interfaz, char *nombre_archivo, uint32_t tamanio)
{
    t_io_dialfs_truncate *truncate = malloc(sizeof(t_io_dialfs_truncate));
    if (truncate == NULL)
        return NULL;

    truncate->nombre_interfaz = my_strdup(nombre_interfaz);
    truncate->nombre_archivo = my_strdup(nombre_archivo);
    truncate->tamanio = tamanio;
    return truncate;
}

t_io_dialfs_rw *crear_io_dialfs_rw(char *nombre_interfaz, char *nombre_archivo, uint32_t tamanio, t_io_frames *frames_data, uint32_t puntero_archivo)
{
    t_io_dialfs_rw *rw = malloc(sizeof(t_io_dialfs_rw));
    if (rw == NULL)
        return NULL;

    rw->nombre_interfaz = my_strdup(nombre_interfaz);
    rw->nombre_archivo = my_strdup(nombre_archivo);
    rw->tamanio = tamanio;
    rw->frames_data = frames_data;
    rw->puntero_archivo = puntero_archivo;
    return rw;
}


void* get_dialfs_generic(t_io_dialfs *io_dialfs)
{
    if (io_dialfs == NULL) return NULL;

    switch (io_dialfs->operacion)
    {
        case IO_FS_CREATE:
        case IO_FS_DELETE:
            return (t_io_dialfs_cd*) io_dialfs->dialfs_generic;
        case IO_FS_TRUNCATE:
            return (t_io_dialfs_truncate*) io_dialfs->dialfs_generic;
        case IO_FS_WRITE:
        case IO_FS_READ:
            return (t_io_dialfs_rw*) io_dialfs->dialfs_generic;
        default:
            return NULL;
    }
}