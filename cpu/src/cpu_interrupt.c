#include "cpu_interrupt.h"


int fd_server_interrupt;
int fd_kernel_interrupt;

// TODO: 
    // headear de fin de quantum (case QUANTUM) --> con la accion de cambiar interrupcion_pendiente = TRUE. mandandole el PID para que el cpu
//  maneje(cargue) el pcb y se lo envie a kernel con su contexto actualizado.

void atender_cpu_kernel_interrupt(t_log* logger)
{   
    bool control_key = 1;
    while (control_key)
    {
        int cod_op = recibir_operacion(fd_kernel_interrupt);

        switch (cod_op)
        {
        case MSG_QUANTUM:
            // logica a implementar cuando llega este tipo de interrupcion.
            interrupcion_pendiente = true;
            sem_wait(&SEM_INTERRUPT); // BINARIO
            // variable global: tipo_de_interrupcion
            // hay condicion de carrera ya que pueden entrar una interrupcion de IO y una de QUANTUM al mismo tiempo, 
            // queriendo modificar la misma variable.
            tipo_de_interrupcion = MSG_QUANTUM;
            log_info(logger, "Se recibio un mensaje de interrupcion por QUANTUM del kernel");
            break;

        case -1:
            log_error(logger, "el KERNEL se desconecto. Terminando servidor");
            control_key = 0;
            break;
        default:
            log_warning(logger, "Operacion desconocida. No quieras meter la pata");
            break;
        }
    }
}

bool manejar_interrupcion() {
    if (interrupcion_pendiente) {
        log_info(logger_cpu, "Interrupción recibida, devolviendo PCB al Kernel");
        //TODO: se debe cargar el nuevo contexto de ejecucion asociado al PCB antes
        // de enviar de nuevo al kernel
        cargar_contexto_ejecucion_a_pcb(pcb_execute);
        send_pcb_kernel_interruption(tipo_de_interrupcion); // aca esta la logica de cual mensaje enviar al kernel segun cual sea el tipo de interrupccion
        sem_post(&SEM_INTERRUPT);
        interrupcion_pendiente = false; 
        log_info(logger_cpu, "PCB enviado al Kernel");
        return true;
    }

    return false;
}

// cargar contexto de ejecucion del cpu a los registros del pcb
void cargar_contexto_ejecucion_a_pcb(t_PCB* pcb) {
    t_cpu_registers* contexto = get_cpu_registers(pcb);

    // Cargo el contexto de ejecucion de la CPU en el pcb
    contexto->pc = cpu_registers->pc;
    contexto->ax = cpu_registers->ax;
    contexto->bx = cpu_registers->bx;
    contexto->cx = cpu_registers->cx;
    contexto->dx = cpu_registers->dx;
    contexto->eax = cpu_registers->eax;
    contexto->ebx = cpu_registers->ebx;
    contexto->ecx = cpu_registers->ecx;
    contexto->edx = cpu_registers->edx;
    contexto->si = cpu_registers->si;
    contexto->di = cpu_registers->di;
}

