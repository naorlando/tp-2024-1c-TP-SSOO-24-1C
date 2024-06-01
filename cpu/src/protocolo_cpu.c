#include "protocolo_cpu.h"

void atender_cpu_kernel_dispatch()
{
    bool control_key = 1;
    while (control_key)
    {
        int cod_op = recibir_operacion(fd_kernel_dispatch);

        switch (cod_op)
        {

        case MSG_KERNEL_CPU_DISPATCH:
            // atender_cpu_kernel_dispatch();
            recv_pcb_cpu();
            break;

        case EXAMPLE:
            recv_example_msg_kernel();
            break;

        case -1:
            log_error(logger_cpu, "el KERNEL se desconecto. Terminando servidor");
            control_key = 0;
            break;
        default:
            log_warning(logger_cpu, "Operacion desconocida en dispatch. No quieras meter la pata");
            break;
        }
    }
}

void atender_cpu_memoria()
{
    bool control_key = 1;
    while (control_key)
    {
        int cod_op = recibir_operacion(fd_memoria);

        switch (cod_op)
        {
        case MSG_INSTRUCTION_MEMORIA:
            atender_instruccion_memoria();
            log_info(logger_cpu, "Se recibio una instruccion de memoria");
            break;
        
        case MSG_MEMORIA_CPU:
            log_info(logger_cpu, "Se recibio un mje del memoria");
            break;

        case -1:
            log_error(logger_cpu, "la memoria se desconecto. Terminando servidor");
            control_key = 0;
            break;
        default:
            log_warning(logger_cpu, "Operacion desconocida en dispatch. No quieras meter la pata");
            break;
        }
    }
}

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

int recv_pcb_cpu()
{
    t_buffer *new_buffer = recive_full_buffer(fd_kernel_dispatch);
    t_PCB* pcb = deserialize_pcb(new_buffer);
    log_info(logger_cpu, "PCB pc => %d", pcb->program_counter);
    log_info(logger_cpu, "PCB Quantum => %d", pcb->quantum);
    log_info(logger_cpu, "PCB cpu_registers AX => %d", pcb->cpu_registers->ax);

    pcb_destroy(pcb);
    buffer_destroy(new_buffer);

    return 0;
}

void atender_instruccion_memoria()
{
    //DECODE:

    t_buffer *new_buffer = recive_full_buffer(fd_memoria);
    t_instruction *instruccion = deserialize_instruction(new_buffer); //TODO
    log_info(logger_cpu, "INSTRUCCION => %d", instruccion->name);
    //TODO corregir para recorrer la lista y loggear los parametros
    // log_info(logger_cpu, "PARAMETROS => %s", instruccion->params);

    // EXECUTE:
    //TODO crear variable global de los registros de la cpu 
    t_cpu_registers* registros = NULL;
    ejecutar_instruccion(instruccion,registros);//TODO: antes implementar la logica de registros globales


    buffer_destroy(new_buffer);
    instruccion_destroy(instruccion); //TODO
    
}

