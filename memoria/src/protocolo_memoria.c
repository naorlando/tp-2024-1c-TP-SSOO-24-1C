#include "protocolo_memoria.h"

int recv_example_msg_cpu(){
    log_info(logger_memoria, "<<<<< EXAMPLE RECIVE MESSAGE FROM CPU>>>>");
    t_message_example * new_msg = malloc(sizeof(t_message_example));
    t_buffer* new_buffer = recive_full_buffer(fd_cpu);

    example_deserialize_msg(new_buffer, new_msg);
            
    log_info(logger_memoria, "%s", new_msg->cadena);
    log_info(logger_memoria, "%d", new_msg->entero);
    free(new_msg->cadena);
    free(new_msg);
    buffer_destroy(new_buffer);

    return 0;
}

int recv_example_msg_kernel(){
    log_info(logger_memoria, "<<<<< EXAMPLE RECIVE MESSAGE FROM KERNEL>>>>");
    t_message_example * new_msg = malloc(sizeof(t_message_example));
    t_buffer* new_buffer = recive_full_buffer(fd_kernel);

    example_deserialize_msg(new_buffer, new_msg);
            
    log_info(logger_memoria, "%s", new_msg->cadena);
    log_info(logger_memoria, "%d", new_msg->entero);
    free(new_msg->cadena);
    free(new_msg);
    buffer_destroy(new_buffer);

    return 0;
}

int recv_example_msg_entradasalida(){
    log_info(logger_memoria, "<<<<< EXAMPLE RECIVE MESSAGE FROM ENTRADASALIDA>>>>");
    t_message_example * new_msg = malloc(sizeof(t_message_example));
    t_buffer* new_buffer = recive_full_buffer(fd_entradasalida);

    example_deserialize_msg(new_buffer, new_msg);
            
    log_info(logger_memoria, "%s", new_msg->cadena);
    log_info(logger_memoria, "%d", new_msg->entero);
    free(new_msg->cadena);
    free(new_msg);
    buffer_destroy(new_buffer);

    return 0;
}

t_new_process* recv_process_kernel() {
    
    log_info(logger_memoria, "Se recibio la solicitud de KERNEL para crear un proceso en memoria");
    
    t_buffer* buffer = recive_full_buffer(fd_kernel);
    //t_new_process* nuevo_proceso= malloc(sizeof(t_new_process)); NO hace falta uso la funcion que crea

    if(buffer == NULL) return NULL;

    t_new_process* new_process= deserialize_nuevo_proceso(buffer);

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
    buffer_destroy(buffer);

    return new_process;
}

t_next_instruction* recv_next_instruction() 
{
    log_info(logger_memoria, "Se recibio la solicitud de CPU para obtener la siguiente instruccion de un proceso");
    
    t_buffer* buffer = recive_full_buffer(fd_cpu);

    if(buffer == NULL) return NULL;

    t_next_instruction* next_instruction= deserialize_next_instruction(buffer);

    log_info(logger_memoria,"Siguiente instruccion para el proceso:\npid:%d\nprogram counter: %d",obtener_pid_process(next_instruction), obtener_pc_process(next_instruction));

    buffer_destroy(buffer);

    return next_instruction;
}

void send_instrution(t_instruction* instruction)
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