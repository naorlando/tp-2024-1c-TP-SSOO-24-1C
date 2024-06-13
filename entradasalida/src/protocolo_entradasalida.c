#include <protocolo_entrada.h>

//Agrego la función para atender la instrucción de IO_GEN_SLEEP
//REVISAR!!!
void atender_instruccion_sleep() {
    int unidades_trabajo;
    if (recibir_instruccion(fd_kernel, &unidades_trabajo) == 0) {
        int tiempo_espera = unidades_trabajo * TIEMPO_UNIDAD_TRABAJO;
        log_info(logger_entradasalida, "Esperando %d milisegundos", tiempo_espera);
        sleep(tiempo_espera);
        log_info(logger_entradasalida, "Operacion IO_GEN_SLEEP finalizada");
        enviar_confirmacion(fd_kernel, MSG_KERNEL_IO);
    } else {
        log_error(logger_entradasalida, "Error al recibir la instruccion IO_GEN_SLEEP");
    }
}

//Agrego la función para recibir una instrucción, que realiza el proceso de deserialización
/* La función recibir_instruccion() deserializa el mensaje recibido del kernel y crea una estructura 
t_instruction que contiene el nombre de la instrucción y sus parámetros. Esta estructura es pasada 
por referencia a atender_instruccion_sleep().  Podría servir en el futuro para recibir más variedad de 
instrucciones y parámetros.  Por el momento no la uso, y en su lugar, usaré otra función más acorde 
a la instrucción IO_GEN_SLEEP*/
/*
int recibir_instruccion(int fd, t_instruction **instruccion) {
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
    (*instruccion)->name = (t_name_instruct)instruccion_id;

    // Deserializar los parámetros
    uint32_t cant_params;
    memcpy(&cant_params, stream, sizeof(uint32_t));
    stream += sizeof(uint32_t);
    (*instruccion)->params = list_create();
    for (uint32_t i = 0; i < cant_params; i++) {
        uint32_t param_size;
        memcpy(&param_size, stream, sizeof(uint32_t));
        stream += sizeof(uint32_t);
        void *param = malloc(param_size);
        memcpy(param, stream, param_size);
        stream += param_size;
        list_add((*instruccion)->params, param);
    }

    package_destroy(package);
    return 0;
}
*/

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
    memcpy(&pid, stream, sizeof(int));
    stream += sizeof(int);

    memcpy(unidades_trabajo, stream, sizeof(int));

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

// Comento las funciones de ejemplo, ya que no se utilizan en la Interfaz Genérica

// Agrego la función para atender la instrucción de sleep
void atender_instruccion_sleep()
{
    // int unidades_trabajo;
    // if (recibir_instruccion(fd_kernel, &unidades_trabajo) == 0) {
    //     int tiempo_espera = unidades_trabajo * TIEMPO_UNIDAD_TRABAJO;
    //     log_info(logger_entradasalida, "Esperando %d milisegundos", tiempo_espera);
    //     sleep(tiempo_espera);
    //     log_info(logger_entradasalida, "Operacion IO_GEN_SLEEP finalizada");
    //     enviar_confirmacion(fd_kernel, MSG_KERNEL_IO);
    // } else {
    //     log_error(logger_entradasalida, "Error al recibir la instruccion IO_GEN_SLEEP");
    // }
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

