#include <consola.h>

void iniciar_consola_interactiva()
{
    char *leido;
    leido = readline("> ");
    bool validacion_leido;

    while (strcmp(leido, "") != 0)
    {
        validacion_leido = _validacion_de_instrucciones_consola(leido);

        if (!validacion_leido)
        {
            log_error(logger_kernel, "Comando de CONSOLA no reconocido.");
            free(leido);
            leido = readline("> ");
            continue; // Saltar y continuar con el resto de la iteración 
        }

        char *leido_copy = strdup(leido);
        if (leido_copy == NULL)
        {
            log_error(logger_kernel, "Error al duplicar el comando de la consola.");
            free(leido);
            leido = readline("> ");
            continue;
        }

        // Se crea un hilo para atender cada comando
        pthread_t hilo_atencion_comando_consola;
        if (pthread_create(&hilo_atencion_comando_consola, NULL, (void *)_atender_instruccion, leido_copy) != 0)
        {
            log_error(logger_kernel, "Error al crear el hilo para atender el comando.");
            free(leido_copy);
            free(leido);
            leido = readline("> ");
            continue;
        }
        pthread_join(hilo_atencion_comando_consola, NULL);

        free(leido);
        leido = readline("> ");
    }
    free(leido);
}

bool _validacion_de_instrucciones_consola(char *leido)
{
    bool resultado_validacion = false;

    char **comando_consola = split(leido, " ");
    if (comando_consola == NULL)
    {
        log_error(logger_kernel, "Error al dividir el comando de la consola.");
        return false;
    }

    int longitud_comando = arrayLength(comando_consola);
    if ((strcmp(comando_consola[0], "EJECUTAR_SCRIPT") == 0 && longitud_comando == 2) ||
        (strcmp(comando_consola[0], "INICIAR_PROCESO") == 0 && longitud_comando == 2) ||
        (strcmp(comando_consola[0], "FINALIZAR_PROCESO") == 0 && longitud_comando == 2) ||
        (strcmp(comando_consola[0], "DETENER_PLANIFICACION") == 0 && longitud_comando == 1) ||
        (strcmp(comando_consola[0], "INICIAR_PLANIFICACION") == 0 && longitud_comando == 1) ||
        (strcmp(comando_consola[0], "MULTIPROGRAMACION") == 0 && longitud_comando == 2) ||
        (strcmp(comando_consola[0], "PROCESO_ESTADO") == 0 && longitud_comando == 1))
    {
        resultado_validacion = true;
    }
    else
    {
        log_error(logger_kernel, "Error en validacion del comando.");
    }

    array_string_destroy(comando_consola);
    return resultado_validacion;
}

void _atender_instruccion(void *args)
{
    char *leido = (char *)args;
    char **comando_consola = split(leido, " ");

    if (strcmp(comando_consola[0], "INICIAR_PROCESO") == 0)
    {
        if (planificador_status)
        {
            char *path = my_strdup(comando_consola[1]);
            f_iniciar_proceso(path);
            free(path);
        }
    }
    else if (strcmp(comando_consola[0], "FINALIZAR_PROCESO") == 0)
    {
        // código correspondiente
        uint32_t pid = atoi(comando_consola[1]);
        f_finalizar_proceso(pid);
    }
    else if (strcmp(comando_consola[0], "DETENER_PLANIFICACION") == 0)
    {
        f_detener_planificacion();
    }
    else if (strcmp(comando_consola[0], "INICIAR_PLANIFICACION") == 0)
    {
        f_iniciar_planificacion();
    }
    else if (strcmp(comando_consola[0], "MULTIPROGRAMACION") == 0)
    {
        f_cambiar_grado_multiprogramacion( (uint16_t) atoi(comando_consola[1]));
    }
    else if (strcmp(comando_consola[0], "PROCESO_ESTADO") == 0)
    {
        f_mostrar_estado_procesos();
    }
    else if (strcmp(comando_consola[0], "EJECUTAR_SCRIPT") == 0)
    {
        char *path = strdup(comando_consola[1]);
        f_ejecutar_script(path);
        free(path);
    }
    else
    {
        log_error(logger_kernel, "Comando no reconocido, pero que paso con el filtro ???");
        exit(EXIT_FAILURE);
    }

    array_string_destroy(comando_consola);
    free(leido);
}

void f_iniciar_proceso(char *path)
{
    int pid = asignar_pid();

    // Creor el PCB
    t_PCB *pcb = pcb_create(pid, obtener_quantum(kernel_config));

    // Cargo el pcb a la tabla de pcbs
    pthread_mutex_lock(&MUTEX_DICTIONARY);
        add_pcb(pcb);
    pthread_mutex_unlock(&MUTEX_DICTIONARY);

    log_info(logger_kernel, "Se serializa el nuevo proceso para enviar a memoria la creacion de la imagen del proceso");

    // Se envia un t_new_process a memoria
    send_new_process(fd_kernel_memoria, pid, path);

    // Agregar PCB a la cola de NEW
    process_to_new(pcb);
}

// Ejecutar un script con comandos de consola
void f_ejecutar_script(const char *filename)
{
    FILE *file = fopen(filename, "r");

    if (file == NULL)
    {
        log_error(logger_kernel, "Error no se pudo abrir el archivo a ejecutar");
        return;
    }

    size_t len = 0;
    ssize_t read;
    char *linea = NULL;

    while ((read = getline(&linea, &len, file)) != -1) {
        // Eliminar el salto de línea si existe
        remove_newline(linea);

        // Duplicar la línea para pasarla a _atender_instruccion
        char *linea_copy = strdup(linea);
        if (linea_copy == NULL)
        {
            log_error(logger_kernel, "Error al duplicar la línea del archivo.");
            continue;
        }

        // Ejecutar el comando leído
        _atender_instruccion(linea_copy);
    }
    free(linea);
    fclose(file);
}

int asignar_pid()
{
    int valor_pid;
    pthread_mutex_lock(&mutex_pid);
    valor_pid = identificador_PID;
    identificador_PID++;
    pthread_mutex_unlock(&mutex_pid);
    return valor_pid;
}

void f_mostrar_estado_procesos(){

    // posibles estados: 
    // NEW,
    // READY,
    // EXEC,
    // BLOCKED,
    // FINISHED 

    // prototipo de Logs: Ingreso a Ready: "Cola Ready / Ready Prioridad: [<LISTA DE PIDS>]"
    printf("--------------------------------------------------------------------------------\n");
    printf("Mostrando estado de los procesos ...\n");
    // NEW
    printf("Estado NEW: ");
    listar_pids_por_estado(NEW);

    // READY
    printf("Estado READY: ");
    listar_pids_por_estado(READY);

    // EXEC
    printf("Estado EXEC: ");
    listar_pids_por_estado(EXEC);

    // BLOCKED
    printf("Estado BLOCKED: ");
    listar_pids_por_estado(BLOCKED);

    // FINISHED
    printf("Estado FINISHED: ");
    listar_pids_por_estado(FINISHED);
    printf("--------------------------------------------------------------------------------\n");
}

void listar_pids_por_estado(t_state state) {

    t_list* pids = list_create();  // Crear una lista para almacenar los PIDs

    void closure(char* key, void* element) {
        t_PCB* pcb = (t_PCB*) element;
        if (pcb->state == state) {
            uint32_t* pid = malloc(sizeof(uint32_t));
            *pid = pcb->pid;
            list_add(pids, pid);
        }
    }

    // mutex:
    pthread_mutex_lock(&MUTEX_DICTIONARY);
        dictionary_iterator(table_pcb, closure);  // Iterar sobre el diccionario
    pthread_mutex_unlock(&MUTEX_DICTIONARY);

    printf("[");
    for (int i = 0; i < list_size(pids); i++) {
        uint32_t* pid = list_get(pids, i);
        printf("%d", *pid);
        if (i < list_size(pids) - 1) {
            printf(", ");
        }
        free(pid);  // Liberar la memoria asignada para el PID
    }
    printf("]\n");

    list_destroy(pids);  // Destruir la lista
}


void f_finalizar_proceso(u_int32_t pid){
    t_PCB *pcb = get_pcb(pid);
    if(pcb == NULL){
        log_error(logger_kernel, "No se encontro el proceso con PID: %d", pid);
        return;
    }
    if(pcb->state == FINISHED){
        log_warning(logger_kernel, "El proceso con PID: %d ya se encuentra finalizado", pid);
        return;
    }
    // ##############################################################
    // En caso de que el proceso se encuentre ejecutando en CPU, 
    // se deberá enviar una señal de interrupción a través de la conexión de interrupt con el mismo y 
    // aguardar a que éste retorne el Contexto de Ejecución antes de iniciar la liberación de recursos.
    if(EXECUTE != NULL && EXECUTE->pid == pid){
        // enviar señal de interrupcion a CPU
        t_interruption* interrupcion_exit = create_interruption(EXIT_INTERRUPT, pid);
        send_interruption_cpu(interrupcion_exit);
        destroy_interruption(interrupcion_exit);
        // el resto de la logica se ve cuando llega el PCB a kernel_server con motivo de desalojo INTERRUPTED_BY_USER
        return;
    }

    // ##############################################################

    // si se encuentra en las distintas listas, eliminarlo de la lista:
    eliminar_de_listas_de_estado(pcb);


    // elimina el PCB para siempre (ver en largo_plazo.c, ACA SE LIBERAN LOS RECURSOS PARA SALIR DEL DEADLOCK)
    agregar_a_cola_exit(pcb);
    log_info(logger_kernel, "Finaliza el proceso <%d> - Motivo: %s", pcb->pid, obtener_motivo_exit(INTERRUPTED_BY_USER));

    // Eliminar de la tabla de PCBs --> DEPRECADO POR DECISION DE MANTENER UN HISTORIAL DE PCBS MANDADOS A EXIT
    // delete_pcb(pid);
}

void eliminar_de_listas_de_estado(t_PCB* pcb)
{
    switch (pcb->state)
    {
    case NEW:
        eliminar_de_new(pcb);
        break;
    case READY:
        eliminar_de_ready(pcb);
        break;
    case BLOCKED:
        eliminar_de_blocked_io(pcb);
        // el liberar recursos asignados lo hacemos en Largo plazo.
        break;
    default:
        break;
    }
}

void eliminar_de_new(t_PCB* pcb)
{
    // Eliminar de la cola de NEW
    pthread_mutex_lock(&MUTEX_NEW);
        list_remove_element(COLA_NEW, (void*)pcb);
    pthread_mutex_unlock(&MUTEX_NEW);
}

void eliminar_de_ready(t_PCB* pcb)
{
    // Eliminar de la cola de READY
    pthread_mutex_lock(&MUTEX_READY);
    bool exito = list_remove_element(COLA_READY, (void*)pcb);
    pthread_mutex_unlock(&MUTEX_READY);
    
}  

void eliminar_de_blocked_io(t_PCB* pcb)
{
    // Eliminar de la cola de BLOCKED de cada io:
    //t_list* nombres_de_ios = dictionary_keys(io_connections);
    
    // ##############################################################
    // TODO: pensar estrategia para cuando la io ya pida desencolar el PCB y lo este ejecutando ya.
    // ##############################################################

    void closure(char* key, void* element) {
        t_IO_connection* io = (t_IO_connection*) element;
        t_PCB *pcb_removido;
        //bool pcb_encontrado = false;
        // pcb match: (condition)
        bool pid_match(void *pcb_ptr) {
            return ((t_PCB *)pcb_ptr)->pid == pcb->pid;
        }
        if (io->cola_procesos_bloqueados != NULL && !list_is_empty(io->cola_procesos_bloqueados)) {
            pthread_mutex_lock(&io->mutex_cola_bloqueados);
                pcb_removido = (t_PCB *)list_remove_by_condition(io->cola_procesos_bloqueados, pid_match);
                // hacer lo mismo pero con: bool list_remove_element(t_list* self, void* element);
                //pcb_encontrado = list_remove_element(io->cola_procesos_bloqueados, (void*)pcb);
            pthread_mutex_unlock(&io->mutex_cola_bloqueados);
        }

        if (pcb_removido != NULL) {
            log_info(logger_kernel, "Proceso %d removido de la cola de bloqueados de la io %s", pcb_removido->pid, io->nombre_interfaz);
        }
    }

    dictionary_iterator(io_connections, closure);
}


void f_iniciar_planificacion(){
    if(!planificador_status)
    {
        planificador_status = true;

        sem_post(&SEM_PLANIFICACION_NEW_READY_INICIADA);
        sem_post(&SEM_PLANIFICACION_READY_INICIADA);
        sem_post(&SEM_PLANIFICACION_EXEC_INICIADA);
        sem_post(&SEM_PLANIFICACION_BLOCKED_INICIADA);

        log_info(logger_kernel, "Planificación %s iniciada", obtener_algoritmo_planificacion(kernel_config));
    }
}

void f_detener_planificacion(){
    
    if(planificador_status)
    {
        planificador_status = false;
        
        pthread_t detener_new, detener_ready, detener_exec, detener_blocked;
        pthread_create(&detener_new,NULL,(void*) detener_cola_new,NULL);
        pthread_create(&detener_ready,NULL,(void*) detener_cola_ready,NULL);
        pthread_create(&detener_exec,NULL,(void*) detener_cola_exec,NULL);
        pthread_create(&detener_blocked,NULL,(void*) detener_cola_blocked,NULL);
        pthread_detach(detener_new);
        pthread_detach(detener_ready);
        pthread_detach(detener_exec);
        pthread_detach(detener_blocked);
        
        
        log_info(logger_kernel, "Se detuvo la planificacion en %s", obtener_algoritmo_planificacion(kernel_config));
    }else
    {
        log_info(logger_kernel,"la planificacion ya se encuentra pausada");
    }
    
}

// void f_cambiar_grado_multiprogramacion(int numero)
// {
//     uint16_t grado_multiprogramacionV2 = grado_multiprogramacion;
//     int diferencia = abs(grado_multiprogramacion - numero);

//     if(numero < grado_multiprogramacion){
//         grado_multiprogramacion =- numero;
//     }else if(numero > grado_multiprogramacion){
//         grado_multiprogramacion =+ numero;
//     }else{
//         log_info(logger_kernel,"El grado de multiprogramacion ya es %d", grado_multiprogramacion);
//         return;
//     }

//     if (numero < grado_multiprogramacionV2){
//         log_warning(logger_kernel, "El grado de multiprogramación ha sido cambiado a %d", numero);
//         //decrementar_multiprogramacion(grado_multiprogramacion - numero);
//         SEM_MULTIPROGRAMACION->__align =- diferencia;
//     } else if (numero > grado_multiprogramacionV2){
//         //incrementar_multiprogramacion(numero - grado_multiprogramacion);
//         log_warning(logger_kernel, "El grado de multiprogramación ha sido cambiado a %d", numero);
//         SEM_MULTIPROGRAMACION->__align =+ diferencia;
//     } else {
//         log_info(logger_kernel, "El grado de multiprogramación ya es %d", grado_multiprogramacion);
//     }

// }

void f_cambiar_grado_multiprogramacion(uint16_t nuevo_grado) {
    pthread_t thread;
    t_multiprogramacion_args* args = malloc(sizeof(t_multiprogramacion_args));
    args->nuevo_grado = nuevo_grado;
    
    pthread_create(&thread, NULL, cambiar_grado_multiprogramacion, args);
    pthread_detach(thread);
    // cambiar_grado_multiprogramacion(args);

}

void* cambiar_grado_multiprogramacion(void* args) 
{   
    
    flag_de_orden_multiprogramacion = true;
    sem_wait(&SEM_ORDEN_CAMBIO_MULTIPROGRAMACION);

    t_multiprogramacion_args* multiprog_args = (t_multiprogramacion_args*)args;
    uint16_t nuevo_grado = multiprog_args->nuevo_grado;
    free(multiprog_args);

    int diferencia = nuevo_grado - grado_multiprogramacion;
    log_warning(logger_kernel, "El grado de multiprogramación ANTES:%d", grado_multiprogramacion);

    if (diferencia > 0) {
        for (int i = 0; i < diferencia; i++) {
            sem_post(&SEM_MULTIPROGRAMACION);    
            log_warning(logger_kernel, "Se incrementa el grado de multiprogramación");
        }
    } else if (diferencia < 0) {


        for (int i = 0; i < abs(diferencia); i++) {
            sem_wait(&SEM_MULTIPROGRAMACION);
            log_warning(logger_kernel, "Se decrementa el grado de multiprogramación");
        }
    } else {
        log_info(logger_kernel, "El grado de multiprogramación ya es %d", grado_multiprogramacion);
    }

    log_warning(logger_kernel, "El grado de multiprogramación DESPUES:%d", grado_multiprogramacion );
    
    //actualizamos la variable global:
    grado_multiprogramacion = nuevo_grado;

    sem_post(&SEM_ORDEN_CAMBIO_MULTIPROGRAMACION);
    flag_de_orden_multiprogramacion = false;

    return NULL;
}
