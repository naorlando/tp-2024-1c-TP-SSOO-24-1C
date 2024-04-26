#include <protocolo_entrada.h>


int send_example_kernel()
{
    t_package *package_example = package_create(EXAMPLE);
    t_message_example *example = malloc(sizeof(t_message_example));
    char *cadena = "CPU ENVIO MENSAJE A KERNEL";
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
    char *cadena = "CPU ENVIO MENSAJE A MEMORIA";
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

void atender_io_memoria()
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

void atender_io_kernel()
{
    bool control_key = 1;
    while (control_key)
    {
        int cod_op = recibir_operacion(fd_kernel);

        switch (cod_op)
        {

        case MSG_KERNEL_IO:

            log_info(logger_entradasalida, "Se recibio un mje del KERNEL");
            break;

        case -1:
            log_error(logger_entradasalida, "el KERNEL se desconecto. Terminando servidor");
            control_key = 0;
            break;
        default:
            log_warning(logger_entradasalida, "Operacion desconocida en dispatch. No quieras meter la pata");
            break;
        }
    }
}