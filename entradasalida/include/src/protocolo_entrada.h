#ifndef PROTOCOLO_ENTRADASALIDA_H_
#define PROTOCOLO_ENTRADASALIDA_H_

#include "variables_globales.h"
#include "utils/protocolo.h"

// Defino la estructura de instrucción (sirve para IO_GEN_SLEEP, 
// pero podría ser necesario modificarla en otras interfaces I/O)
typedef struct {
    int tipo_instruccion;
    int unidades_trabajo;
} t_instruccion;

// Prototipos de funciones
void requests_kernel();
// void requests_memoria();
// int send_example_kernel();
// int recv_example_msg_kernel();
// int send_example_memoria();
void atender_io_kernel();
// void atender_io_memoria();
t_instruccion* recibir_instruccion(int socket_kernel);
void enviar_confirmacion(int socket_kernel, int resultado);

#endif