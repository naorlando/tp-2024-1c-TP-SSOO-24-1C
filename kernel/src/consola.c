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
    t_buffer* un_buffer = buffer_create();

    if (strcmp(comando_consola[0], "INICIAR_PROCESO") == 0) { // [path][size][prioridad]
        buffer_add_string(un_buffer, comando_consola[1]); // [path]
        buffer_add_string(un_buffer, comando_consola[2]); // [size]
        buffer_add_string(un_buffer, comando_consola[3]); // [prioridad]
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
    char* size = extract_string_buffer(un_buffer);
    char* prioridad = extract_string_buffer(un_buffer);
    log_trace(logger_kernel, "BUFFER[%d]: [PATH:%s][SIZE:%s][PRIO:%s]", un_buffer->size, path, size, prioridad);
    buffer_destroy(un_buffer);

    int pid = asignar_pid();
    int size_num = atoi(size);

    // Avisar a Memoria [int pid][char* path][int size]
    
    // t_buffer* a_enviar = crear_buffer();
    // cargar_int_al_buffer(a_enviar, pid);
    // cargar_string_al_buffer(a_enviar, path);
    // cargar_int_al_buffer(a_enviar, size_num);
    // t_package* un_paquete = package_create(CREAR_PROCESO_KM, a_enviar);
    // enviar_paquete(un_paquete, fd_memoria);
    // destruir_paquete(un_paquete);

    // [FALTA] Hacer el resto de la lógica para el funcionamiento del KERNEL
}


int asignar_pid() {
    int valor_pid;
    pthread_mutex_lock(&mutex_pid);
        valor_pid = identificador_PID;
        identificador_PID++;
    pthread_mutex_unlock(&mutex_pid);
    return valor_pid;
}
