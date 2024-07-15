#ifndef PROTOCOLO_ENTRADASALIDA_H_
#define PROTOCOLO_ENTRADASALIDA_H_

#include "variables_globales.h"
#include "utils/protocolo.h"
#include "utils/estructuras.h"
#include "comportamientos_ios.h"
#include "utils/solicitudes_io.h" 
#include "dialfs.h"

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
 * @param dialfs Puntero a la estructura t_dialfs que representa el sistema de archivos.
 */
void atender_solicitud_dialfs(int fd, t_dialfs* dialfs);

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

#endif // PROTOCOLO_ENTRADASALIDA_H_