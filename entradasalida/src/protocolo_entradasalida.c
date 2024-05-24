#include <protocolo_entrada.h>

// Función principal que atiende las solicitudes del Kernel
void requests_kernel() {
    bool esperar = true;
    while(esperar) {
        int cod_operacion = recibir_operacion(fd_kernel);

        switch(cod_operacion){
            //TODO:
            /*
                Agregar operaciones a las que dara servicio el modulo
            */
            //case EXAMPLE:
            //    // Se procesa el request
            //    recv_example_msg_kernel();
            //    esperar = false; //Cortamos la espera de solicitudes
            //break;
            case MSG_KERNEL_IO:
                log_info(logger_entradasalida, "Se recibio un mje del KERNEL");
            break;
            //Agrego la operacion para el caso de que el kernel solicite un sleep
            case MSG_IO_KERNEL_GEN_SLEEP:
                log_info(logger_entradasalida, "Se recibio una instruccion IO_GEN_SLEEP");
                atender_instruccion_sleep();
            break;
            case -1:
                log_error(logger_entradasalida, "ERROR: Ha surgido un problema inesperado, se desconecto el modulo de entradaSalida.");
                esperar = false; //Cortamos la espera de solicitudes
            break;
            default:
                log_warning(logger_entradasalida, "WARNING: El modulo de entradaSalida ha recibido una solicitud con una operacion desconocida");
            break;
        }
    }
}

//Comento la lógica de la función requests_memoria, ya que no se utiliza
/*void requests_memoria()
{
    bool control_key = 1;
    while (control_key)
    {
        int cod_op = recibir_operacion(fd_memoria);

        switch (cod_op)
        {

        case -1:
            log_error(logger_entradasalida, "la memoria se desconecto. Terminando servidor");
            control_key = 0;
            break;
        default:
            log_warning(logger_entradasalida, "Operacion desconocida en dispatch. No quieras meter la pata");
            break;
        }
    }
}*/

//Agrego la función para atender la instrucción de IO_GEN_SLEEP
void atender_instruccion_sleep() {
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

// Función para recibir una instrucción desde el Kernel
t_instruccion* recibir_instruccion(int socket_kernel) {
    t_instruccion* instruccion = malloc(sizeof(t_instruccion));
    
    // Recibir el tipo de instrucción
    if (recv(socket_kernel, &instruccion->tipo_instruccion, sizeof(int), 0) <= 0) {
        log_error(logger_entradasalida, "Error al recibir tipo de instrucción");
        free(instruccion);
        return NULL;
    }
    
    // Recibir el número de unidades de trabajo
    if (recv(socket_kernel, &instruccion->unidades_trabajo, sizeof(int), 0) <= 0) {
        log_error(logger_entradasalida, "Error al recibir unidades de trabajo");
        free(instruccion);
        return NULL;
    }

    log_info(logger_entradasalida, "Instrucción recibida: tipo=%d, unidades de trabajo=%d", instruccion->tipo_instruccion, instruccion->unidades_trabajo);
    
    return instruccion;
}

// Función para enviar una confirmación al Kernel
void enviar_confirmacion(int socket_kernel, int resultado) {
    // Enviar el resultado de la confirmación
    if (send(socket_kernel, &resultado, sizeof(int), 0) <= 0) {
        log_error(logger_entradasalida, "Error al enviar confirmación al Kernel");
    } else {
        log_info(logger_entradasalida, "Confirmación enviada al Kernel: resultado=%d", resultado);
    }
}

// Comento las funciones de ejemplo, ya que no se utilizan en la Interfaz Genérica
/*
int recv_example_msg_kernel(){
    log_info(logger_entradasalida, "<<<<< EXAMPLE RECIVE MESSAGE FROM KERNEL>>>>");
    t_message_example * new_msg = malloc(sizeof(t_message_example));
    t_buffer* new_buffer = recive_full_buffer(fd_kernel);
    example_deserialize_msg(new_buffer, new_msg);
    log_info(logger_entradasalida, "%s", new_msg->cadena);
    log_info(logger_entradasalida, "%d", new_msg->entero);
    free(new_msg->cadena);
    free(new_msg);
    buffer_destroy(new_buffer);
    return 0;
}

int send_example_kernel()
{
    t_package *package_example = package_create(EXAMPLE);
    t_message_example *example = malloc(sizeof(t_message_example));
    char *cadena = "ENTRADASALIDA ENVIO MENSAJE A KERNEL";
    example->cadena = malloc(strlen(cadena) + 1);
    strcpy(example->cadena, cadena);
    example->entero = 7;
    example_serialize_msg(package_example->buffer, example);
    package_send(package_example, fd_kernel);
    free(example->cadena);
    free(example);
    package_destroy(package_example);
    return 0;
}

int send_example_memoria()
{
    t_package *package_example = package_create(EXAMPLE);
    t_message_example *example = malloc(sizeof(t_message_example));
    char *cadena = "ENTRADASALIDA ENVIO MENSAJE A MEMORIA";
    example->cadena = malloc(strlen(cadena) + 1);
    strcpy(example->cadena, cadena);
    example->entero = 9;
    example_serialize_msg(package_example->buffer, example);
    package_send(package_example, fd_memoria);
    free(example->cadena);
    free(example);
    package_destroy(package_example);
    return 0;
}
*/
