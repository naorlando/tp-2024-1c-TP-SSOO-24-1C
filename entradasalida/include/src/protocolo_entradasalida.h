#ifndef PROTOCOLO_ENTRADA_H_
#define PROTOCOLO_ENTRADA_H_


#include "variables_globales.h"
#include "utils/protocolo.h"
#include "utils/estructuras.h"
#include "comportamientos_ios.h"
#include "utils/solicitudes_io.h" // ??
#include "dialfs.h"

void atender_solicitud_generica(int);
void atender_solicitud_stdin(int);
void atender_solicitud_stdout(int);


void send_confirmacion_io(int, t_msg_header, t_response*);

// FUNCIONES de atención de instrucciones
void atender_instruccion_dialfs(int fd);

// FUNCION auxiliar de interfaz DIALFS
void enviar_datos_leidos(int fd, void* buffer, uint32_t tamanio);

// FUNCIONES comunes
void send_IO_interface_kernel();

void send_IO_interface_memoria();

#endif