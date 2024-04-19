#include "protocolo.h"

/*
 *      Buffer & package functions
 *
 *      Buffer create       t_buffer* buffer_create()
 *      Buffer destroy      void buffer_destroy(t_buffer* buffer)
 *      Buffer add          void buffer_add(t_buffer* buffer, void* stream, uint32_t size)
 *
 *      Package create      t_package* package_create(t_msg_header msg_header)
 *      Package destroy     void package_destroy(t_package* package)
 *      Package send        int package_send(t_package* package, int fd)
 *      Package receive     int package_recv(t_package* package, int fd)
 */

/*
 *      Package
 */

// Package create
t_package *package_create(t_msg_header msg_header)
{

    t_package *package = malloc(sizeof(t_package));

    package->msg_header = msg_header;
    package->buffer = buffer_create();

    return package;
}

// Package destroy
void package_destroy(t_package *package)
{
    if (package->buffer != 0)
    {
        buffer_destroy(package->buffer);
    }

    free(package);
}

// Package send
int package_send(t_package *package, int fd)
{

    printf("SIZE BUFFER %d", package->buffer->size);

    size_t size = sizeof(t_msg_header)     // Message header
                  + sizeof(uint32_t)       // Buffer size
                  + package->buffer->size; // Buffer

    void *stream = malloc(size);
    int offset = 0;

    memcpy(stream + offset, &(package->msg_header), sizeof(t_msg_header));
    offset += sizeof(t_msg_header);
    memcpy(stream + offset, &(package->buffer->size), sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(stream + offset, package->buffer->stream, package->buffer->size);

    send(fd, stream, size, 0);

    free(stream);

    return EXIT_SUCCESS;
}

// Abstraction of sockets recv method. It should check if the connection has ended
void _recv_and_check_response(int fd, void *buf, size_t size, int flags)
{
    ssize_t recv_response = recv(fd, buf, size, flags);
    if (recv_response == 0)
    {
        printf("SE HA DESCONECTADO EL SOCKET. ABORTANDO PROCESO\n");
        exit(1); // TODO para resolver memoryleaks se debería hacer un graceful-exit
    }
}

// Package receive
// El package debe ser creado con package_create
int package_recv(t_package *package, int fd)
{

    _recv_and_check_response(fd, &(package->msg_header), sizeof(t_msg_header), 0);

    uint32_t size;
    _recv_and_check_response(fd, &size, sizeof(uint32_t), 0);

    if (size == 0)
    {
        return EXIT_SUCCESS;
    }

    void *stream = malloc(size);
    _recv_and_check_response(fd, stream, size, 0);

    package->buffer->size = size;
    package->buffer->stream = stream;

    return EXIT_SUCCESS;
}

// serializado generico TP0
void *serializar_paquete(t_package *paquete, int bytes)
{
    void *stream = malloc(bytes);
    int desplazamiento = 0;

    memcpy(stream + desplazamiento, &(paquete->msg_header), sizeof(int));
    desplazamiento += sizeof(int);
    memcpy(stream + desplazamiento, &(paquete->buffer->size), sizeof(int));
    desplazamiento += sizeof(int);
    memcpy(stream + desplazamiento, paquete->buffer->stream, paquete->buffer->size);
    desplazamiento += paquete->buffer->size;

    return stream;
}

// size_t * size, char *cadena, uint8_t entero
void example_serialize_msg(t_buffer *buffer, t_message_example *msg)
{
    // Check for invalid input (empty cadena or zero buffer size)

    // Calculate total serialized message size
    uint8_t size_cadena = strlen(msg->cadena) + 1; // Include null terminator
    buffer->size = sizeof(uint8_t) * 2 + size_cadena ;
    buffer->stream = malloc(buffer->size);
    void *stream = malloc(buffer->size);
    uint32_t offset = 0;
    // Serialize the message content into the buffer
    // void *stream_ptr = malloc(total_size);
    memcpy(stream, &size_cadena, sizeof(uint8_t));
    offset += sizeof(uint8_t);
    memcpy(stream + offset, msg->cadena, size_cadena);
    offset += sizeof(uint8_t);
    memcpy(stream + offset, &(msg->entero), sizeof(msg->entero));
    offset += sizeof(uint8_t);

    // Update buffer size with the actual serialized data size

    buffer->stream = stream;
}

void example_deserialize_msg(t_buffer *buffer, t_message_example *msg)
{
    // void *stream = buffer->stream;
   
    // uint32_t size_cadena = 0;
    // memcpy(&size_cadena, stream, sizeof(uint32_t)); //COPIA VALOR LARGO DE LA CADENA TAM
    // stream += sizeof(size_cadena);

    // msg->cadena = malloc(size_cadena); 

    // memcpy(msg->cadena, stream + sizeof(uint32_t), size_cadena); // Se copia la cadena desde el buffer al área de memoria asignada //ERROR

    // memcpy(&(msg->entero), stream + sizeof(uint32_t) + size_cadena, sizeof(uint32_t));


    uint8_t offset = 0;

    // Copiar el tamaño de la cadena desde el stream al buffer
    uint8_t size_cadena = 0;
    memcpy(&size_cadena, buffer->stream, sizeof(uint8_t));
    offset += sizeof(uint8_t);

    // Reservar memoria para la cadena y copiarla desde el stream al buffer
    msg->cadena = malloc(size_cadena);
    memcpy(msg->cadena, buffer->stream + offset, size_cadena);
    offset += size_cadena;

    // Copiar el entero desde el stream al buffer
    memcpy(&(msg->entero), buffer->stream + offset, sizeof(uint8_t));
}
