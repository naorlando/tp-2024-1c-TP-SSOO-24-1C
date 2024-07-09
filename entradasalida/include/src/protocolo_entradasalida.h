#ifndef PROTOCOLO_ENTRADA_H_
#define PROTOCOLO_ENTRADA_H_

#include <readline/readline.h>
#include "variables_globales.h"
#include "utils/protocolo.h"
#include "utils/estructuras.h"
#include "utils/solicitudes_io.h"
#include "dialfs.h"

// FUNCIONES de atención de instrucciones
void atender_instruccion_generica(int fd);
void atender_instruccion_stdin(int fd);
void atender_instruccion_stdout(int fd);
void atender_instruccion_dialfs(int fd);

// FUNCION auxiliar de interfaz STDIN
void escribir_memoria(uint32_t direccion_fisica, char* valor, uint32_t tamanio);

// FUNCION auxiliar de interfaz STDOUT
char* leer_memoria(uint32_t direccion_fisica, uint32_t tamanio);

// FUNCION auxiliar de interfaz DIALFS
void enviar_datos_leidos(int fd, void* buffer, uint32_t tamanio);

// FUNCIONES comunes
void enviar_confirmacion_io(int fd);
void send_IO_interface_kernel();

#endif