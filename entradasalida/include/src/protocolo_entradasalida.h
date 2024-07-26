#ifndef PROTOCOLO_ENTRADASALIDA_H_
#define PROTOCOLO_ENTRADASALIDA_H_

#include <readline/readline.h>
#include <readline/history.h>
#include "variables_globales.h"
#include "utils/protocolo.h"
#include "utils/estructuras.h"
#include "comportamientos_ios.h"
#include "utils/solicitudes_io.h" 
#include "dialfs.h"
#include "comportamientos_dialfs.h"


//======================================================
//        FUNCIONES DE ENTRADA/SALIDA GENERICA
//======================================================

/**
 * Atiende una solicitud genérica de entrada/salida.
 * Esta función espera un tiempo determinado según la solicitud.
 *
 * @param fd Descriptor de archivo para la comunicación.
 */
void atender_solicitud_generica(int fd);

//======================================================
//        FUNCIONES DE ENTRADA/SALIDA STDIN
//======================================================

/**
 * Atiende una solicitud de entrada/salida STDIN.
 * Lee un texto desde la consola y lo guarda en memoria.
 *
 * @param fd Descriptor de archivo para la comunicación.
 */
void atender_solicitud_stdin(int fd);

//======================================================
//        FUNCIONES DE ENTRADA/SALIDA STDOUT
//======================================================

/**
 * Atiende una solicitud de entrada/salida STDOUT.
 * Lee un texto desde memoria y lo muestra por consola.
 *
 * @param fd Descriptor de archivo para la comunicación.
 */
void atender_solicitud_stdout(int fd);

//======================================================
//        FUNCIONES DE ENTRADA/SALIDA DIALFS
//======================================================

/**
 * Atiende una solicitud de entrada/salida DialFS.
 * Maneja operaciones de creación, eliminación, truncado, lectura y escritura de archivos.
 *
 * @param fd Descriptor de archivo para la comunicación.
 */
void atender_solicitud_dialfs(int fd);

//======================================================
//               FUNCIONES COMUNES
//======================================================

/**
 * Envía una confirmación de la operación de entrada/salida.
 *
 * @param fd Descriptor de archivo para la comunicación.
 * @param header Encabezado del mensaje de confirmación.
 * @param response Respuesta a enviar.
 */
void send_confirmacion_io(int fd, t_msg_header header, t_response* response);

/**
 * Envía la información de la interfaz de E/S al kernel.
 */
void send_IO_interface_kernel();


//======================================================
//               FUNCIONES AUXILIARES
//======================================================

/**
 * Obtiene el nombre de una operación de entrada/salida.
 * @param operacion Operación de entrada/salida.
 * @return Nombre de la operación.
 * @note Esta función es utilizada para mostrar información en los logs.
 */
const char* get_operation_name(t_name_instruction operacion);

/**
 * Envia los datos leidos al kernel
 * @param fd Descriptor de archivo para la comunicación.
 * @param buffer Buffer con los datos leidos.
 * @param tamanio Tamaño de los datos leidos.
 */
void enviar_datos_leidos(int fd, void* buffer, uint32_t tamanio);

#endif // PROTOCOLO_ENTRADASALIDA_H_