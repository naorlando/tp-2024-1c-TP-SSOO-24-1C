#ifndef CONSOLA_H_
#define CONSOLA_H_

#include <variables_globales.h>
#include "utils/estructuras.h"
#include "utils/protocolo.h"
#include "utils/utils.h"
#include "table_pcb.h"
#include "protocolo_kernel.h"
#include "largo_plazo.h"



void iniciar_consola_interactiva();
bool _validacion_de_instrucciones_consola(char*);
void _atender_instruccion(void*);

// FORMAS DE ATENDER PROCESO:
void f_iniciar_proceso(char*);
void f_ejecutar_script(const char*);

void enviar_pcb_cpu();

int asignar_pid();

// ----------------------------------------------
//      PROCESO_ESTADO:
// ----------------------------------------------
// funcion para atender el comando de la consola: PROCESO_ESTADO
void f_mostrar_estado_procesos();
// funcion para obtener los PID de las t_queue de cada estado posible
void listar_pids_de_queue(t_queue *queue);


// ----------------------------------------------
//      FINALIZAR_PROCESO:
// ----------------------------------------------
void f_finalizar_proceso(u_int32_t pid);


#endif //CONSOLA_H_