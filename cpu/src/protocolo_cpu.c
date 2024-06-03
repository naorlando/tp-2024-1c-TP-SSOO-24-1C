#include "protocolo_cpu.h"

int send_example_memoria()
{
    char *cadena = "CPU ENVIO MENSAJE A MEMORIA";
    uint8_t size_cadena = strlen(cadena) + 1; // Include null terminator
    uint32_t buffer_size = sizeof(uint8_t) * 2 + size_cadena;
    t_package *package_example = package_create(EXAMPLE, buffer_size);
    t_message_example *example = malloc(sizeof(t_message_example));

    example->cadena = malloc(strlen(cadena) + 1);
    strcpy(example->cadena, cadena);
    example->entero = 10;

    example_serialize_msg(package_example->buffer, example);

    package_send(package_example, fd_memoria);

    free(example->cadena);
    free(example);
    package_destroy(package_example);
    return 0;
}

int recv_example_msg_kernel()
{
    log_info(logger_cpu, "<<<<< EXAMPLE RECIVE MESSAGE FROM KERNEL >>>>");
    t_message_example *new_msg = malloc(sizeof(t_message_example));
    t_buffer *new_buffer = recive_full_buffer(fd_kernel_dispatch);
    example_deserialize_msg(new_buffer, new_msg);
    log_info(logger_cpu, "MENSAJE => %s", new_msg->cadena);
    log_info(logger_cpu, "ENTERO => %d", new_msg->entero);
    free(new_msg->cadena);
    free(new_msg);
    buffer_destroy(new_buffer);

    return 0;
}

t_PCB* recv_pcb_cpu()
{
    t_buffer* buffer = recive_full_buffer(fd_kernel_dispatch);
    t_PCB* pcb = deserialize_pcb(buffer);
    log_info(logger_cpu, "PCB pc => %d", pcb->program_counter);
    log_info(logger_cpu, "PCB Quantum => %d", pcb->quantum);
    log_info(logger_cpu, "PCB cpu_registers AX => %d", pcb->cpu_registers->ax);

    //pcb_destroy(pcb);
    buffer_destroy(buffer);

    return pcb;
}

// void atender_instruccion_memoria()
// {
//     //DECODE:

//     t_buffer *new_buffer = recive_full_buffer(fd_memoria);
//     t_instruction *instruccion = deserialize_instruction(new_buffer); //TODO
//     log_info(logger_cpu, "INSTRUCCION => %d", instruccion->name);
//     //TODO corregir para recorrer la lista y loggear los parametros
//     // log_info(logger_cpu, "PARAMETROS => %s", instruccion->params);

//     // EXECUTE:
//     //TODO crear variable global de los registros de la cpu 
//     t_cpu_registers* registros = NULL;
//     ejecutar_instruccion(instruccion,registros);//TODO: antes implementar la logica de registros globales


//     buffer_destroy(new_buffer);
//     instruccion_destroy(instruccion); //TODO
    
// }

t_instruction* recv_instruction()
{
    t_buffer* buffer = recive_full_buffer(fd_memoria);
    t_instruction* instruction = deserialize_instruction(buffer);
    
    log_info(logger_cpu, "Se recibio una instruccion de memoria");

    return instruction;
}

void send_get_next_instruction(uint32_t pid, uint32_t program_counter)
{
    t_next_instruction* next_instruction = crear_siguiente_instruccion(pid, program_counter);

    // Creo el paquete que se va a enviar a memoria
    t_package* package = package_create(MSG_NEXT_INSTRUCTION_CPU, obtener_next_instruction_size(next_instruction));

    // Serializo en el buffer el t_next_instruction
    serialize_next_instruction(get_buffer(package), next_instruction);

    // Envio el paquete a memoria
    package_send(package, fd_memoria);

    // Elimino t_next_instruction
    eliminar_next_instruction(next_instruction);

    //Elimino el paquete usado
    package_destroy(package);
}

void send_pcb_kernel()
{
    send_pcb(MSG_PCB_KERNEL, fd_kernel_dispatch, pcb_execute);
}