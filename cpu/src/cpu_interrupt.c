#include "cpu_interrupt.h"

void recibir_interrupcion()
{
    t_interruption* interruption = recv_interruption(fd_kernel_interrupt);

    // logica a implementar cuando llega este tipo de interrupcion.
    sem_wait(&SEM_INTERRUPT); // BINARIO

    // variable global: tipo_de_interrupcion
    // hay condicion de carrera ya que pueden entrar una interrupcion de IO y una de QUANTUM al mismo tiempo, 
    // queriendo modificar la misma variable.
    if(pcb_execute->pid == get_pid_interruption(interruption)){
        interrupcion_pendiente = true;
        tipo_de_interrupcion = get_name(interruption);
    }

    destroy_interruption(interruption);
    //tipo_de_interrupcion = QUANTUM_INTERRUPT;
}
