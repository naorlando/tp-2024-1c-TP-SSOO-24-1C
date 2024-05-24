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
        case INTRUCCION_MEMORIA_CPU:
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
    t_package *package_example = package_create(EXAMPLE);
    t_message_example *example = malloc(sizeof(t_message_example));
    char *cadena = "CPU ENVIO MENSAJE A MEMORIA";
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

    t_PCB *pcb = malloc(sizeof(t_PCB));
    t_buffer *new_buffer = recive_full_buffer(fd_kernel_dispatch);
    deserialize_pcb(new_buffer, pcb);
    log_info(logger_cpu, "PCB pc => %d", pcb->program_counter);
    log_info(logger_cpu, "PCB Quantum => %d", pcb->quantum);
    log_info(logger_cpu, "PCB cpu_registers AX => %d", pcb->cpu_registers->ax);

    pcb_destroy(pcb);
    buffer_destroy(new_buffer);

    return 0;
}

int atender_instruccion_memoria()
{
    //DECODE:

    t_buffer *new_buffer = recive_full_buffer(fd_memoria);
    t_instruction *instruccion = malloc(sizeof(t_instruction));
    deserialize_instruccion(new_buffer, instruccion); //TODO
    log_info(logger_cpu, "INSTRUCCION => %d", instruccion->name);
    log_info(logger_cpu, "PARAMETROS => %s", instruccion->params);

    // EXECUTE:
 
    ejecutar_instruccion(instruccion,registros);//TODO: antes implementar la logica de registros globales


    buffer_destroy(new_buffer);
    instruccion_destroy(instruccion); //TODO
    return 0;
}

//case de ejecucion de instrucciones:
void ejecutar_instruccion(t_instruccion *instruccion, t_cpu_registers *cpu_registers)
{
    log_info(logger_cpu, "EJECUTANDO INSTRUCCION");
    log_info(logger_cpu, "INSTRUCCION => %s", instruccion->linea_instruccion);
 
 switch (instruccion->name) {
        case SET: {
            char* reg = (char*)list_get(instruccion->params, 0);
            int value = atoi((char*)list_get(instruccion->params, 1));
            //TODO: revisar si es la mejor manera de hacerlo
            if(strcmp(reg, "AX") == 0){
                cpu_registers->ax = value;
            }else if(strcmp(reg, "BX") == 0){
                cpu_registers->bx = value;
            }else if(strcmp(reg, "CX") == 0){
                cpu_registers->cx = value;
            }else if(strcmp(reg, "DX") == 0){
                cpu_registers->dx = value;
            }else if(strcmp(reg, "EAX") == 0){
                cpu_registers->eax = value;
            }else if(strcmp(reg, "EBX") == 0){
                cpu_registers->ebx = value;
            }else if(strcmp(reg, "ECX") == 0){
                cpu_registers->ecx = value;
            }else if(strcmp(reg, "EDX") == 0){
                cpu_registers->edx = value;
            }else if(strcmp(reg, "SI") == 0){
                cpu_registers->si = value;
            }else if(strcmp(reg, "DI") == 0){
                cpu_registers->di = value;
            }
            
            log_info(logger_cpu, "SET %s %d\n", reg, value);
            break;
        }
        case SUM: {
            char* reg_dest = (char*)list_get(instruccion->params, 0);
            char* reg_src = (char*)list_get(instruccion->params, 1);
            //TODO

            log_info(logger_cpu, "SUM %s %s\n", reg_dest, reg_src);
            break;
        }
        case SUB: {
            char* reg_dest = (char*)list_get(instruccion->params, 0);
            char* reg_src = (char*)list_get(instruccion->params, 1);
            //TODO
  
            log_info(logger_cpu, "SUB %s %s\n", reg_dest, reg_src);
            break;
        }
        case JNZ: {
            char* reg = (char*)list_get(instruccion->params, 0);
            int instruction_index = atoi((char*)list_get(instruccion->params, 1));
            //TODO

            log_info(logger_cpu, "JNZ %s %d\n", reg, instruction_index);
            break;
        }
        case IO_GEN_SLEEP: {
            char* interface = (char*)list_get(instruccion->params, 0);
            int units = atoi((char*)list_get(instruccion->params, 1));
            // Aquí iría la lógica para solicitar al Kernel la operación de I/O
            //TODO
            log_info(logger_cpu, "IO_GEN_SLEEP %s %d\n", interface, units);
            break;
        }
        default:
            printf("Instrucción no reconocida\n");
            break;
    }


}

