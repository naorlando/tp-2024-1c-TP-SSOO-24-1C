#include <protocolo_kernel.h>

int send_example_cpu()
{
    t_package *package_example = package_create(EXAMPLE);
    t_message_example *example = malloc(sizeof(t_message_example));
    char *cadena = "KERNEL ENVIO MENSAJE A CPU";
    example->cadena = malloc(strlen(cadena) + 1);
    strcpy(example->cadena, cadena);
    example->entero = 5;

    example_serialize_msg(package_example->buffer, example);

    package_send(package_example, fd_cpu_dispatch);

    free(example->cadena);
    free(example);
    package_destroy(package_example);
    return 0;
}

int send_example_memoria()
{
    t_package *package_example = package_create(EXAMPLE);
    t_message_example *example = malloc(sizeof(t_message_example));
    char *cadena = "KERNEL ENVIO MENSAJE A MEMORIA";
    example->cadena = malloc(strlen(cadena) + 1);
    strcpy(example->cadena, cadena);
    example->entero = 5;

    example_serialize_msg(package_example->buffer, example);

    package_send(package_example, fd_kernel_memoria);

    free(example->cadena);
    free(example);
    package_destroy(package_example);
    return 0;
}

void requests_entradasalida() {
    bool esperar = true;

    while(esperar) {
        int cod_operacion = recibir_operacion(fd_kernel_IO);

        switch(cod_operacion){
            case EXAMPLE:
                // Se procesa el request
                recv_example_msg_entradasalida();
                esperar = false; //Cortamos la espera de solicitudes
            break;
            //TODO:
            /*
                Agregar operaciones a las que dara servicio el modulo
            */ 
            case -1:
                log_error(logger_kernel, "ERROR: Ha surgido un problema inesperado, se desconecto el modulo de memoria.");
                esperar = false; //Cortamos la espera de solicitudes
            break;
            default:
                log_warning(logger_kernel, "WARNING: El modulo de memoria ha recibido una solicitud con una operacion desconocida");
            break;
        }
    }
}

int recv_example_msg_entradasalida(){
    log_info(logger_kernel, "<<<<< EXAMPLE RECIVE MESSAGE FROM ENTRADASALIDA >>>>");
    t_message_example * new_msg = malloc(sizeof(t_message_example));
    t_buffer* new_buffer = recive_full_buffer(fd_kernel_IO);

    example_deserialize_msg(new_buffer, new_msg);
            
    log_info(logger_kernel, "%s", new_msg->cadena);
    log_info(logger_kernel, "%d", new_msg->entero);
    free(new_msg->cadena);
    free(new_msg);
    buffer_destroy(new_buffer);

    return 0;
}