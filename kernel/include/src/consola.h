#ifndef CONSOLA_H_
#define CONSOLA_H_

#include <stdlib.h>
#include <variables_globales.h>
#include "utils/estructuras.h"
#include "utils/protocolo.h"
#include "utils/utils.h"
#include "table_pcb.h"
#include "protocolo_kernel.h"
#include "largo_plazo.h"
#include "syncro.h"

typedef struct {
    uint16_t nuevo_grado;
} t_multiprogramacion_args;


void iniciar_consola_interactiva();
bool _validacion_de_instrucciones_consola(char*);
void _atender_instruccion(void*);

// FORMAS DE ATENDER PROCESO:
void f_iniciar_proceso(char*);
void f_ejecutar_script(const char*);

int asignar_pid();

// ----------------------------------------------
//      PROCESO_ESTADO:
// ----------------------------------------------
// funcion para atender el comando de la consola: PROCESO_ESTADO
void f_mostrar_estado_procesos();
// funcion para listar los PID segun su estado en el diccionarios de PCBs
// pre: estado a buscar
// post: estado : [lista de pids]
void listar_pids_por_estado(t_state state);

// ----------------------------------------------
//      FINALIZAR_PROCESO:
// ----------------------------------------------
void f_finalizar_proceso(u_int32_t pid);
void eliminar_de_listas_de_estado(t_PCB* pcb);
void eliminar_de_new(t_PCB* pcb);
void eliminar_de_ready(t_PCB* pcb);
void eliminar_de_blocked_io(t_PCB* pcb);

// ----------------------------------------------
//      INICIAR_PLANIFICACION:
// ----------------------------------------------

// funcion que activa los semaforos de planificacion
// pre: -
// post: activa los semaforos de planificacion
void f_iniciar_planificacion();
// funcion que desactiva los semaforos de planificacion
// pre: -
// post: desactiva los semaforos de planificacion
void f_detener_planificacion();

// ----------------------------------------------
//      CAMBIAR_GRADO_MULTIPROGRAMACION:
// ----------------------------------------------
void f_cambiar_grado_multiprogramacion(uint16_t nuevo_grado);
void* cambiar_grado_multiprogramacion(void* args);


#endif //CONSOLA_H_