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

/*
 *  Messages: enum & structs
 */

// Message headers
typedef enum
{

    // KERNEL <-> CPU
    MSG_KERNEL_CPU_DISPATCH,
    MSG_CPU_DISPATCH_KERNEL,
    MSG_IO_KERNEL,
    MSG_KERNEL_IO,
    // KERNEL <-> MEMORIA
    MSG_KERNEL_MEMORIA,
    MSG_MEMORIA_KERNEL,
    CREAR_PROCESO_KERNEL,
    // KERNEL <-> IO
    IO_GEN_SLEEP,
    //CPU <-> MEMORIA
    MSG_MEMORIA_CPU,
    MSG_CPU_MEMORIA,
    //IO <-> MEMORIA
    MSG_IO_MEMORIA,
    EXAMPLE,

    //Empty package
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
t_package *package_create(t_msg_header msg_header);
void package_destroy(t_package *package);
int package_send(t_package *package, int fd);
int package_recv(t_package *package, int fd);

// serialize
void *serializar_paquete(t_package *paquete, int bytes);
void example_serialize_msg(t_buffer *buffer, t_message_example *msg);
void serialize_pcb(t_buffer *buffer, t_PCB *pcb);
void serialize_cpu_registers(t_buffer *buffer, t_cpu_registers *cpu_registers);

// deserialize
void example_deserialize_msg(t_buffer *buffer, t_message_example *msg);

void deserialize_pcb(t_buffer *buffer, t_PCB *pcb);
void deserialize_cpu_registers(t_buffer *buffer, t_cpu_registers *cpu_registers);

void serialize_nuevo_proceso(t_buffer *buffer, t_nuevo_proceso *nuevo_proceso);
void deserialize_nuevo_proceso(t_buffer *buffer, t_nuevo_proceso *nuevo_proceso);
//send

#endif