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
    }
    else if (strcmp(comando_consola[0], "DETENER_PLANIFICACION") == 0)
    {
        planificador_status = false;
    }
    else if (strcmp(comando_consola[0], "INICIAR_PLANIFICACION") == 0)
    {
        if(!planificador_status)
        planificador_status = true;
    }
    else if (strcmp(comando_consola[0], "MULTIPROGRAMACION") == 0)
    {
        // código correspondiente
    }
    else if (strcmp(comando_consola[0], "PROCESO_ESTADO") == 0)
    {
        // código correspondiente
    }
    else if (strcmp(comando_consola[0], "EJECUTAR_SCRIPT") == 0)
    {
        char *path = strdup(comando_consola[1]);
        f_ejecutar_script(path);
        free(path);
    }
    else
    {
        log_error(logger_kernel, "Comando no reconocido, pero que paso el filtro ???");
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
    
    log_info(logger_kernel, "Se crea el proceso <%d> en NEW", pcb->pid);

    // Cargo el pcb a la tabla de pcbs
    add_pcb(pcb);

    log_info(logger_kernel, "Se serializa el nuevo proceso para enviar a memoria la creacion de la imagen del proceso");

    // Se envia un t_new_process a memoria
    send_new_process(fd_kernel_memoria, pid, path);

    // SEMAFORO PLANIFICADOR
    // Agregar PCB a la cola de NEW
    process_to_new(pcb);
}

void enviar_pcb_cpu()
{
    t_PCB *pcb = get_pcb(1);
    // envio a cpu el pcb
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

    // char* linea = malloc(sizeof(char)*256);
    // while (fgets(linea, sizeof(linea), file))
    // {
    //     // Eliminar el salto de línea si existe
    //     linea[strcspn(linea, "\n")] = '\0';

    //     log_info(logger_kernel, "Ejecutando comando: %s", linea);

    //     // Ejecutar el comando leído
    //     // Aquí se debería llamar a la función correspondiente al comando leído
    //     _atender_instruccion(linea);
    // }

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
