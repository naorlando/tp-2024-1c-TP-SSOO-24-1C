#ifndef PROTOCOLO_ENTRADA_H_
#define PROTOCOLO_ENTRADA_H_

#include "utils/protocolo.h"
#include "variables_globales.h"
#include "utils/estructuras.h"
#include "comportamientos_ios.h"

void atender_solicitud_generica(int);
void atender_solicitud_stdin(int);
void atender_solicitud_stdout(int);

void escribir_memoria(uint32_t, char*, uint32_t);
char* leer_memoria(uint32_t, uint32_t);

void send_confirmacion_io(int, t_msg_header, t_response*);

void send_IO_interface_kernel();

#endif