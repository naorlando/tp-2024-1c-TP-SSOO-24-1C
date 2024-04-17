#ifndef SOCKETS_H_
#define SOCKETS_H_

#include <stdlib.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netdb.h>
#include<commons/log.h>
#include<commons/collections/list.h>
#include<string.h>
#include<assert.h>

//TODO: ¿¿¿¿¿¿¿¿¿¿¿¿¿ crear ENUMS con &cod_op del recv() del recibir_operacion() ?????????
typedef enum
{
	MENSAJE,
	PAQUETE
    //HANDSHAKE,
    //RTA_HANDSHAKE,
//KERNEK - MEMORIA-------------------- nos aconseja poner de donde a donde se dirijen:
    //CREAR_PROCESO_KM,
    //RTA_CREAR_PROCESO_MK

//KERNEK - CPU: ---------------------    

//CPU - MEMORIA: ---------------------    

//...
}op_code;

int iniciar_servidor(t_log* logger, const char* name, char* ip, char* puerto);
int esperar_cliente(t_log* logger, const char* name, int socket_servidor);
int recibir_operacion(int socket_cliente);
int crear_conexion(t_log* logger, const char* server_name, char* ip, char* puerto);
void liberar_conexion(int socket_cliente);



#endif