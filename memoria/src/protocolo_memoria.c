#include "protocolo_memoria.h"

int recv_example_msg_cpu()
{
    log_info(logger_memoria, "<<<<< EXAMPLE RECIVE MESSAGE FROM CPU>>>>");

    t_message_example *new_msg = recv_example(fd_cpu);

    log_info(logger_memoria, "MENSAJE => %s", get_cadena(new_msg));
    log_info(logger_memoria, "ENTERO => %d", get_entero(new_msg));

    message_example_destroy(new_msg);

    return 0;
}

int recv_example_msg_kernel()
{
    log_info(logger_memoria, "<<<<< EXAMPLE RECIVE MESSAGE FROM KERNEL>>>>");

    t_message_example *new_msg = recv_example(fd_kernel);

    log_info(logger_memoria, "MENSAJE => %s", get_cadena(new_msg));
    log_info(logger_memoria, "ENTERO => %d", get_entero(new_msg));

    message_example_destroy(new_msg);

    return 0;
}

int recv_example_msg_entradasalida()
{
    log_info(logger_memoria, "<<<<< EXAMPLE RECIVE MESSAGE FROM ENTRADASALIDA>>>>");

    t_message_example *new_msg = recv_example(fd_entradasalida);

    log_info(logger_memoria, "MENSAJE => %s", get_cadena(new_msg));
    log_info(logger_memoria, "ENTERO => %d", get_entero(new_msg));

    message_example_destroy(new_msg);

    return 0;
}

t_new_process *recv_new_process_kernel()
{

    log_info(logger_memoria, "Se recibio la solicitud de KERNEL para crear un proceso en memoria");

    // t_new_process* nuevo_proceso= malloc(sizeof(t_new_process)); NO hace falta uso la funcion que crea

    t_new_process *new_process = recv_new_process(fd_kernel);

    // Crear estructuras administrativas necesarias
    // t_proceso* proceso = malloc(sizeof(t_proceso));
    // proceso->pid = nuevo_proceso->pid;
    // proceso->path = strdup(nuevo_proceso->path);

    log_info(logger_memoria, "\nNuevo proceso:\npid:%d\npath relativo: %s", new_process->pid, new_process->path);

    // // Agregar proceso a la lista de procesos
    // pthread_mutex_lock(&mutex_lista_procesos);
    // list_add(lista_procesos, proceso);
    // pthread_mutex_unlock(&mutex_lista_procesos);

    // free(proceso->path);
    // free(proceso);
    // free(nuevo_proceso->path);
    // free(nuevo_proceso);

    return new_process;
}

t_next_instruction *recv_next_instruction_cpu()
{
    log_info(logger_memoria, "Se recibio la solicitud de CPU para obtener la siguiente instruccion de un proceso");

    t_next_instruction *next_instruction = recv_next_instruction(fd_cpu);

    log_info(logger_memoria, "Siguiente instruccion para el proceso:\npid:%d\nprogram counter: %d", obtener_pid_process(next_instruction), obtener_pc_process(next_instruction));

    return next_instruction;
}

void send_instrution_cpu(t_instruction *instruction)
{
    // Retardo para la respuesta
    usleep(obtener_retardo_respuesta(memoria_config) * 1000);

    send_instrution(fd_cpu, instruction);
}

int send_msg_memoria_cpu_init(uint32_t page_size, int fd)
{

    t_package *package = package_create(MSG_MEMORIA_CPU_INIT, sizeof(u_int32_t));

    serialize_uint32_t(package->buffer, 1, page_size);

    package_send(package, fd);

    package_destroy(package);

    return EXIT_SUCCESS;
}
// --   PAGE & FRAME   --

// CPU -> MEMORIA :: MSG_CPU_MEMORIA_PAGE
int recv_msg_cpu_memoria_page(t_buffer *buffer, uint32_t *pid, uint32_t *page)
{

    deserialize_uint32_t(buffer, 2, pid, page);

    return EXIT_SUCCESS;
}

// --   READ DATA    --

// GENERICO -> MEMORIA 
int recv_msg_memoria_data_read(t_buffer *buffer, uint32_t *pid, uint32_t *frame, uint32_t *offset, uint32_t *size_value)
{

    deserialize_uint32_t(buffer, 4, pid, frame, offset, size_value);

    return EXIT_SUCCESS;
}

// --   WRITE DATA    --

// GENERICO -> MEMORIA ::
int recv_msg_memoria_data_write(t_buffer *buffer, uint32_t *pid, uint32_t *frame, uint32_t *offset, uint32_t *size_value)
{
    deserialize_uint32_t(buffer, 4, pid, frame, offset, size_value);

    return EXIT_SUCCESS;
}

// MEMORIA -> CPU :: MSG_MEMORIA_CPU_FRAME
int send_msg_memoria_cpu(int fd, uint32_t frame)
{

    t_package *package = package_create(MSG_MEMORIA_CPU_FRAME, sizeof(u_int32_t));

    serialize_uint32_t(package->buffer, 1, frame);
    usleep(obtener_retardo_respuesta(memoria_config) * 1000);
    package_send(package, fd);

    package_destroy(package);

    return EXIT_SUCCESS;
}

// MEMORIA -> CPU :: MSG_MEMORIA_CPU_DATA_READ
int send_msg_memoria_generic_data_read(void *value, uint32_t size_value, int fd)
{
    t_package *package = package_create(MSG_MEMORIA_GENERIC_DATA_READ, size_value);
    buffer_add_data(package->buffer, value, size_value);

    package_send(package, fd);

    package_destroy(package);

    return EXIT_SUCCESS;
}

int recv_msg_cpu_memoria_resize(t_buffer *buffer, uint32_t *pid, uint32_t *new_size)
{

    deserialize_uint32_t(buffer, 2, pid, new_size);

    return EXIT_SUCCESS;
}

int send_msg_cpu_memoria_resize(uint8_t resize_response, int fd)
{
    t_package *package = package_create(MSG_MEMORIA_CPU_RESIZE, sizeof(uint8_t));

    buffer_add_uint8(package->buffer, resize_response);

    package_send(package,fd);

    package_destroy(package);

    return EXIT_SUCCESS;
}

int recv_msg_cpu_memoria_handshake(t_buffer *buffer, uint32_t *handshake)
{
    deserialize_uint32_t(buffer, 1, handshake);

    return EXIT_SUCCESS;
}
