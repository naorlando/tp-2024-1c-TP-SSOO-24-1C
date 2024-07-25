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
        // código correspondiente
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
    add_pcb(pcb);

    log_info(logger_kernel, "Se serializa el nuevo proceso para enviar a memoria la creacion de la imagen del proceso");

    // Se envia un t_new_process a memoria
    send_new_process(fd_kernel_memoria, pid, path);

    // Agregar PCB a la cola de NEW
    process_to_new(pcb);
}

void enviar_pcb_cpu()
{
    t_PCB *pcb = get_pcb(1);
    // envio el pcb a cpu 
    send_pcb_cpu(pcb);
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

    dictionary_iterator(table_pcb, closure);  // Iterar sobre el diccionario

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
    // TODO: falta hacer la siguiente logica:
    // En caso de que el proceso se encuentre ejecutando en CPU, 
    // se deberá enviar una señal de interrupción a través de la conexión de interrupt con el mismo y 
    // aguardar a que éste retorne el Contexto de Ejecución antes de iniciar la liberación de recursos.
    if(EXECUTE != NULL && EXECUTE->pid == pid){
        // enviar señal de interrupcion a CPU
        t_interruption* interrupcion_exit = create_interruption(EXIT_INTERRUPT, pid);
        send_interruption_cpu(interrupcion_exit);
        destroy_interruption(interrupcion_exit);


        // ####### esto ya lo hace manager_dispatch.c #######
        // // esperar a que retorne el contexto de ejecucion...
        // pcb = recv_pcb_cpu();
        // // actualizar el pcb en la tabla de pcb:
        // update_pcb(pcb);

        return;
    }

    // ##############################################################

    // elimina el PCB para siempre (ver en largo_plazo.c, ACA SE LIBERAN LOS RECURSOS PARA SALIR DEL DEADLOCK)
    agregar_a_cola_exit(pcb);
    log_info(logger_kernel, "Finaliza el proceso <%d> - Motivo: %s", pcb->pid, obtener_motivo_exit(INTERRUPTED_BY_USER));

    // Eliminar de la tabla de PCBs --> DEPRECADO POR DECISION DE MANTENER UN HISTORIAL DE PCBS MANDADOS A EXIT
    // delete_pcb(pid);
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