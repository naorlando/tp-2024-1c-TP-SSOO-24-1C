#ifndef PROTOCOLO_ENTRADA_H_
#define PROTOCOLO_ENTRADA_H_

#include <readline/readline.h>
#include "variables_globales.h"
#include "utils/protocolo.h"
#include "utils/estructuras.h"
#include "comportamientos_ios.h"
#include "utils/solicitudes_io.h" // ??
#include "dialfs.h"

// FUNCIONES de atención de instrucciones
void atender_solicitud_generica(int);
void atender_solicitud_stdin(int);
void atender_solicitud_stdout(int);
void atender_solicitud_dialfs(int fd);

void send_confirmacion_io(int, t_msg_header, t_response*);




// FUNCIONES comunes
void send_IO_interface_kernel();

#endif