#include "solicitud.h"

t_solicitud* solicitud;

t_solicitud* create_solicitud(t_header_solicitud tipo, void* contenido)
{
    t_solicitud* solicitud = malloc(sizeof(t_solicitud));

    if(solicitud == NULL)
    {
        return NULL;
    }

    solicitud->tipo = tipo;
    solicitud->contenido = contenido;

    return solicitud;
}

t_solicitud* get_solicitud()
{
    if(solicitud == NULL){
        solicitud = create_solicitud(SOLICITUD_VACIA, NULL);
    }

    return solicitud;
}

void destroy_solicitud()
{
    if(solicitud != NULL) {
        free(solicitud);
    }
}

t_header_solicitud get_tipo()
{
    return solicitud->tipo;
}

void* get_contenido(t_solicitud* solicitud_a_procesar)
{
    return solicitud_a_procesar->contenido;
}

void set_tipo(t_header_solicitud tipo)
{
    solicitud->tipo = tipo;
}

void set_contenido(void* contenido)
{
    solicitud->contenido = contenido;
}

bool procesar_solicitud(t_solicitud* solicitud_a_procesar)
{
    t_IO_connection* io_connection;
    void* proceso;

    switch(get_tipo(solicitud_a_procesar)){
        case SOLICITUD_VACIA:
            
            break;
        case SOLICITUD_GENERICA:
            t_solicitud_io_generica* solicitud_gen = (t_solicitud_io_generica*)get_contenido(solicitud_a_procesar);
            io_connection = get_IO_connection(obtener_nombre_solicitud_generica(solicitud_gen));

            proceso = solicitud_gen;
            break;
        case SOLICITUD_STDIN:
            t_solicitud_io_stdin* solicitud_stdin = (t_solicitud_io_stdin*)get_contenido(solicitud_a_procesar);
            io_connection = get_IO_connection(obtener_nombre_solicitud_stdin(solicitud_stdin));

            proceso = solicitud_stdin;
            break;
        case SOLICITUD_STDOUT:
            t_solicitud_io_stdout* solicitud_stdout = (t_solicitud_io_stdout*)get_contenido(solicitud_a_procesar);
            io_connection = get_IO_connection(obtener_nombre_solicitud_stdout(solicitud_stdout));

            proceso = solicitud_stdout;
            break;
        case SOLICITUD_DIALFS:
            /*t_solicitud_io_dialfs* solicitud_dialfs = (t_solicitud_io_dialfs*)get_contenido(solicitud_a_procesar);
            io_connection = get_IO_connection(obtener_nombre_solicitud_generica(solicitud_dialfs));

            proceso = solicitud_dialfs;*/
            break;
        default:
            //caso de error
            break;
    }

    if(io_connection != NULL) {
        return agregar_proceso_bloqueado(io_connection, proceso);  
    }

    return false;
}

t_PCB* obtener_pcb_solicitud(t_solicitud* solicitud)
{
    t_PCB* pcb;

    switch(get_tipo(solicitud)){
        case SOLICITUD_VACIA:
            
            break;
        case SOLICITUD_GENERICA:
        case SOLICITUD_STDIN:
        case SOLICITUD_STDOUT:
        case SOLICITUD_DIALFS:
            //TODO: REALIZAR UNA FUNCION QUE PERMITA OBTENER EL TIPO DE INTERFAZ QUE SE RECIBE
            pcb = obtener_pcb_de_solicitud(get_contenido(solicitud), "GENERICA");
            break;
        default:
            //caso de error
            break;
    }
    return pcb;
}