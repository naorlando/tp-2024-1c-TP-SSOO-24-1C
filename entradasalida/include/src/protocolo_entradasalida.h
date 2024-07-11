#ifndef PROTOCOLO_ENTRADA_H_
#define PROTOCOLO_ENTRADA_H_

#include "utils/protocolo.h"
#include "variables_globales.h"
#include "utils/estructuras.h"

void atender_solicitud_generica(int fd);
void atender_solicitud_stdin(int fd);
void atender_solicitud_stdout(int fd);

void escribir_memoria(uint32_t direccion_fisica, char* valor, uint32_t tamanio);
char* leer_memoria(uint32_t direccion_fisica, uint32_t tamanio);

void enviar_confirmacion_io(int fd);

void send_IO_interface_kernel();

#endif