#ifndef PROTOCOLO_H_
#define PROTOCOLO_H_

// Standard Library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>

// Libraries
#include <inttypes.h>

// Commons
#include <sys/socket.h>
#include <commons/collections/list.h>

#include "estructuras.h"
#include "buffer.h"
#include "instruccion.h"
#include "next_instruction.h"

/*
 *  Messages: enum & structs
 */

// Message headers
typedef enum
{

    // KERNEL <-> CPU
    MSG_KERNEL_CPU_DISPATCH,
    MSG_PCB_KERNEL,
    MSG_PCB_CPU,
    MSG_CPU_DISPATCH_KERNEL,
    MSG_IO_KERNEL,
    MSG_KERNEL_IO,
    MSG_PCB_IO_KERNEL,
    MSG_QUANTUM,                         // KERNEL -> CPU (interrupcion por fin de quantum)
    MSG_PCB_KERNEL_INTERRUPTION_QUANTUM, // CPU -> KERNEL (devuelvo pcb de proceso a kernel con interrupcion por fin de quantum)
                                         // KERNEL <-> MEMORIA
    MSG_KERNEL_MEMORIA,
    MSG_MEMORIA_KERNEL,
    MSG_KERNEL_CREATE_PROCESS,
    // KERNEL <-> IO
    MSG_IO_GEN_SLEEP,

    // MEMORIA <-> CPU
    MSG_MEMORIA_CPU_DATA_READ,
    MSG_MEMORIA_CPU_DATA_WRITE,
    MSG_MEMORIA_CPU_FRAME,
    

    // CPU <-> MEMORIA
    MSG_CPU_MEMORIA_DATA_WRITE,
    MSG_CPU_MEMORIA_DATA_READ,
    MSG_MEMORIA_CPU,
    MSG_CPU_MEMORIA,
    MSG_CPU_MEMORIA_INIT,
    MSG_INSTRUCTION_MEMORIA,
    MSG_NEXT_INSTRUCTION_CPU,
    MSG_MEMORIA_CPU_INIT,
    MSG_CPU_MEMORIA_PAGE,
    // IO <-> MEMORIA
    MSG_IO_MEMORIA,
    EXAMPLE,

    // Empty package
    NULL_HEADER
} t_msg_header;

// Message package
typedef struct
{
    t_msg_header msg_header; // Message header
    t_buffer *buffer;        // Buffer
} t_package;

typedef struct
{
    char *cadena;
    uint8_t entero;
} t_message_example;

/*
 *  package functions
 */

// Package
t_package *package_create(t_msg_header, u_int32_t);
void package_destroy(t_package *);
int package_send(t_package *, int);
int package_recv(t_package *, int);
t_buffer *get_buffer(t_package *);
t_msg_header get_message_header(t_package *);

// Crea un nuevo t_message_example con los datos proporcionados.
// Pre: El parámetro cadena debe apuntar a una cadena de caracteres válida y no debe ser NULL.
//      El parámetro entero debe ser un valor entero de 8 bits.
// Post: La función retorna un puntero a una estructura t_message_example creada con los datos proporcionados.
//       Si la creación falla, la función retorna NULL.
t_message_example *message_example_create(char *, uint8_t);

// Obtiene la cadena almacenada en t_message_example.
// Pre: El parámetro example debe ser un puntero válido a una estructura t_message_example.
// Post: La función retorna un puntero a la cadena de caracteres almacenada en la estructura.
char *get_cadena(t_message_example *);

// Obtiene el valor entero almacenado en t_message_example.
// Pre: El parámetro example debe ser un puntero válido a una estructura t_message_example.
// Post: La función retorna el valor entero de 8 bits almacenado en la estructura.
uint8_t get_entero(t_message_example *);

// Calcula el tamaño en bytes de la estructura t_message_example.
// Pre: El parámetro example debe ser un puntero válido a una estructura t_message_example.
// Post: La función retorna el tamaño en bytes de la estructura.
uint32_t get_message_example_size(t_message_example *);

// Libera la memoria utilizada por un t_message_example.
// Pre: El parámetro example debe ser un puntero válido a una estructura t_message_example.
// Post: La memoria asociada a la estructura y su cadena interna se libera.
void message_example_destroy(t_message_example *);

/*********** SEND AND RECIVE FUNCTIONS ***********/
// Envía un t_PCB a través de un socket especificado.
// Pre: El parámetro msg_header debe contener un encabezado de mensaje válido.
//      El parámetro fd debe ser un descriptor de archivo de socket válido y abierto.
//      El puntero pcb debe apuntar a un bloque de control de proceso válido y no debe ser NULL.
// Post: El t_PCB se serializa y se envía a través del socket especificado.
//       La función retorna 0 si el envío se realizó correctamente.
int send_pcb(t_msg_header, int, t_PCB *);

// Envía un mensaje t_message_example a través de un socket especificado.
// Pre: El parámetro cadena debe apuntar a una cadena de caracteres válida y no debe ser NULL.
//      El parámetro entero debe ser un valor entero de 8 bits.
//      El parámetro fd debe ser un descriptor de archivo de socket válido y abierto.
// Post: El t_message_example se crea, serializa y se envía a través del socket especificado.
//       La función retorna 0 si el envío se realizó correctamente, y -1 en caso de error.
int send_example(char *, uint8_t, int);

// Recibe un mensaje de ejemplo a través de un socket especificado.
// Pre: El parámetro fd debe ser un descriptor de archivo de socket válido y abierto.
// Post: La función retorna un puntero a una estructura t_message_example creada dinámicamente con los datos recibidos y deserializados.
t_message_example *recv_example(int);

/*********** SERIALIZE AND DESERIALIZE FUNCTIONS ***********/

void *serializar_paquete(t_package *, int);

/*********** SERIALIZE AND DESERIALIZE 'T_MESSAGE_EXAMPLE' ***********/
// Serializa un mensaje de ejemplo.
// Pre: El buffer y el mensaje deben ser válidos y no NULL.
// Post: El mensaje se serializa en el buffer.
void example_serialize_msg(t_buffer *, t_message_example *);

// Deserializa un mensaje de ejemplo desde un buffer especificado.
// Pre: El parámetro buffer debe ser un puntero válido a un t_buffer inicializado.
// Post: La función retorna un puntero a una estructura t_message_example creada dinámicamente con los datos deserializados.
t_message_example *example_deserialize_msg(t_buffer *buffer);

/*********** SERIALIZE AND DESERIALIZE 'T_PCB' ***********/
// Serializa un t_PCB.
// Pre: El buffer y el t_PCB deben ser válidos y no NULL.
// Post: El t_PCB se serializa en el buffer.
void serialize_pcb(t_buffer *, t_PCB *);

// Deserializa un t_PCB.
// Pre: El buffer debe ser válido y no NULL.
// Post: El t_PCB se deserializa desde el buffer y se devuelve.
t_PCB *deserialize_pcb(t_buffer *);

/*********** SERIALIZE AND DESERIALIZE 'T_CPU_REGISTERS' ***********/
// Serializa los registros de la CPU.
// Pre: El buffer y los registros de la CPU deben ser válidos y no NULL.
// Post: Los registros de la CPU se serializan en el buffer.
void serialize_cpu_registers(t_buffer *, t_cpu_registers *);

// Deserializa los registros de la CPU.
// Pre: El buffer y los registros de la CPU deben ser válidos y no NULL.
// Post: Los registros de la CPU se deserializan desde el buffer y se almacenan en la estructura t_cpu_registers.
void deserialize_cpu_registers(t_buffer *, t_cpu_registers *);

/*********** SERIALIZE AND DESERIALIZE 'T_NEW_PROCESS' ***********/
// Serializa la información de un nuevo proceso.
// Pre: El buffer y la estructura t_new_process deben ser válidos y no NULL.
// Post: La información del t_new_process se serializa en el buffer.
void serialize_nuevo_proceso(t_buffer *, t_new_process *);

// Deserializa la información de un nuevo proceso.
// Pre: El buffer debe ser válido y no NULL.
// Post: La información del nuevo proceso se deserializa desde el buffer y se devuelve.
t_new_process *deserialize_nuevo_proceso(t_buffer *);

/*********** SERIALIZE AND DESERIALIZE 'T_INSTRUCTION' ***********/
// Serializa una instrucción en el buffer.
// pre: el buffer y la instrucción deben ser válidos y no NULL.
// post: la instrucción es serializada en el buffer.
void serialize_instruction(t_buffer *, t_instruction *);

// Deserializa una instrucción a partir de un buffer.
// Pre: El buffer debe ser válido y no NULL, debe contener datos.
// Post: Retorna un puntero a una estructura t_instruction con el nombre y los parámetros deserializados.
t_instruction *deserialize_instruction(t_buffer *);

/*********** SERIALIZE AND DESERIALIZE 'T_NEXT_INSTRUCTION' ***********/
// Serializa una estructura t_next_instruction en un buffer.
// Pre: Los punteros a t_buffer y t_next_instruction deben ser válidos y no NULL.
// Post: Los datos de la estructura t_next_instruction se añade al buffer.
void serialize_next_instruction(t_buffer *, t_next_instruction *);

// Deserializa una estructura t_next_instruction a partir de un buffer.
// Pre: El puntero a t_buffer debe ser válido y no NULL.
// Post: Retorna un puntero a una estructura t_next_instruction creada a partir de los datos del buffer.
//       Si ocurre un error en la deserialización, retorna NULL.
t_next_instruction *deserialize_next_instruction(t_buffer *);

void serialize_uint32_t(t_buffer *buffer, int args_qty, ...);

void deserialize_uint32_t(t_buffer *buffer, int args_qty, ...);

#endif