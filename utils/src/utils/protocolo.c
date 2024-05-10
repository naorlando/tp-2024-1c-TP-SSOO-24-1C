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
    package->buffer = buffer_create(sizeof(package->buffer));// cambio

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

void example_serialize_msg(t_buffer *buffer, t_message_example *msg)
{

    uint8_t size_cadena = strlen(msg->cadena) + 1; // Include null terminator
    buffer->size = sizeof(uint8_t) * 2 + size_cadena;
    buffer->stream = malloc(buffer->size);
    void *stream = malloc(buffer->size);
    uint8_t offset = 0;

    memcpy(stream, &size_cadena, sizeof(uint8_t));
    offset += sizeof(uint8_t);
    memcpy(stream + offset, msg->cadena, size_cadena);
    offset += size_cadena;
    memcpy(stream + offset, &(msg->entero), sizeof(uint8_t));
    offset += sizeof(uint8_t);

    buffer->stream = stream;
}

void example_deserialize_msg(t_buffer *buffer, t_message_example *msg)
{

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
// CPU registers
void serialize_cpu_registers(t_buffer *buffer, t_cpu_registers *cpu_registers)
{

    // CPU registers: AX, BX, CX, DX
    //buffer->size = sizeof(uint8_t) * 4 + sizeof(uint32_t) * 7; // cambio
    buffer->stream = malloc(buffer->size);

    memcpy(buffer->stream, &(cpu_registers->pc), sizeof(uint32_t));
    int offset = sizeof(uint32_t);
    memcpy(buffer->stream + offset, &(cpu_registers->ax), sizeof(uint8_t));
    offset += sizeof(uint8_t);
    memcpy(buffer->stream + offset, &(cpu_registers->bx), sizeof(uint8_t));
    offset += sizeof(uint8_t);
    memcpy(buffer->stream + offset, &(cpu_registers->cx), sizeof(uint8_t));
    offset += sizeof(uint8_t);
    memcpy(buffer->stream + offset, &(cpu_registers->dx), sizeof(uint8_t));
    offset += sizeof(uint8_t);
    memcpy(buffer->stream + offset, &(cpu_registers->eax), sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(buffer->stream + offset, &(cpu_registers->ebx), sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(buffer->stream + offset, &(cpu_registers->ecx), sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(buffer->stream + offset, &(cpu_registers->edx), sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(buffer->stream + offset, &(cpu_registers->si), sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(buffer->stream + offset, &(cpu_registers->di), sizeof(uint32_t));
    offset += sizeof(uint32_t);
}

// PCB
void serialize_pcb(t_buffer *buffer, t_PCB *pcb)
{
    size_t size = sizeof(t_PCB) + sizeof(uint32_t);
    void *stream = malloc(size);

    // PID
    memcpy(stream, &(pcb->pid), sizeof(uint32_t));
    int offset = sizeof(uint32_t);

    // PC
    memcpy(stream + offset, &(pcb->program_counter), sizeof(uint32_t));
    offset += sizeof(uint32_t);

    // Quantum
    memcpy(stream + offset, &(pcb->quantum), sizeof(uint32_t));
    offset += sizeof(uint32_t);

    // CPU registers
    t_buffer *buffer_cpu_registers = buffer_create(sizeof(uint8_t) * 4 + sizeof(uint32_t) * 7); // cambio

    serialize_cpu_registers(buffer_cpu_registers, pcb->cpu_registers);

    memcpy(stream + offset, &(buffer_cpu_registers->size), sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(stream + offset, buffer_cpu_registers->stream, sizeof(t_cpu_registers));
    offset += sizeof(t_cpu_registers);

    buffer->stream = stream;
    buffer->size = offset;

    buffer_destroy(buffer_cpu_registers);
}

void deserialize_pcb(t_buffer *buffer, t_PCB *pcb)
{

    t_buffer *buffer_cpu_registers = buffer_create();
    uint32_t offset = 0;

    void *stream = buffer->stream;

    // PID
    memcpy(&(pcb->pid), stream, sizeof(uint32_t));
    stream += sizeof(uint32_t);

    // PC
    memcpy(&(pcb->program_counter), stream, sizeof(uint32_t));

    stream += sizeof(uint32_t);
    // Quantum
    memcpy(&(pcb->quantum), stream, sizeof(uint32_t));
    stream += sizeof(uint32_t);

    // CPU registers

    memcpy(&(offset), stream, (sizeof(uint32_t)));
    stream += sizeof(uint32_t);

    void *stream_aux = malloc(offset);

    memcpy(stream_aux, stream, offset);
    stream += sizeof(offset);

    buffer_cpu_registers->stream = stream_aux;
    buffer_cpu_registers->size = offset;

    pcb->cpu_registers = malloc(sizeof(t_cpu_registers));
    deserialize_cpu_registers(buffer_cpu_registers, pcb->cpu_registers);
    buffer_destroy(buffer_cpu_registers);
}

void deserialize_cpu_registers(t_buffer *buffer, t_cpu_registers *cpu_registers)
{

    void *stream = buffer->stream;

    memcpy(&(cpu_registers->pc), stream, sizeof(uint32_t));
    stream += sizeof(uint32_t);
    memcpy(&(cpu_registers->ax), stream, sizeof(uint8_t));
    stream += sizeof(uint8_t);
    memcpy(&(cpu_registers->bx), stream, sizeof(uint8_t));
    stream += sizeof(uint8_t);
    memcpy(&(cpu_registers->cx), stream, sizeof(uint8_t));
    stream += sizeof(uint8_t);
    memcpy(&(cpu_registers->dx), stream, sizeof(uint8_t));
    stream += sizeof(uint8_t);
    memcpy(&(cpu_registers->eax), stream, sizeof(uint32_t));
    stream += sizeof(uint32_t);
    memcpy(&(cpu_registers->ebx), stream, sizeof(uint32_t));
    stream += sizeof(uint32_t);
    memcpy(&(cpu_registers->ecx), stream, sizeof(uint32_t));
    stream += sizeof(uint32_t);
    memcpy(&(cpu_registers->edx), stream, sizeof(uint32_t));
    stream += sizeof(uint32_t);
    memcpy(&(cpu_registers->si), stream, sizeof(uint32_t));
    stream += sizeof(uint32_t);
    memcpy(&(cpu_registers->di), stream, sizeof(uint32_t));
}