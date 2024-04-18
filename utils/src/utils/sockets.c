#include "sockets.h"

int iniciar_servidor(t_log *logger, const char *name, char *ip, char *puerto)
{

    int socket_servidor;

    struct addrinfo hints, *servinfo, *p;

    // Inicializando hints
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    // Recibe los addrinfo
    getaddrinfo(ip, puerto, &hints, &servinfo);

    bool conecto = false; // se agrega boleano

    // Aca comienza la logica utilizar for hasta que se conecten
    for (struct addrinfo *p = servinfo; p != NULL; p = p->ai_next)
    {
        socket_servidor = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (socket_servidor == -1)
        {
            perror("Hubo un error al crear el socket.");
            // exit(EXIT_FAILURE); Queremos que continue el programa
            continue;
        } // fallo de crear socket

        /*  Evita que el socket quede en un limbo y se pueda reutilizar en forma inmediata,
            sin necesidad de esperar a que el S.O. lo libere. */
        int active = 1;
        setsockopt(socket_servidor, SOL_SOCKET, SO_REUSEADDR, &active, sizeof(active));

        if (bind(socket_servidor, p->ai_addr, p->ai_addrlen) == -1)
        {
            // Si entra aca fallo el bind
            close(socket_servidor);
            continue;
        }
        // Ni bien conecta uno nos vamos del for
        conecto = true;
        break;
    }
    // #########################################
    //  CREACIÓN DE SOCKET TP0
    //  // Creamos el socket de escucha del servidor
    //  socket_servidor = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);

    // if (socket_servidor == -1)
    // {
    //     perror("Hubo un error al crear el socket.");
    //     exit(EXIT_FAILURE);
    // }

    // Asociamos el socket a un puerto
    // bind(socket_servidor, servinfo->ai_addr, servinfo->ai_addrlen);
    // ###############################################

    // Escuchamos las conexiones entrantes

    listen(socket_servidor, SOMAXCONN);

    freeaddrinfo(servinfo);
    log_trace(logger, "Listo para escuchar a mi cliente");

    return socket_servidor;
}

int esperar_cliente(t_log *logger, const char *name, int socket_servidor)
{

    // #include<sys/socket.h>
    struct sockaddr_in dir_cliente;
    socklen_t tam_direccion = sizeof(struct sockaddr_in);

    // Aceptamos un nuevo cliente
    int socket_cliente = accept(socket_servidor, (void *)&dir_cliente, &tam_direccion);

    log_info(logger, "Cliente (%s:%d) conectado a %s", inet_ntoa(dir_cliente.sin_addr), dir_cliente.sin_port, name);

    return socket_cliente;
}

int recibir_operacion(t_log *logger, int socket_cliente)
{

    int cod_op;
    if (recv(socket_cliente, &cod_op, sizeof(int), MSG_WAITALL) > 0)
    {

        return cod_op;
    }
    else
    {
        log_info(logger, "Hello from downtown 3");
        close(socket_cliente);
        return -1;
    }
}

// Se agrega del TP0 conexion tipo Cliente
int crear_conexion(t_log *logger, const char *server_name, char *ip, char *puerto)
{
    // struct addrinfo hints;
    // struct addrinfo *server_info;

    struct addrinfo hints, *server_info;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    getaddrinfo(ip, puerto, &hints, &server_info);

    // Ahora vamos a crear el socket.
    int socket_cliente = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);

    if (socket_cliente == -1)
    {
        // perror("Hubo un error al crear el socket.");
        // exit(EXIT_FAILURE); No queremos que el programa cierre
        log_error(logger, "Error creando el socket para %s:%s", ip, puerto);
        return 0;
    }

    // Ahora que tenemos el socket, vamos a conectarlo
    // connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen); Se modifica validando la conexion
    if (connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen) == -1)
    {
        log_error(logger, "Error al conectar (a %s)\n", server_name);
        freeaddrinfo(server_info);
        return 0;
    }
    else
        log_info(logger, "Cliente conectado en %s:%s (a %s)\n", ip, puerto, server_name);

    freeaddrinfo(server_info); // free

    return socket_cliente;
}

// CERRAR CONEXION
void liberar_conexion(int socket_cliente)
{
    close(socket_cliente);
    socket_cliente = -1;
}

// Esto es parte del TP0 pero esta libreria solo se encarga de lo relacionado al socket
void *recibir_buffer(int *size, int socket_cliente)
{
    void *buffer;

    recv(socket_cliente, size, sizeof(int), MSG_WAITALL);
    buffer = malloc(*size);
    recv(socket_cliente, buffer, *size, MSG_WAITALL);

    return buffer;
}

void recibir_mensaje(t_log* logger, int socket_cliente)
{
    int size;
    char *buffer = recibir_buffer(&size, socket_cliente);
    log_info(logger, "Me llego el mensaje: %s", buffer);
    free(buffer);
}

// t_list *recibir_paquete(int socket_cliente)
// {
//     int size;
//     int desplazamiento = 0;
//     void *buffer;
//     t_list *valores = list_create();
//     int tamanio;

//     buffer = recibir_buffer(&size, socket_cliente);
//     while (desplazamiento < size)
//     {
//         memcpy(&tamanio, buffer + desplazamiento, sizeof(int));
//         desplazamiento += sizeof(int);
//         char *valor = malloc(tamanio);
//         memcpy(valor, buffer + desplazamiento, tamanio);
//         desplazamiento += tamanio;
//         list_add(valores, valor);
//     }
//     free(buffer);
//     return valores;
// }
