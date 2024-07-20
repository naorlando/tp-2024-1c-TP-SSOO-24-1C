#include "corto_plazo.h"

void planificador_corto_plazo()
{
    switch (_obtener_planificador(obtener_algoritmo_planificacion(kernel_config)))
    {
    case FIFO:
        planificador_FIFO();
        break;
    case RR:
        planificador_RR();
        break;
    case VRR:
        planificador_VRR();
        break;
    default:
        log_warning(logger_kernel, "ALGORITMO DE PLANIFICACION desconocido. OJO LOCO, CUIDADO con el config!");
        break;
    }
}
    
// ############################################################################################################
//  PLANIFICADORES:
// ############################################################################################################

//  FIFO:
void planificador_FIFO()
{
    while (1)
    {
        t_PCB *pcb = get_next_pcb_ready_to_exec();
        pcb_execute(pcb);
    }
} 

//  ROUND ROBIN:
void planificador_RR()
{
    while (1)
    {  
        t_PCB *pcb = get_next_pcb_ready_to_exec();
        pcb_execute(pcb);
        interrupcion_quantum(EXECUTE->pid, obtener_quantum(kernel_config));
    }
}

//  VIRTUAL ROUND ROBIN:
void planificador_VRR()
{
    while (1) {
        t_PCB *pcb = NULL;

        // Jerarquizamos las colas de READY
        if (!queue_is_empty(COLA_AUX_READY)) {
            pcb = get_next_pcb_aux_ready_to_exec();
        } else {
            pcb = get_next_pcb_ready_to_exec();
            pcb->quantum = obtener_quantum(kernel_config);
        }

        // Ejecutar PCB
        pcb_execute(pcb);

        // iniciar cronomoetro:
        cronometro_iniciar();

        // Lógica de interrupción por quantum:
        interrupcion_quantum(pcb->pid, pcb->quantum);

    }
}

// ############################################################################################################
//  FUNCIONES AUXILIARES:
// ############################################################################################################

// -----------------------------------------------------------------------------------------------
//  RR || VRR:
// -----------------------------------------------------------------------------------------------

void interrupcion_quantum(uint32_t pid, uint32_t quantum)
{
    pthread_t hilo_de_quantum;
    hilo_args args;
    args.pid = pid;
    args.quantum = quantum;
    pthread_create(&hilo_de_quantum, NULL, funcion_hilo_quantum, &args);
    // Para que el hilo se limpie automáticamente al terminar.
        // si hago un pthread_exit(), ¿no es mas performante?  ya que se borrarian todas las estructuras dentro del hilo.
    pthread_join(hilo_de_quantum, NULL);  
}

// Función que ejecuta el hilo quantum
void* funcion_hilo_quantum(void* args) 
{
    hilo_args* argumentos = (hilo_args*) args;
    uint32_t pid = argumentos->pid;
    uint32_t quantum = argumentos->quantum;

    // Suponiendo que esta función crea y devuelve un puntero a t_datos_hilo
    datos_hilo_quantum = datos_hilo_create(pid, quantum, pthread_self()); // TODO: Se debe liberar la estructura si no se manda y se cancela
    interrupcion_enviada = false; // TODO: Ver si es necesario usar un mutex!
    usleep(quantum * 1000);

    pthread_mutex_lock(&MUTEX_EXECUTE);
    if (EXECUTE != NULL && EXECUTE->pid == get_pid_datos_hilo(datos_hilo_quantum)) {
        enviar_interrupcion_a_cpu(get_pid_datos_hilo(datos_hilo_quantum));
    }
    pthread_mutex_unlock(&MUTEX_EXECUTE);
    //free(hilo_quantum);
    
    return NULL;
}

void enviar_interrupcion_a_cpu(uint32_t pid)
{
    // enviar mensaje a cpu MSG_QUANTUM
    t_interruption* interrupcion = create_interruption(QUANTUM_INTERRUPT,pid);
    send_interruption_cpu(interrupcion);

    destroy_interruption(interrupcion);
}

// -----------------------------------------------------------------------------------------------
//      GENERALES:
// -----------------------------------------------------------------------------------------------
t_planificador _obtener_planificador(char *str)
{
    if (strcmp(str, "FIFO") == 0)
        return FIFO;
    if (strcmp(str, "RR") == 0)
        return RR;
    if (strcmp(str, "VRR") == 0)
        return VRR;
    return -1;
}

void blocked()
{

    while(1) {
        sem_wait(&SEM_BLOCKED);
        
        t_solicitud* solicitud = get_next_solicitud();

        sem_wait(&SEM_SOLICITUDES);

        if(!procesar_solicitud(solicitud)) {
            // la io no esta conectada desde el principio
            agregar_a_cola_exit(obtener_pcb_solicitud(solicitud));
        }
        
    }
}

void pcb_execute( t_PCB* pcb)
{
    //log_info(logger_kernel, "Se prepara para ejecutar el PCB con PID: <%d>", pcb->pid);
    
    sem_wait(&SEM_CPU);

    pthread_mutex_lock(&MUTEX_EXECUTE);
        EXECUTE = pcb;
        pcb->state = EXEC;
        send_pcb_cpu(pcb);
    pthread_mutex_unlock(&MUTEX_EXECUTE);

    log_info(logger_kernel, "PID: %d - Estado Anterior: %s - Estado Actual: %s", pcb->pid, "READY", "EXEC");
}
