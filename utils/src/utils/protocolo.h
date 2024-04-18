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

#include "buffer.h"


/*
 *  Messages: enum & structs
 */

// Message headers
typedef enum {

    // KERNEL <-> CPU
    MSG_KERNEL_CPU_DISPATCH,
	MSG_CPU_KERNEL_DISPATCH,

    //
    NULL_HEADER
} t_msg_header;





// Message package
typedef struct {
    t_msg_header        msg_header;     // Message header 
    t_buffer*           buffer;         // Buffer
} t_package;



typedef struct {
    t_msg_header        msg_header;     // Message header 
    t_buffer*           buffer;         // Buffer
} t_message_string;
/*
 *  package functions
 */



// Package
t_package* package_create(t_msg_header msg_header);
void package_destroy(t_package* package);
int package_send(t_package* package, int fd);
int package_recv(t_package* package, int fd);


//serialize
void* serializar_paquete(t_package* paquete, int bytes);

#endif