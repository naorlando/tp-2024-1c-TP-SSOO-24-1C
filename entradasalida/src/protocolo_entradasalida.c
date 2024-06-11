#include <protocolo_entrada.h>

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
