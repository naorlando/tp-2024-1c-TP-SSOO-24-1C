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
            _establecer_registro(cpu_registers, reg, value);
            log_info(logger_cpu, "SET %s %d\n", reg, value);
            break;
        }
        case MOV_IN: {
            char *reg_datos = (char *)list_get(instruccion->params, 0);
            char *reg_direccion = (char *)list_get(instruccion->params, 1);

            uint32_t direccion_logica = _obtener_valor_registro(cpu_registers, reg_direccion);
            uint32_t valor_memoria = leer_memoria(direccion_logica);
            _establecer_registro(cpu_registers, reg_datos, valor_memoria);

            log_info(logger_cpu, "MOV_IN %s %s\n", reg_datos, reg_direccion);
            break;
        }
        case MOV_OUT: {
            char *reg_direccion = (char *)list_get(instruccion->params, 0);
            char *reg_datos = (char *)list_get(instruccion->params, 1);

            uint32_t direccion_logica = _obtener_valor_registro(cpu_registers, reg_direccion);
            uint32_t valor_datos = _obtener_valor_registro(cpu_registers, reg_datos);
            escribir_memoria(direccion_logica, valor_datos);

            log_info(logger_cpu, "MOV_OUT %s %s\n", reg_direccion, reg_datos);
            break;
        }
        case SUM: {
            char *reg_dest = (char *)list_get(instruccion->params, 0);
            char *reg_src = (char *)list_get(instruccion->params, 1);
            uint8_t valor_src = _obtener_valor_registro(cpu_registers, reg_src);
            uint8_t valor_operacion = _obtener_valor_registro(cpu_registers, reg_dest) + valor_src;
            _establecer_registro(cpu_registers, reg_dest, valor_operacion);
            log_info(logger_cpu, "SUM %s %s\n", reg_dest, reg_src);
            break;
        }
         case SUB: {
            char *reg_dest = (char *)list_get(instruccion->params, 0);
            char *reg_src = (char *)list_get(instruccion->params, 1);
            uint32_t valor_src = _obtener_valor_registro(cpu_registers, reg_src);
            uint32_t valor_operacion = _obtener_valor_registro(cpu_registers, reg_dest) - valor_src;
            _establecer_registro(cpu_registers, reg_dest, valor_operacion);
            log_info(logger_cpu, "SUB %s %s\n", reg_dest, reg_src);
            break;
        }
        case JNZ: {
            char *reg = (char *)list_get(instruccion->params, 0);
            int instruction_index = atoi((char *)list_get(instruccion->params, 1));
            uint32_t valor_reg = _obtener_valor_registro(cpu_registers, reg);

            if (valor_reg != 0) {
                cpu_registers->pc = instruction_index;
                log_trace(logger_cpu, "el registro %s era igual a cero, por lo tanto se salto a %d (JNZ)\n", reg, instruction_index);
            }
                log_info(logger_cpu, "JNZ %s %d\n", reg, instruction_index);

            // le restamos uno al PC ya que al finalizar Execute se le va a sumar 1 al PC: entonces esto se cancela.
            cpu_registers->pc--;
            break;
        }
        case RESIZE: {
            int nuevo_tamano = atoi((char *)list_get(instruccion->params, 0));
            
            // Función para solicitar a la memoria el ajuste de tamaño
            if (!ajustar_tamano_proceso(cpu_registers, nuevo_tamano)) {
                // TODO: Si la memoria devuelve Out of Memory, devolver el contexto al Kernel
                informar_kernel_error("Out of Memory");
            } else {
                log_info(logger_cpu, "RESIZE %d\n", nuevo_tamano);
            }
            break;
        }
        case COPY_STRING: {
            int tamano = atoi((char *)list_get(instruccion->params, 0));
            
            uint32_t direccion_origen = cpu_registers->si;
            uint32_t direccion_destino = cpu_registers->di;
            
            copiar_cadena(direccion_origen, direccion_destino, tamano);
            
            log_info(logger_cpu, "COPY_STRING %d\n", tamano);
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

// FUNCIONES AUXILIARES DE EJECUCION DE INSTRUCCIONES:

//#############################################################################################################
// TODO: REVISAR MACHEO DE INT, debuggear.
//#############################################################################################################
// Función para obtener el puntero a un registro basado en su nombre:
uint32_t* _obtener_registro(t_cpu_registers *registros, const char *nombre) {
    if (strcmp(nombre, "AX") == 0) return (uint32_t *)&registros->ax;
    if (strcmp(nombre, "BX") == 0) return (uint32_t *)&registros->bx;
    if (strcmp(nombre, "CX") == 0) return (uint32_t *)&registros->cx;
    if (strcmp(nombre, "DX") == 0) return (uint32_t *)&registros->dx;
    if (strcmp(nombre, "EAX") == 0) return &registros->eax;
    if (strcmp(nombre, "EBX") == 0) return &registros->ebx;
    if (strcmp(nombre, "ECX") == 0) return &registros->ecx;
    if (strcmp(nombre, "EDX") == 0) return &registros->edx;
    if (strcmp(nombre, "SI") == 0) return &registros->si;
    if (strcmp(nombre, "DI") == 0) return &registros->di;
    return NULL;
}

// Función para establecer el valor de un registro
// void _establecer_registro(t_cpu_registers *registros, const char *nombre, uint32_t valor) {
//     uint32_t *reg = _obtener_registro(registros, nombre);
//     if (reg) *reg = valor;
// }
void _establecer_registro(t_cpu_registers *registros, char *nombre, uint32_t valor) {
    if (strcmp(nombre, "AX") == 0) registros->ax = valor;
    if (strcmp(nombre, "BX") == 0) registros->bx = valor;
    if (strcmp(nombre, "CX") == 0) registros->cx = valor;
    if (strcmp(nombre, "DX") == 0) registros->dx = valor;
    if (strcmp(nombre, "EAX") == 0) registros->eax = valor;
    if (strcmp(nombre, "EBX") == 0) registros->ebx = valor;
    if (strcmp(nombre, "ECX") == 0) registros->ecx = valor;
    if (strcmp(nombre, "EDX") == 0) registros->edx = valor;
    if (strcmp(nombre, "SI") == 0) registros->si = valor;
    if (strcmp(nombre, "DI") == 0) registros->di = valor;
}

// Función para obtener el valor de un registro
uint32_t _obtener_valor_registro(t_cpu_registers *registros, char *nombre) {
    remove_newline(nombre);
    uint32_t *reg = _obtener_registro(registros, nombre);
    return reg ? *reg : 0;
}

void remove_newline(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

// #############################################################################################################
// Es importante tener en cuenta las siguientes aclaraciones:
// Una dirección lógica se traduce a una dirección física, pero al copiar un string/registro a memoria, 
// podría estar presente en más de una página (ver sección de MMU).
// #############################################################################################################
uint32_t leer_memoria(uint32_t direccion_logica){
    //TODO: se relaciona con la MMU, implementar cuando la MMU esté desarrollada.
}

void escribir_memoria(uint32_t direccion_logica,uint32_t valor_datos){
    //TODO: se relaciona con la MMU, implementar cuando la MMU esté desarrollada.
}

bool ajustar_tamano_proceso(t_cpu_registers *cpu_registers, int nuevo_tamano) {
    // TODO: Implementa la lógica para solicitar el ajuste de tamaño a la memoria
    // Devuelve true si el ajuste es exitoso, false si hay un error (por ejemplo, Out of Memory)
    // Ejemplo simplificado
    // if (memoria_tiene_espacio(nuevo_tamano)) {
    //     // Ajustar el tamaño en la memoria
    //     return true;
    // } else {
    //     return false;
    // }
}

void informar_kernel_error(const char *mensaje) {
    // TODO: Implementa la lógica para informar al Kernel sobre un error.
    printf("Kernel error: %s\n", mensaje);
}

void copiar_cadena(uint32_t origen, uint32_t destino, int tamano) {
    // Implementa la lógica para copiar una cadena de bytes de origen a destino

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

void solicitar_instruccion(uint32_t pid, uint32_t pc) 
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
    log_info(logger_cpu, "antes: AX: %d", cpu_registers->ax);
    log_info(logger_cpu, "antes: BX: %d", cpu_registers->bx);
    log_info(logger_cpu, "antes: CX: %d", cpu_registers->cx);

    // EXECUTE: Ejecuto la instruccion recibida
    ejecutar_instruccion(instruccion, cpu_registers);
    //imprimir todos los cpu_registers:
    log_info(logger_cpu, "despues: AX: %d", cpu_registers->ax);
    log_info(logger_cpu, "despues: BX: %d", cpu_registers->bx);
    log_info(logger_cpu, "despues: CX: %d", cpu_registers->cx);
 
    eliminar_instruccion(instruccion);

    // INTERRUPT: verificar y manejar interrupciones después de ejecutar la instrucción
    if(manejar_interrupcion()) return;

    // actualizar PC:
    cpu_registers->pc++;
    pcb_execute->program_counter = cpu_registers->pc;
    //TODO: Se debe actualizar el PC antes de pedir la siguiente instruccion a memoria
    solicitar_instruccion(pcb_execute->pid, pcb_execute->program_counter);

}

bool manejar_interrupcion() {
    if (interrupcion_pendiente) {
        log_info(logger_cpu, "Interrupción recibida, devolviendo PCB al Kernel");
        //TODO: se debe cargar el nuevo contexto de ejecucion asociado al PCB antes
        // de enviar de nuevo al kernel
        send_pcb_kernel();
        interrupcion_pendiente = false;
        return true;
    }

    return false;
}