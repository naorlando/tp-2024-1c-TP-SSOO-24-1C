#include <protocolo_entrada.h>

void requests_kernel()
{
    bool esperar = true;

    while (esperar)
    {
        int cod_operacion = recibir_operacion(fd_kernel);

        switch (cod_operacion)
        {
        case EXAMPLE:
            // Se procesa el request
            recv_example_msg_kernel();
            esperar = false; // Cortamos la espera de solicitudes
            break;
        // TODO:
        /*
            Agregar operaciones a las que dara servicio el modulo
        */
        case MSG_KERNEL_IO:

            log_info(logger_entradasalida, "Se recibio un mje del KERNEL");
            break;
        // Agrego la operacion para el caso de que el kernel solicite un sleep
        case MSG_IO_GEN_SLEEP:
            log_info(logger_entradasalida, "Se recibio una instruccion MSG_IO_GEN_SLEEP");
            atender_instruccion_sleep();
            break;
        case -1:
            log_error(logger_entradasalida, "ERROR: Ha surgido un problema inesperado, se desconecto el modulo de entradaSalida.");
            esperar = false; // Cortamos la espera de solicitudes
            break;
        default:
            log_warning(logger_entradasalida, "WARNING: El modulo de entradaSalida ha recibido una solicitud con una operacion desconocida");
            break;
        }
    }
}

// Comento la lógica de la función requests_memoria, ya que no se utiliza
void requests_memoria()
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
}

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
