#ifndef SOLICITUD_H_
#define SOLICITUD_H_

#include <stdint.h>
#include <stdlib.h>
#include <variables_globales.h>
#include "utils/solicitudes_io.h"
#include "manager_ios.h"

typedef enum
{   
    SOLICITUD_VACIA,
    SOLICITUD_GENERICA,
    SOLICITUD_STDIN,
    SOLICITUD_STDOUT,
    SOLICITUD_DIALFS
}t_header_solicitud;

typedef struct
{
    t_header_solicitud tipo;
    void* contenido;
}t_solicitud;

extern t_solicitud* solicitud;

t_solicitud* create_solicitud(t_header_solicitud, void*);
void destroy_solicitud();
t_solicitud* get_solicitud();
t_header_solicitud get_tipo();
void* get_contenido(t_solicitud*);
void set_tipo(t_header_solicitud);
void set_contenido(void*);
void procesar_solicitud(t_solicitud*);

#endif /*SOLICITUD_H_*/