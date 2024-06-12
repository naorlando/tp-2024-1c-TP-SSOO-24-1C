#include "cpu_interrupt.h"

// TODO: 
    // headear de fin de quantum (case QUANTUM) --> con la accion de cambiar interrupcion_pendiente = TRUE. mandandole el PID para que el cpu
//  maneje(cargue) el pcb y se lo envie a kernel con su contexto actualizado.

void atender_cpu_kernel_interrupt()
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
            log_info(logger_cpu, "Se recibio un mensaje de interrupcion por QUANTUM del kernel");
            break;

        case -1:
            log_error(logger_cpu, "el KERNEL se desconecto. Terminando servidor");
            control_key = 0;
            break;
        default:
            log_warning(logger_cpu, "Operacion desconocida. No quieras meter la pata");
            break;
        }
    }
}
