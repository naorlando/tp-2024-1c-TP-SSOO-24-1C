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
        //planificador_VRR();
        break;
    default:
        log_warning(logger_kernel, "ALGORITMO DE PLANIFICACION desconocido. OJO LOCO, CUIDADO con el config!");
        break;
    }
}
    
// ############################################################################################################
// En los planificadores vamos a manipular la cola de ready nada mas.
// ############################################################################################################
void planificador_FIFO()
{
    while (1)
    {
        t_PCB *pcb = get_next_pcb_to_exec(COLA_READY);
        pcb_execute(pcb);
    }
} 

// planificador ROUND ROBIN:
void planificador_RR()
{
    while (1)
    {
        t_PCB *pcb = get_next_pcb_to_exec(COLA_READY);
        pcb_execute(pcb);
        interrupcion_quantum(EXECUTE->pid, obtener_quantum(kernel_config));
    }
}

// planificador VRR:
void planificador_VRR()
{
    while (1)
    {
        t_PCB *pcb = NULL;
        
        if (!queue_is_empty(COLA_AUX_READY)) {
            pcb = queue_pop(COLA_AUX_READY);
        } else {
            pcb = get_next_pcb_to_exec(COLA_READY);
            pcb->quantum = obtener_quantum(kernel_config);
        }

        // Inicializar el hilo del quantum
        pthread_t hilo_quantum;
        if (pthread_create(&hilo_quantum, NULL, (void*)hilo_quantum_func, (void*)pcb) != 0) {
            log_error(logger_kernel, "Error al crear el hilo de quantum");
            continue;
        }
        pthread_detach(hilo_quantum);

        pcb_execute(pcb);
        iniciar_cronometro();

        // esperar que llegue el pcb y actualizar el quantum...
        // hay que tener una especie de semaforo parecido al.. sem_wait(&SEM_CPU);

    }
    queue_destroy(COLA_AUX_READY);
}

void* hilo_quantum_func(void* arg) {
    t_PCB* pcb = (t_PCB*)arg;
    usleep(pcb->quantum * 1000); // Quantum del PCB ya actualizado.
    enviar_interrupcion_a_cpu(pcb->pid);
    return NULL;
}

void enviar_interrupcion_a_cpu(uint32_t pid)
{
    // enviar mensaje a cpu MSG_QUANTUM
    t_interruption* interrupcion = create_interruption(QUANTUM_INTERRUPT,pid);
    send_interruption_cpu(interrupcion);

    destroy_interruption(interrupcion);
}

// // Función que crea la interrupción quantum
// void interrupcion_quantum(uint32_t pid, uint32_t quantum) {
//     pthread_t hilo_de_quantum;
//     hilo_args args;
//     args.pid = pid;
//     args.quantum = quantum;
//     pthread_create(&hilo_de_quantum, NULL, funcion_hilo_quantum, &args);
//     pthread_join(hilo_de_quantum, NULL);  // Para que el hilo se limpie automáticamente al terminar
// }

// // Función que ejecuta el hilo quantum
// void* funcion_hilo_quantum(void* args) {
//     hilo_args* argumentos = (hilo_args*) args;
//     uint32_t pid = argumentos->pid;
//     uint32_t quantum = argumentos->quantum;

//     // Suponiendo que esta función crea y devuelve un puntero a t_datos_hilo
//     datos_hilo_quantum = datos_hilo_create(pid, quantum, pthread_self()); // TODO: Se debe liberar la estructura si no se manda y se cancela
//     interrupcion_enviada = false; // TODO: Ver si es necesario usar un mutex!
//     usleep(quantum * 1000);

//     pthread_mutex_lock(&MUTEX_EXECUTE);
//     if (EXECUTE != NULL && EXECUTE->pid == get_pid_datos_hilo(datos_hilo_quantum)) {
//         enviar_interrupcion_a_cpu(get_pid_datos_hilo(datos_hilo_quantum));
//     }
//     pthread_mutex_unlock(&MUTEX_EXECUTE);
//     //free(hilo_quantum);
    
//     return NULL;
// }
// void interrupcion_quantum(uint32_t pid) {
//     pthread_t hilo_de_quantum;
//     pthread_create(&hilo_interrupt, NULL, (void *)funcion_hilo_quantum, &pid);
//     hilo_de_quantum = hilo_interrupt;
//     pthread_detach(hilo_de_quantum);  // Para que el hilo se limpie automáticamente al terminar
// }

// void funcion_hilo_quantum(void* pid) {

//     hilo_quantum = datos_hilo_create((uint32_t)pid, kernel_config->QUANTUM, &hilo_interrupt); // TODO:get_quantum
//     usleep(hilo_quantum->quantum / 1000);

//     //pthread_mutex_lock(&MUTEX_EXECUTE);
//     if (EXECUTE != NULL && EXECUTE->pid == get_pid_datos_hilo(hilo_quantum)) {
//         enviar_interrupcion_a_cpu(hilo_quantum->pid);
//     }
//     //pthread_mutex_unlock(&MUTEX_EXECUTE);
//     //free(hilo_quantum);
// }


// Setear pcb en EXECUTE y mandar a ejecutar a CPU
void pcb_execute(t_PCB* pcb)
{
    log_info(logger_kernel, "Se prepara para ejecutar el PCB con PID: <%d>", pcb->pid);
    
    sem_wait(&SEM_CPU);

    pthread_mutex_lock(&MUTEX_EXECUTE);
    EXECUTE = pcb;
    pcb->state = EXEC;
    send_pcb_cpu(pcb);
    pthread_mutex_unlock(&MUTEX_EXECUTE);

    log_info(logger_kernel, "PID: %d - Estado Anterior: %s - Estado Actual: %s", pcb->pid, "READY", "EXEC");
}

t_PCB *get_next_pcb_to_exec(t_queue* queue)
{
    sem_wait(&SEM_READY); // Espera a que haya un PCB en la cola de READY
    t_PCB *pcb_a_tomar;

    log_info(logger_kernel, "Se va a tomar el siguiente PCB de la cola de READY");

    pthread_mutex_lock(&MUTEX_READY);
    pcb_a_tomar = queue_pop(queue);
    pthread_mutex_unlock(&MUTEX_READY);

    return pcb_a_tomar;
}

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

// ############################################################################################################
//       CRONOMETRO: la idea es siempre ir actualizando el quantum del PCB en ejecucion
// ############################################################################################################

void iniciar_cronometro()
{
    // no usamos esto por que ya tenemos un cronometro global
            //t_temporal *temporal = temporal_create();

    // activamos el cronometro:
    cronometro_CPU->status = TEMPORAL_STATUS_RUNNING;
}

// obtiene el tiempo en el que el pcb estuvo en ejecucion
void actualizar_quantum(t_PCB* pcb)
{
    // paramos el cronometro:
    cronometro_CPU->status = TEMPORAL_STATUS_STOPPED;

    // obtengo el tiempo que estuvo el proceso ejecutando en cpu:
    pcb->quantum = pcb->quntum - (uint32_t)temporal_gettime(cronometro_CPU);

    // en vez de crear y destruir constantemente, 
    // ACTUALIZAMOS la variable global cronometro_CPU..,
        //temporal_destroy(t_temporal* temporal);
    cronometro_CPU->elapsed_ms = 0;
}

t_temporal* temporal_create_v2(void) {
	t_temporal* self = malloc(sizeof(t_temporal));

	self->elapsed_ms = 0;
	self->status = TEMPORAL_STATUS_STOPPED;

	clock_gettime(CLOCK_MONOTONIC_RAW, &self->current);

	return self;
}
