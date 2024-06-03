#include "cpu_dispatch.h"

//case de ejecucion de instrucciones:
void ejecutar_instruccion(t_instruction *instruccion, t_cpu_registers *cpu_registers)
{
    log_info(logger_cpu, "EJECUTANDO INSTRUCCION");
    log_info(logger_cpu, "INSTRUCCION => %d", instruccion->name);
 
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

void cargar_contexto_ejecucion(t_PCB* pcb) {
    t_cpu_registers* contexto = get_cpu_registers(pcb);

    // Cargo el contexto de ejecucion del pcb en la CPU
    cpu_registers->pc = contexto->pc;
    cpu_registers->ax = contexto->ax;
    cpu_registers->bx = contexto->bx;
    cpu_registers->cx = contexto->cx;
    cpu_registers->dx = contexto->dx;
    cpu_registers->eax = contexto->eax;
    cpu_registers->ebx = contexto->ebx;
    cpu_registers->ecx = contexto->ecx;
    cpu_registers->edx = contexto->edx;
    cpu_registers->si = contexto->si;
    cpu_registers->di = contexto->di;
}

void solicitar_instruccion(int pid, int pc) 
{
    // Pido la siguiente instruccion a memoria
    send_get_next_instruction(pid, pc);
}

void recibir_pcb() 
{
    // Recibo el pcb que manda kernel para ejecutar sus instrucciones
    pcb_execute = recv_pcb_cpu();

    // Cargo el contexto de ejecucion del pcb en los registros de la cpu
    cargar_contexto_ejecucion(pcb_execute);

    // Pido a memoria que me mande las instrucciones del proceso
    solicitar_instruccion(pcb_execute->pid, pcb_execute->program_counter);
}

void manejar_ciclo_de_instruccion() {
    // FETCH: Recibo la instruccion que manda memoria
    t_instruction* instruccion = recv_instruction();

    log_info(logger_cpu, "Instrucción recibida");

    // EXECUTE: Ejecuto la instruccion recibida
    ejecutar_instruccion(instruccion, cpu_registers);
    eliminar_instruccion(instruccion);

    // INTERRUPT: verificar y manejar interrupciones después de ejecutar la instrucción
    if(manejar_interrupcion()) return;

    //TODO: Se debe actualizar el PC antes de pedir la siguiente instruccion a memoria
    solicitar_instruccion(pcb_execute->pid, pcb_execute->program_counter);
}

bool manejar_interrupcion() {
    if (interrupcion_pendiente) {
        log_info(logger, "Interrupción recibida, devolviendo PCB al Kernel");
        //TODO: se debe cargar el nuevo contexto de ejecucion asociado al PCB antes
        // de enviar de nuevo al kernel
        send_pcb_kernel();
        interrupcion_pendiente = false;
        return true;
    }

    return false;
}