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
t_package* package_create(t_msg_header msg_header) {

    t_package* package = malloc(sizeof(t_package));

    package->msg_header = msg_header;
    package->buffer = buffer_create();

    return package;

}

// Package destroy
void package_destroy(t_package* package) {
    if(package->buffer != 0){
        buffer_destroy(package->buffer);
    }
    
    free(package);
}

// Package send
int package_send(t_package* package, int fd) {

    size_t size     =   sizeof(t_msg_header)    // Message header
                    +   sizeof(uint32_t)        // Buffer size
                    +   package->buffer->size;  // Buffer
    
    void* stream = malloc(size);
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
void _recv_and_check_response(int fd, void* buf,size_t size,int flags){
    ssize_t recv_response = recv(fd, buf, size, flags);
    if(recv_response == 0){
        printf("SE HA DESCONECTADO EL SOCKET. ABORTANDO PROCESO\n");
        exit(1); //TODO para resolver memoryleaks se debería hacer un graceful-exit
    }
}

// Package receive
// El package debe ser creado con package_create
int package_recv(t_package* package, int fd) {

    _recv_and_check_response(fd, &(package->msg_header), sizeof(t_msg_header), 0);
    
    uint32_t size;
    _recv_and_check_response(fd, &size, sizeof(uint32_t), 0);

    if(size == 0){
        return EXIT_SUCCESS;
    }

    void* stream = malloc(size);
    _recv_and_check_response(fd, stream, size, 0);

    package->buffer->size = size;
    package->buffer->stream = stream;

    return EXIT_SUCCESS;

}

//serializado generico TP0
void* serializar_paquete(t_package* paquete, int bytes)
{
	void * magic = malloc(bytes);
	int desplazamiento = 0;

	memcpy(magic + desplazamiento, &(paquete->msg_header), sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(magic + desplazamiento, &(paquete->buffer->size), sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(magic + desplazamiento, paquete->buffer->stream, paquete->buffer->size);
	desplazamiento+= paquete->buffer->size;

	return magic;
}