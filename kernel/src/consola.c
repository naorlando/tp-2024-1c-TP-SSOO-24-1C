#include <kernel.h>
#include <consola.h>


void iniciar_consola_interactiva(){
    char* leido;
    leido = readline("> ");
    bool validacion_leido;

    while (strcmp(leido, "0") != 0) {
        validacion_leido = _validacion_de_instrucciones_consola(leido);
        if (!validacion_leido) {
            log_error(logger_kernel, "Comando de CONSOLA no reconocido.");
            free(leido);
            leido = readline("> ");
            continue; // Saltar y continuar con el resto de la iteración
        }

        _atender_instruccion(leido);

        free(leido);
        leido = readline("> ");
    }
    free(leido);
}

bool _validacion_de_instrucciones_consola(char *leido){
    bool resultado_validacion = false;

    // TODO: Hacer más controles de validación
    char** comando_consola = string_split(leido, " ");

    // en vez de strcmp para la comparacion podriamos usar:  string_equals_ignore_case(char * actual, char * expected)
    // la unica diferencia es que contempla escribir el comando en mayuscula o minuscula, le da igual.

    if (strcmp(comando_consola[0], "EJECUTAR_SCRIPT") == 0 && arrayLength(comando_consola) == 2) { // 2 son la cantidad de paramentros
        resultado_validacion = true;
    }   else if (strcmp(comando_consola[0], "INICIAR_PROCESO") == 0 && arrayLength(comando_consola) == 2) {
        resultado_validacion = true;
    } else if (strcmp(comando_consola[0], "FINALIZAR_PROCESO") == 0 && arrayLength(comando_consola) == 2) {
        resultado_validacion = true;
    } else if (strcmp(comando_consola[0], "DETENER_PLANIFICACION") == 0  && arrayLength(comando_consola) == 1) {
        resultado_validacion = true;
    } else if (strcmp(comando_consola[0], "INICIAR_PLANIFICACION") == 0 && arrayLength(comando_consola) == 1) {
        resultado_validacion = true;
    } else if (strcmp(comando_consola[0], "MULTIPROGRAMACION") == 0 && arrayLength(comando_consola) == 2) {
        resultado_validacion = true;
    } else if (strcmp(comando_consola[0], "PROCESO_ESTADO") == 0 && arrayLength(comando_consola) == 1) {
        resultado_validacion = true;
    // } else if (strcmp(comando_consola[0], "HELP") == 0) {
    //     resultado_validacion = true;
    // } else if (strcmp(comando_consola[0], "PRINT") == 0) {
    //     resultado_validacion = true;
    } else {
        log_error(logger_kernel, "Error en validacion del comando.");
        resultado_validacion = false;
    }

    string_array_destroy(comando_consola);

    return resultado_validacion;
}

void _atender_instruccion(char *leido) {
    char** comando_consola = string_split(leido, " ");
    pthread_t un_hilo;
    t_buffer* un_buffer = buffer_create(sizeof(leido)+1); //TODO: Revisar el tamaño del buffer

    if (strcmp(comando_consola[0], "INICIAR_PROCESO") == 0) { // [path][size][prioridad]
        buffer_add_string(un_buffer, comando_consola[1]); // [path]
        // buffer_add_string(un_buffer, comando_consola[2]); // [size]
        // buffer_add_string(un_buffer, comando_consola[3]); // [prioridad]
        f_iniciar_proceso(un_buffer);
        pthread_create(&un_hilo, NULL, (void*)f_iniciar_proceso, un_buffer);
        pthread_detach(un_hilo);
    } else if (strcmp(comando_consola[0], "FINALIZAR_PROCESO") == 0) {
        // código correspondiente
    } else if (strcmp(comando_consola[0], "DETENER_PLANIFICACION") == 0) {
        // código correspondiente
    } else if (strcmp(comando_consola[0], "INICIAR_PLANIFICACION") == 0) {
        // código correspondiente
    } else if (strcmp(comando_consola[0], "MULTIPROGRAMACION") == 0) {
        // código correspondiente
    } else if (strcmp(comando_consola[0], "PROCESO_ESTADO") == 0) {
        // código correspondiente
    } else {
        log_error(logger_kernel, "Comando no reconocido, pero que paso el filtro ???");
        exit(EXIT_FAILURE);
    }

    string_array_destroy(comando_consola);
    buffer_destroy(un_buffer);
}

void f_iniciar_proceso(t_buffer* un_buffer) {
    char* path = extract_string_buffer(un_buffer);
    int pid = asignar_pid();

    // Crear el PCB
    log_info(logger_kernel, "KERNEL CREA PCB");
    t_PCB*  pcb = pcb_create(pid, 1);
    t_nuevo_proceso* nuevo_proceso = create_new_process(pid,path);

    log_info(logger_kernel,"pid del pcb: %d",pcb->pid);
 

    

    // Serializar el PCB y enviar a la memoria
    log_info(logger_kernel, "KERNEL SERIALIZA PCB Y ENVIA A MEMORIA");
    t_package *package = package_create(CREAR_PROCESO_KERNEL);
    serialize_nuevo_proceso(package->buffer, nuevo_proceso);
    package_send(package, fd_kernel_memoria);
    package_destroy(package);

    // // Agregar PCB a la cola de NEW
    // pthread_mutex_lock(&mutex_new_queue);
    // queue_push(new_queue, pcb);
    // pthread_mutex_unlock(&mutex_new_queue);

    // Liberar memoria
    free(path);
    free(nuevo_proceso->path);
    free(nuevo_proceso);
    buffer_destroy(un_buffer);
}



int asignar_pid() {
    int valor_pid;
    pthread_mutex_lock(&mutex_pid);
        valor_pid = identificador_PID;
        identificador_PID++;
    pthread_mutex_unlock(&mutex_pid);
    return valor_pid;
}
