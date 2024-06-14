#include <protocolo_entrada.h>

//======================================================
//        FUNCIONES DE ENTRADA/SALIDA GENERICA
//======================================================

//Agrego la función para atender instrucción de interfaz genérica (IO_GEN_SLEEP)
void atender_instruccion_generica(int fd) {
    t_instruction *instruccion = recibir_instruccion(fd);
    if (instruccion != NULL) {
        if (instruccion->name == IO_GEN_SLEEP) {
            int unidades_trabajo = atoi(list_get(instruccion->params, 1));
            int tiempo_espera = unidades_trabajo * obtener_tiempo_unidad_trabajo(entradasalida_config);
            log_info(logger_entradasalida, "Esperando %d milisegundos", tiempo_espera);
            sleep(tiempo_espera);
            log_info(logger_entradasalida, "Operacion IO_GEN_SLEEP finalizada");
        } else {
            log_warning(logger_entradasalida, "Instrucción no soportada en dispositivo genérico");
        }
        enviar_confirmacion_io(fd);
        eliminar_instruccion(instruccion);
    } else {
        log_error(logger_entradasalida, "Error al recibir instrucción");
    }
}

/*Agrego la función para recibir una instrucción IO_GEN_SLEEP, que realiza la deserialización 
y devuelve el valor de las unidades de trabajo y el PID del proceso que solicita la instrucción*/
int recibir_instruccion(int fd, int *unidades_trabajo) {
    t_package *package = package_create(NULL_HEADER);
    if (package_recv(package, fd) != EXIT_SUCCESS) {
        log_error(logger_entradasalida, "Error al recibir instrucción");
        package_destroy(package);
        return -1;
    }

    t_buffer *buffer = package->buffer;
    void *stream = buffer->stream;

    // Deserializar el nombre de la instrucción
    uint32_t instruccion_id;
    memcpy(&instruccion_id, stream, sizeof(uint32_t));
    stream += sizeof(uint32_t);

    if (instruccion_id != IO_GEN_SLEEP) {
        log_error(logger_entradasalida, "Instrucción no soportada");
        package_destroy(package);
        return -1;
    }

    // Deserializar los parámetros
    uint32_t cant_params;
    memcpy(&cant_params, stream, sizeof(uint32_t));
    stream += sizeof(uint32_t);

    if (cant_params != 2) {
        log_error(logger_entradasalida, "Número incorrecto de parámetros");
        package_destroy(package);
        return -1;
    }

    // Deserializar los parámetros
    int pid;
    memcpy(&pid, stream, sizeof(int)); //Obtengo el PID del proceso
    stream += sizeof(int); //Stream, es un puntero a void, por lo que se debe incrementar en la cantidad de bytes que se desplaza

    memcpy(unidades_trabajo, stream, sizeof(int)); //Obtengo las unidades de trabajo

    package_destroy(package);
    return 0;
}


//Agrego la función para enviar una confirmación al Kernel después de procesar una instrucción
int enviar_confirmacion(int fd, t_msg_header header) {
    t_package *package = package_create(header);
    if (package_send(package, fd) != EXIT_SUCCESS) {
        log_error(logger_entradasalida, "Error al enviar confirmación");
        package_destroy(package);
        return -1;
    }
    package_destroy(package);
    return 0;
}

int recv_example_msg_kernel()
{
    log_info(logger_entradasalida, "<<<<< EXAMPLE RECIVE MESSAGE FROM KERNEL>>>>");

    t_message_example* new_msg = recv_example(fd_kernel);

    log_info(logger_entradasalida, "MENSAJE => %s", get_cadena(new_msg));
    log_info(logger_entradasalida, "ENTERO => %d", get_entero(new_msg));
    
    message_example_destroy(new_msg);

    return 0;
}

int send_example_kernel()
{
    char* cadena = "ENTRADASALIDA ENVIO MENSAJE A KERNEL";
    uint8_t entero = 7;

    return send_example(cadena, entero, fd_kernel);
}

int send_example_memoria()
{
    char* cadena = "ENTRADASALIDA ENVIO MENSAJE A MEMORIA";
    uint8_t entero = 9;

    return send_example(cadena, entero, fd_memoria);
}

//======================================================
//          FUNCIONES DE ENTRADA/SALIDA STDIN
//======================================================

// Agrego la función para atender instrucción de interfaz STDIN (IO_STDIN_READ), 
// que recibe un texto por consola y lo escribe en memoria
void atender_instruccion_stdin(int fd) {
    t_instruction *instruccion = recibir_instruccion(fd);
    if (instruccion != NULL) {
        if (instruccion->name == IO_STDIN_READ) {
            char *input = readline("Ingrese un texto: ");
            uint32_t direccion_fisica = (uint32_t)atoi(list_get(instruccion->params, 1));
            uint32_t tamanio = (uint32_t)atoi(list_get(instruccion->params, 2));
            escribir_memoria(direccion_fisica, input, tamanio);
            free(input);
        } else {
            log_warning(logger_entradasalida, "Instrucción no soportada en dispositivo STDIN");
        }
        enviar_confirmacion_io(fd);
        eliminar_instruccion(instruccion);
    } else {
        log_error(logger_entradasalida, "Error al recibir instrucción");
    }
}

// Función auxiliar para escribir en memoria un valor
void escribir_memoria(uint32_t direccion_fisica, char *valor, uint32_t tamanio) {
    t_package *package = package_create(MSG_WRITE_MEMORY);
    t_buffer *buffer = get_buffer(package);
    buffer_add_uint32(buffer, direccion_fisica);
    buffer_add_uint32(buffer, tamanio);
    buffer_add_string(buffer, valor);
    package_send(package, fd_memoria);
    package_destroy(package);
}

//======================================================
//          FUNCIONES DE ENTRADA/SALIDA STDOUT
//======================================================

// Agrego la función para atender instrucción de interfaz STDOUT (IO_STDOUT_WRITE)
void atender_instruccion_stdout(int fd) {
    t_instruction *instruccion = recibir_instruccion(fd);
    if (instruccion != NULL) {
        if (instruccion->name == IO_STDOUT_WRITE) {
            uint32_t direccion_fisica = (uint32_t)atoi(list_get(instruccion->params, 1));
            uint32_t tamanio = (uint32_t)atoi(list_get(instruccion->params, 2));
            char *valor = leer_memoria(direccion_fisica, tamanio);
            printf("%.*s", tamanio, valor);
            free(valor);
        } else {
            log_warning(logger_entradasalida, "Instrucción no soportada en dispositivo STDOUT");
        }
        enviar_confirmacion_io(fd);
        eliminar_instruccion(instruccion);
    } else {
        log_error(logger_entradasalida, "Error al recibir instrucción");
    }
}

// Función auxiliar para leer un valor enviado desde memoria
char *leer_memoria(uint32_t direccion_fisica, uint32_t tamanio) {
    t_package *package = package_create(MSG_READ_MEMORY);
    t_buffer *buffer = get_buffer(package);
    buffer_add_uint32(buffer, direccion_fisica);
    buffer_add_uint32(buffer, tamanio);
    package_send(package, fd_memoria);
    package_destroy(package);

    t_package *response = package_create(NULL_HEADER);
    package_recv(response, fd_memoria);
    char *valor = strdup(extract_string_buffer(get_buffer(response)));
    package_destroy(response);
    return valor;
}

//======================================================
//          FUNCIONES DE ENTRADA/SALIDA DIALFS
//======================================================

// A implementar!