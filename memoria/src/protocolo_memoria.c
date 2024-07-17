#include "protocolo_memoria.h"

int recv_example_msg_cpu(){
    log_info(logger_memoria, "<<<<< EXAMPLE RECIVE MESSAGE FROM CPU>>>>");
    
    t_message_example * new_msg = recv_example(fd_cpu);

    log_info(logger_memoria, "MENSAJE => %s", get_cadena(new_msg));
    log_info(logger_memoria, "ENTERO => %d", get_entero(new_msg));
    
    message_example_destroy(new_msg);

    return 0;
}

int recv_example_msg_kernel(){
    log_info(logger_memoria, "<<<<< EXAMPLE RECIVE MESSAGE FROM KERNEL>>>>");

    t_message_example * new_msg = recv_example(fd_kernel);

    log_info(logger_memoria, "MENSAJE => %s", get_cadena(new_msg));
    log_info(logger_memoria, "ENTERO => %d", get_entero(new_msg));
    
    message_example_destroy(new_msg);

    return 0;
}

int recv_example_msg_entradasalida(){
    log_info(logger_memoria, "<<<<< EXAMPLE RECIVE MESSAGE FROM ENTRADASALIDA>>>>");

    t_message_example * new_msg = recv_example(fd_entradasalida);

    log_info(logger_memoria, "MENSAJE => %s", get_cadena(new_msg));
    log_info(logger_memoria, "ENTERO => %d", get_entero(new_msg));
    
    message_example_destroy(new_msg);

    return 0;
}

t_new_process* recv_new_process_kernel() {
    
    log_info(logger_memoria, "Se recibio la solicitud de KERNEL para crear un proceso en memoria");
    
    //t_new_process* nuevo_proceso= malloc(sizeof(t_new_process)); NO hace falta uso la funcion que crea

    t_new_process* new_process= recv_new_process(fd_kernel);

    // Crear estructuras administrativas necesarias
    //t_proceso* proceso = malloc(sizeof(t_proceso));
    //proceso->pid = nuevo_proceso->pid;
    //proceso->path = strdup(nuevo_proceso->path);

    log_info(logger_memoria,"\nNuevo proceso:\npid:%d\npath relativo: %s",new_process->pid, new_process->path);

    // // Agregar proceso a la lista de procesos
    // pthread_mutex_lock(&mutex_lista_procesos);
    // list_add(lista_procesos, proceso);
    // pthread_mutex_unlock(&mutex_lista_procesos);

    //free(proceso->path);
    //free(proceso);
    //free(nuevo_proceso->path);
    //free(nuevo_proceso);

    return new_process;
}

t_next_instruction* recv_next_instruction_cpu() 
{
    log_info(logger_memoria, "Se recibio la solicitud de CPU para obtener la siguiente instruccion de un proceso");
    
    t_next_instruction* next_instruction= recv_next_instruction(fd_cpu);

    log_info(logger_memoria,"Siguiente instruccion para el proceso:\npid:%d\nprogram counter: %d",obtener_pid_process(next_instruction), obtener_pc_process(next_instruction));

    return next_instruction;
}

void send_instrution_cpu(t_instruction* instruction)
{
    // Creo el paquete que se va a enviar a CPU
    t_package* package = package_create(MSG_INSTRUCTION_MEMORIA, obtener_instruction_size(instruction));

    // Serializo en el buffer el t_instruction
    serialize_instruction(get_buffer(package), instruction);

    // Envio el paquete a memoria
    package_send(package, fd_cpu);

    // Elimino t_instruction
    eliminar_instruccion(instruction);

    //Elimino el paquete usado
    package_destroy(package);
}

int send_msg_memoria_cpu_init( uint32_t page_size, int fd) {

    t_package* package = package_create(MSG_MEMORIA_CPU_INIT,sizeof(u_int32_t));

    serialize_uint32_t(package->buffer, 1, page_size);

    package_send(package, fd);

    package_destroy(package);

    return EXIT_SUCCESS;

}
// --   PAGE & FRAME   --

// CPU -> MEMORIA :: MSG_CPU_MEMORIA_PAGE
int recv_msg_cpu_memoria_page(t_buffer* buffer, uint32_t* pid, uint32_t* page_table_id, uint32_t* page) {

    deserialize_uint32_t(buffer, 3, pid, page_table_id, page);
    
    return EXIT_SUCCESS;
}

// MEMORIA -> CPU :: MSG_MEMORIA_CPU_FRAME
int recv_msg_memoria_cpu_frame(t_buffer* buffer, uint32_t* frame) {
    
    deserialize_uint32_t(buffer, 1, frame);
    
    return EXIT_SUCCESS;
}


// --   READ DATA    --



// CPU -> MEMORIA :: MSG_CPU_MEMORIA_DATA_READ
int recv_msg_cpu_memoria_data_read(t_buffer* buffer, uint32_t* pid,  uint32_t* page, uint32_t* frame, uint32_t* offset) {

    deserialize_uint32_t(buffer, 5, pid, page, frame, offset);
    
    return EXIT_SUCCESS;
}

// MEMORIA -> CPU :: MSG_MEMORIA_CPU_DATA_READ
int recv_msg_memoria_cpu_data(t_buffer* buffer, uint32_t* value) {

    deserialize_uint32_t(buffer, 1, value);
    
    return EXIT_SUCCESS;
}



// --   WRITE DATA    --

// CPU -> MEMORIA :: MSG_MEMORIA_CPU_DATA_WRITE
int recv_msg_cpu_memoria_data_write(t_buffer* buffer, uint32_t* pid, uint32_t* page, uint32_t* frame, uint32_t* offset, uint32_t* value) {

    deserialize_uint32_t(buffer, 6, pid, page, frame, offset, value);
    
    return EXIT_SUCCESS;
}

// MEMORIA -> CPU :: MSG_MEMORIA_CPU_DATA_READ
int send_msg_memoria_cpu_data_read(uint32_t value, int fd) {

    t_package* package = package_create(MSG_MEMORIA_CPU_DATA_READ,sizeof(u_int32_t));

    serialize_uint32_t(package->buffer, 1, value);

    package_send(package, fd);

    package_destroy(package);

    return EXIT_SUCCESS;
}

