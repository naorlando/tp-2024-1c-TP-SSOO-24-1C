#include "protocolo_memoria.h"

void requests_cpu() {
    bool esperar = true;

    while(esperar) {
        int cod_operacion = recibir_operacion(fd_cpu);

        switch(cod_operacion){
            case MENSAJE:
                // Se procesa el request
                recibir_mensaje(logger_memoria, fd_cpu);
            break;
            //TODO:
            /*
                Agregar operaciones a las que dara servicio el modulo
            */ 
            case ERROR:
                log_error(logger_memoria, "ERROR: Ha surgido un problema inesperado, se desconecto el modulo de memoria.");
                esperar = false; //Cortamos la espera de solicitudes
            break;
            default:
                log_warning(logger_memoria, "WARNING: El modulo de memoria ha recibido una solicitud con una operacion desconocida");
            break;
        }
    }
}

void requests_kernel() {
    bool esperar = true;

    while(esperar) {
        int cod_operacion = recibir_operacion(fd_kernel);

        switch(cod_operacion){
            case MENSAJE:
                // Se procesa el request
                recibir_mensaje(logger_memoria, fd_kernel);
            break;
            //TODO:
            /*
                Agregar operaciones a las que dara servicio el modulo
            */ 
            case ERROR:
                log_error(logger_memoria, "ERROR: Ha surgido un problema inesperado, se desconecto el modulo de memoria.");
                esperar = false; //Cortamos la espera de solicitudes
            break;
            default:
                log_warning(logger_memoria, "WARNING: El modulo de memoria ha recibido una solicitud con una operacion desconocida");
            break;
        }
    }
}

void requests_entradasalida() {
    bool esperar = true;

    while(esperar) {
        int cod_operacion = recibir_operacion(fd_entradasalida);

        switch(cod_operacion){
            case MENSAJE:
                // Se procesa el request
                recibir_mensaje(logger_memoria, fd_entradasalida);
            break;
            //TODO:
            /*
                Agregar operaciones a las que dara servicio el modulo
            */ 
            case ERROR:
                log_error(logger_memoria, "ERROR: Ha surgido un problema inesperado, se desconecto el modulo de memoria.");
                esperar = false; //Cortamos la espera de solicitudes
            break;
            default:
                log_warning(logger_memoria, "WARNING: El modulo de memoria ha recibido una solicitud con una operacion desconocida");
            break;
        }
    }
}

int recv_example_msg_cpu(){
    log_info(logger_memoria, "<<<<< EXAMPLE >>>>");
    t_package *package = package_create(NULL_HEADER);
    t_message_example * new_msg = malloc(sizeof(t_message_example));
    //package_recv(package, fd_kernel_dispatch);
    t_buffer* new_buffer = recive_full_buffer(fd_cpu);

    example_deserialize_msg(new_buffer, new_msg);
            
    log_info(logger_memoria, "%s", new_msg->cadena);
    log_info(logger_memoria, "%d", new_msg->entero);
    free(new_msg->cadena);
    free(new_msg);
    buffer_destroy(new_buffer);
    package_destroy(package);

    return 0;
}

int recv_example_msg_kernel(){
    log_info(logger_memoria, "<<<<< EXAMPLE >>>>");
    t_package *package = package_create(NULL_HEADER);
    t_message_example * new_msg = malloc(sizeof(t_message_example));
    //package_recv(package, fd_kernel_dispatch);
    t_buffer* new_buffer = recive_full_buffer(fd_kernel);

    example_deserialize_msg(new_buffer, new_msg);
            
    log_info(logger_memoria, "%s", new_msg->cadena);
    log_info(logger_memoria, "%d", new_msg->entero);
    free(new_msg->cadena);
    free(new_msg);
    buffer_destroy(new_buffer);
    package_destroy(package);

    return 0;
}

int recv_example_msg_entradasalida(){
    log_info(logger_memoria, "<<<<< EXAMPLE >>>>");
    t_package *package = package_create(NULL_HEADER);
    t_message_example * new_msg = malloc(sizeof(t_message_example));
    //package_recv(package, fd_kernel_dispatch);
    t_buffer* new_buffer = recive_full_buffer(fd_entradasalida);

    example_deserialize_msg(new_buffer, new_msg);
            
    log_info(logger_memoria, "%s", new_msg->cadena);
    log_info(logger_memoria, "%d", new_msg->entero);
    free(new_msg->cadena);
    free(new_msg);
    buffer_destroy(new_buffer);
    package_destroy(package);

    return 0;
}