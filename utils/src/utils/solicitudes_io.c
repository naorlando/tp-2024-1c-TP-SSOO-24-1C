#include "solicitudes_io.h"

//===============================================
// FUNCIONES DE SERIALIZACION
//===============================================
void serializar_solicitud_io_generica(t_buffer* buffer, t_solicitud_io_generica* solicitud) {
    serialize_pcb(buffer, solicitud->pcb);
    buffer_add_string(buffer, solicitud->nombre_interfaz);
    serializar_io_generica(buffer, solicitud->generica);
}

void serializar_io_generica(t_buffer* buffer, t_io_generica* io_generica) {
    buffer_add_string(buffer, io_generica->nombre_interfaz);
    buffer_add_uint32(buffer, io_generica->tiempo_sleep);
}

void serializar_solicitud_io_stdin(t_buffer* buffer, t_solicitud_io_stdin* solicitud) {
    serialize_pcb(buffer, solicitud->pcb);
    buffer_add_string(buffer, solicitud->nombre_interfaz);
    serializar_io_stdin(buffer, solicitud->io_stdin);
}

void serializar_io_stdin(t_buffer* buffer, t_io_stdin* io_stdin) {
    buffer_add_uint32(buffer, io_stdin->direccion_fisica);
    buffer_add_uint32(buffer, io_stdin->tamanio);
}

void serializar_solicitud_io_stdout(t_buffer* buffer, t_solicitud_io_stdout* solicitud) {
    serialize_pcb(buffer, solicitud->pcb);
    buffer_add_string(buffer, solicitud->nombre_interfaz);
    serializar_io_stdout(buffer, solicitud->io_stdout);
}

void serializar_io_stdout(t_buffer* buffer, t_io_stdout* io_stdout) {
    buffer_add_uint32(buffer, io_stdout->direccion_fisica);
    buffer_add_uint32(buffer, io_stdout->tamanio);
}

//===============================================
// FUNCIONES DE DESERIALIZACIÓN
//===============================================
t_solicitud_io_generica* deserializar_solicitud_io_generica(t_buffer* buffer) {
    t_PCB* pcb = deserialize_pcb(buffer);

    uint32_t length_string = buffer_read_uint32(buffer);
    char* nombre_interfaz = buffer_read_string(buffer,length_string);

    t_io_generica* generica = deserializar_io_generica(buffer);

    return crear_solicitud_io_generica(pcb, nombre_interfaz, generica);
}

t_io_generica* deserializar_io_generica(t_buffer* buffer) {
    uint32_t length_string = buffer_read_uint32(buffer);
    char* nombre_interfaz = buffer_read_string(buffer,length_string);

    uint32_t tiempo_sleep = buffer_read_uint32(buffer);

    return crear_io_generica(nombre_interfaz, tiempo_sleep);
}

t_solicitud_io_stdin* deserializar_solicitud_io_stdin(t_buffer* buffer) {
    t_PCB* pcb = deserialize_pcb(buffer);

    uint32_t length_string = buffer_read_uint32(buffer);
    char* nombre_interfaz = buffer_read_string(buffer,length_string);

    t_io_stdin* io_stdin = deserializar_io_stdin(buffer);

    return crear_solicitud_io_stdin(pcb, nombre_interfaz, io_stdin);
}

t_io_stdin* deserializar_io_stdin(t_buffer* buffer) {
    uint32_t direccion_fisica = buffer_read_uint32(buffer);
    uint32_t tamanio = buffer_read_uint32(buffer);
    return crear_io_stdin(direccion_fisica, tamanio);
}

t_solicitud_io_stdout* deserializar_solicitud_io_stdout(t_buffer* buffer) {
    t_PCB* pcb = deserialize_pcb(buffer);

    uint32_t length_string = buffer_read_uint32(buffer);
    char* nombre_interfaz = buffer_read_string(buffer,length_string);

    t_io_stdout* io_stdout = deserializar_io_stdout(buffer);

    return crear_solicitud_io_stdout(pcb, nombre_interfaz, io_stdout);
}

t_io_stdout* deserializar_io_stdout(t_buffer* buffer) {
    uint32_t direccion_fisica = buffer_read_uint32(buffer);
    uint32_t tamanio = buffer_read_uint32(buffer);
    return crear_io_stdout(direccion_fisica, tamanio);
}

//===============================================
// FUNCIONES DE CREACIÓN
//===============================================
t_solicitud_io_generica* crear_solicitud_io_generica(t_PCB* pcb, char* nombre_interfaz, t_io_generica* generica) {
    t_solicitud_io_generica* solicitud = malloc(sizeof(t_solicitud_io_generica));

    if(solicitud == NULL) return NULL;

    solicitud->pcb = pcb;
    solicitud->nombre_interfaz = strdup(nombre_interfaz);
    solicitud->generica = generica;

    return solicitud;
}

t_io_generica* crear_io_generica(char* nombre_interfaz, uint32_t tiempo_sleep) {
    t_io_generica* io_generica = malloc(sizeof(t_io_generica));
    
    if(io_generica == NULL) return NULL;

    io_generica->nombre_interfaz = strdup(nombre_interfaz);
    io_generica->tiempo_sleep = tiempo_sleep;

    return io_generica;
}

t_solicitud_io_stdin* crear_solicitud_io_stdin(t_PCB* pcb, char* nombre_interfaz, t_io_stdin* io_stdin) {
    t_solicitud_io_stdin* solicitud = malloc(sizeof(t_solicitud_io_stdin));

    if(solicitud == NULL) return NULL;

    solicitud->pcb = pcb;
    solicitud->nombre_interfaz = strdup(nombre_interfaz);
    solicitud->io_stdin = io_stdin;

    return solicitud;
}

t_io_stdin* crear_io_stdin(uint32_t direccion_fisica, uint32_t tamanio) {
    t_io_stdin* io_stdin = malloc(sizeof(t_io_stdin));

    if(io_stdin == NULL) return NULL;

    io_stdin->direccion_fisica = direccion_fisica;
    io_stdin->tamanio = tamanio;

    return io_stdin;
}

t_solicitud_io_stdout* crear_solicitud_io_stdout(t_PCB* pcb, char* nombre_interfaz, t_io_stdout* io_stdout) {
    t_solicitud_io_stdout* solicitud = malloc(sizeof(t_solicitud_io_stdout));

    if(solicitud == NULL) return NULL;

    solicitud->pcb = pcb;
    solicitud->nombre_interfaz = strdup(nombre_interfaz);
    solicitud->io_stdout = io_stdout;

    return solicitud;
}

t_io_stdout* crear_io_stdout(uint32_t direccion_fisica, uint32_t tamanio) {
    t_io_stdout* io_stdout = malloc(sizeof(t_io_stdout));

    if(io_stdout == NULL) return NULL;

    io_stdout->direccion_fisica = direccion_fisica;
    io_stdout->tamanio = tamanio;

    return io_stdout;
}

//===============================================
// FUNCIONES DE ACCESO A CAMPOS
//===============================================

/*********** Functiones 't_io_generica' ***********/
char* obtener_nombre_interfaz_generica(t_io_generica* io_generica)
{
    return io_generica->nombre_interfaz;
}

uint32_t obtener_tiempo_sleep(t_io_generica* io_generica)
{
    return io_generica->tiempo_sleep;
}

/*********** Functiones 't_io_stdin' ***********/
uint32_t obtener_direccion_fisica_stdin(t_io_stdin* io_stdin)
{
    return io_stdin->direccion_fisica;
}

uint32_t obtener_tamanio_stdin(t_io_stdin* io_stdin)
{
    return io_stdin->tamanio;
}

/*********** Functiones 't_io_stdout' ***********/
uint32_t obtener_direccion_fisica_stdout(t_io_stdout* io_stdout)
{
    return io_stdout->direccion_fisica;
}

uint32_t obtener_tamanio_stdout(t_io_stdout* io_stdout)
{
    return io_stdout->tamanio;
}

/*********** Functiones 't_solicitud_io_generica' ***********/
t_PCB* obtener_pcb_solicitud_generica(t_solicitud_io_generica* solicitud) 
{
    return solicitud->pcb;
}

char* obtener_nombre_solicitud_generica(t_solicitud_io_generica* solicitud)
{
    return solicitud->nombre_interfaz;
}

t_io_generica* obtener_io_solicitud_generica(t_solicitud_io_generica* solicitud)
{
    return solicitud->generica;
}

/*********** Functiones 't_solicitud_io_stdin' ***********/
t_PCB* obtener_pcb_solicitud_stdin(t_solicitud_io_stdin* solicitud) 
{
    return solicitud->pcb;
}

char* obtener_nombre_solicitud_stdin(t_solicitud_io_stdin* solicitud)
{
    return solicitud->nombre_interfaz;
}

t_io_stdin* obtener_io_solicitud_stdin(t_solicitud_io_stdin* solicitud)
{
    return solicitud->io_stdin;
}

/*********** Functiones 't_solicitud_io_stdout' ***********/
t_PCB* obtener_pcb_solicitud_stdout(t_solicitud_io_stdout* solicitud) 
{
    return solicitud->pcb;
}

char* obtener_nombre_solicitud_stdout(t_solicitud_io_stdout* solicitud)
{
    return solicitud->nombre_interfaz;
}

t_io_stdout* obtener_io_solicitud_stdout(t_solicitud_io_stdout* solicitud)
{
    return solicitud->io_stdout;
}

//===============================================
// FUNCIONES DE DESTRUCCIÓN
//===============================================
void destruir_solicitud_io_generica(t_solicitud_io_generica* solicitud) {
    free(solicitud->nombre_interfaz);
    destruir_io_generica(solicitud->generica);
    free(solicitud);
}

void destruir_io_generica(t_io_generica* io_generica) {
    free(io_generica->nombre_interfaz);
    free(io_generica);
}

void destruir_solicitud_io_stdin(t_solicitud_io_stdin* solicitud) {
    free(solicitud->nombre_interfaz);
    destruir_io_stdin(solicitud->io_stdin);
    free(solicitud);
}

void destruir_io_stdin(t_io_stdin* io_stdin) {
    free(io_stdin);
}

void destruir_solicitud_io_stdout(t_solicitud_io_stdout* solicitud) {
    free(solicitud->nombre_interfaz);
    destruir_io_stdout(solicitud->io_stdout);
    free(solicitud);
}

void destruir_io_stdout(t_io_stdout* io_stdout) {
    free(io_stdout);
}