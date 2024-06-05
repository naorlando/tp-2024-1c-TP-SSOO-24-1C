#include "protocolo.h"

/*
 *      Package create      t_package* package_create(t_msg_header msg_header)
 *      Package destroy     void package_destroy(t_package* package)
 *      Package send        int package_send(t_package* package, int fd)
 *      Package receive     int package_recv(t_package* package, int fd)
 */

/*
 *      Package
 */

// Package create
t_package *package_create(t_msg_header msg_header, u_int32_t buffer_size)
{

    t_package *package = malloc(sizeof(t_package));

    package->msg_header = msg_header;
    package->buffer = buffer_create(buffer_size);

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

t_buffer* get_buffer(t_package* package) {
    return package->buffer;
}

t_msg_header get_message_header(t_package* package) {
    return package->msg_header;
}

t_message_example* message_example_create(char* cadena, uint8_t entero)
{
    t_message_example* example = malloc(sizeof(t_message_example));

    if(example == NULL) return NULL;

    // Guardo la cadena
    example->cadena = malloc(strlen(cadena));
    strcpy(example->cadena, cadena);
    
    // Guardo el entero
    example->entero = entero;

    return example;
}

char* get_cadena(t_message_example* example) 
{
    return example->cadena;
}

uint8_t get_entero(t_message_example* example)
{
    return example->entero;
}

uint32_t get_message_example_size(t_message_example* example) 
{
    return sizeof(uint32_t) + (strlen(example->cadena) + 1) + sizeof(example->entero);
}

void message_example_destroy(t_message_example* example)
{
    if (example->cadena != NULL)
    {
        free(example->cadena);
    }

    free(example);
}

int send_pcb(t_msg_header msg_header, int fd, t_PCB* pcb) 
{
    uint32_t buffer_size = get_pcb_size(pcb) + sizeof(uint32_t);
    t_package* package = package_create(msg_header, buffer_size);

    serialize_pcb(get_buffer(package), pcb);
    package_send(package, fd);

    package_destroy(package);

    return 0;
}

int send_example(char* cadena, uint8_t entero, int fd) 
{
    t_message_example* example = message_example_create(cadena, entero);

    if(example == NULL) return -1;

    //Tamaño para el buffer
    uint32_t buffer_size = get_message_example_size(example);

    //Creo el paquete
    t_package* package_example = package_create(EXAMPLE, buffer_size);

    // Serializo el t_message_example en el buffer
    example_serialize_msg(get_buffer(package_example), example);

    // Envio el paquete a fd
    package_send(package_example, fd);

    message_example_destroy(example);
    package_destroy(package_example);
    return 0;
}

t_message_example* recv_example(int fd) 
{
    t_buffer *new_buffer = recive_full_buffer(fd);
    return example_deserialize_msg(new_buffer);
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
    // Agrego la cadena
    buffer_add_string(buffer, get_cadena(msg));

    // Agrego el numero entero
    buffer_add_uint8(buffer, get_entero(msg));
}

t_message_example* example_deserialize_msg(t_buffer* buffer)
{
    // Copiar el tamaño de la cadena
    uint32_t size_cadena = buffer_read_uint32(buffer);

    // Obtengo la cadena
    char* cadena = buffer_read_string(buffer, size_cadena);

    // Obtengo el numero entero
    uint8_t entero = buffer_read_uint8(buffer);

    //Creo la estrucutura t_message_example
    return message_example_create(cadena, entero);
}

void serialize_cpu_registers(t_buffer *buffer, t_cpu_registers *cpu_registers)
{
    //Agrego pc
    buffer_add_uint32(buffer, cpu_registers->pc);
    //Agrego ax
    buffer_add_uint8(buffer, cpu_registers->ax);
    //Agrego bx
    buffer_add_uint8(buffer, cpu_registers->bx);
    //Agrego cx
    buffer_add_uint8(buffer, cpu_registers->cx);
    //Agrego dx
    buffer_add_uint8(buffer, cpu_registers->dx);
    //Agrego eax
    buffer_add_uint32(buffer, cpu_registers->eax);
    //Agrego ebx
    buffer_add_uint32(buffer, cpu_registers->ebx);
    //Agrego ecx
    buffer_add_uint32(buffer, cpu_registers->ecx);
    //Agrego edx
    buffer_add_uint32(buffer, cpu_registers->edx);
    //Agrego si
    buffer_add_uint32(buffer, cpu_registers->si);
    //Agrego di
    buffer_add_uint32(buffer, cpu_registers->di);
}

// PCB
void serialize_pcb(t_buffer *buffer, t_PCB *pcb)
{
    //Agrego el pid
    buffer_add_uint32(buffer, pcb->pid);

    //Agrego quantum
    buffer_add_uint32(buffer, pcb->quantum);

    //Agrego el pc
    buffer_add_uint32(buffer, pcb->program_counter);

    // CPU registers
    uint32_t buffer_cpu_registers_size = get_cpu_registers_size(get_cpu_registers(pcb));
    t_buffer *buffer_cpu_registers = buffer_create(buffer_cpu_registers_size);

    serialize_cpu_registers(buffer_cpu_registers, get_cpu_registers(pcb));

    //Agrego size del buffer_cpu_registers
    buffer_add_uint32(buffer, buffer_cpu_registers_size);

    //Agrego el contenido de buffer_cpu_registers en buffer 
    buffer_add_buffer(buffer, buffer_cpu_registers);

    buffer_destroy(buffer_cpu_registers);
}

t_PCB* deserialize_pcb(t_buffer *buffer)
{
    // Obtengo pid
    uint32_t pid = buffer_read_uint32(buffer);

    // Obtengo quantum 
    uint32_t quantum = buffer_read_uint32(buffer);

    // Creo pcb
    t_PCB* pcb = pcb_create(pid, quantum);

    // Obtengo pc
    pcb->program_counter = buffer_read_uint32(buffer);

    // Obtengo el tamaño del sub-buffer de t_cpu_registers
    uint32_t buffer_cpu_registers_size = buffer_read_uint32(buffer);

    // Creo sub-buffer de t_cpu_registers
    t_buffer* buffer_cpu_registers = buffer_create(buffer_cpu_registers_size);

    // Agrego parte del contenido de buffer en buffer_cpu_registers
    buffer_add_partial_buffer(buffer_cpu_registers, buffer, buffer_cpu_registers_size);

    // Obtengo los registros de la cpu
    deserialize_cpu_registers(buffer_cpu_registers, pcb->cpu_registers);
    
    buffer_destroy(buffer_cpu_registers);

    return pcb;
}

void deserialize_cpu_registers(t_buffer *buffer, t_cpu_registers *cpu_registers)
{
    cpu_registers->pc= buffer_read_uint32(buffer);
    cpu_registers->ax= buffer_read_uint8(buffer);
    cpu_registers->bx= buffer_read_uint8(buffer);
    cpu_registers->cx= buffer_read_uint8(buffer);
    cpu_registers->dx= buffer_read_uint8(buffer);
    cpu_registers->eax= buffer_read_uint32(buffer);
    cpu_registers->ebx= buffer_read_uint32(buffer);
    cpu_registers->ecx= buffer_read_uint32(buffer);
    cpu_registers->edx= buffer_read_uint32(buffer);
    cpu_registers->si= buffer_read_uint32(buffer);
    cpu_registers->di= buffer_read_uint32(buffer);
}

void serialize_nuevo_proceso(t_buffer *buffer, t_new_process *nuevo_proceso) {

    //Agrego el pid del nuevo proceso
    buffer_add_uint32(buffer, nuevo_proceso->pid);

    //Agrego el largo de la cadena y el path
    buffer_add_string(buffer, nuevo_proceso->path);
}

t_new_process* deserialize_nuevo_proceso(t_buffer* buffer) {
    if(buffer == NULL) return NULL;

    //Obtenemos el pid del nuevo proceso
    uint32_t pid_new_process = buffer_read_uint32(buffer);

    //Obtenemos el size del path relativo
    uint32_t path_length= buffer_read_uint32(buffer);

    //Obtenemos el path relativo del codigo
    char* path_relative = buffer_read_string(buffer, path_length);

    return create_new_process(pid_new_process, path_relative);
}

void serialize_instruction(t_buffer *buffer, t_instruction *instruccion) {
    // Serializar el nombre de la instrucción
    buffer_add_uint32(buffer, (uint32_t)instruccion->name);

    // Serializar el número de parámetros
    uint32_t num_params = list_size(instruccion->params);
    buffer_add_uint32(buffer, num_params);

    // Serializar cada parámetro
    for (int i = 0; i < num_params; i++) {
        char* param = list_get(instruccion->params, i);
        buffer_add_string(buffer, param);
    }
}

t_instruction* deserialize_instruction(t_buffer *buffer) {

    // Obtengo el nombre de la instruccion
    t_name_instruction name = buffer_read_uint32(buffer);

    // Obtengo la cantidad de parametros
    uint32_t count_params = buffer_read_uint32(buffer);

    // Creo la lista donde se guardaran los parametros
    t_list* params = list_create();

    for(size_t i = 0; i < count_params; i++) {
        uint32_t size_param = buffer_read_uint32(buffer);

        // Leo el parametro
        char* param = buffer_read_string(buffer, size_param);

        // Agrego el parametro a la lista de parametros
        list_add(params, param);
    }

    return crear_instruccion_con_parametros(name, params);
}

void serialize_next_instruction(t_buffer* buffer, t_next_instruction* next_instruction) {
    // Serializar el pid del proceso
    buffer_add_uint32(buffer, obtener_pid_process(next_instruction));

    // Serializar el program counter que apunta a la siguiente instruccion
    buffer_add_uint32(buffer, obtener_pc_process(next_instruction));
}

t_next_instruction* deserialize_next_instruction(t_buffer* buffer) {
    // Obtengo el pid del proceso
    uint32_t pid = buffer_read_uint32(buffer);

    // Obtengo el program counter que apunta a la siguiente instruccion
    uint32_t pc = buffer_read_uint32(buffer);

    return crear_siguiente_instruccion(pid, pc);
}