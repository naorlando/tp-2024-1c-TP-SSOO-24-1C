#include <protocolo_kernel.h>

int send_example_cpu()
{
    char *cadena = "KERNEL ENVIO MENSAJE A CPU";
    uint8_t size_cadena = strlen(cadena) + 1; // Include null terminator
    uint32_t buffer_size = sizeof(uint8_t) * 2 + size_cadena;
    t_package *package_example = package_create(EXAMPLE, buffer_size);
    t_message_example *example = malloc(sizeof(t_message_example));

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

void send_pcb_cpu(t_PCB* pcb)
{
    send_pcb(MSG_PCB_CPU, fd_cpu_dispatch, pcb);
}

int send_example_memoria()
{
    char *cadena = "KERNEL ENVIO MENSAJE A MEMORIA";
    uint8_t size_cadena = strlen(cadena) + 1; // Include null terminator
    uint32_t buffer_size = sizeof(uint8_t) * 2 + size_cadena;
    t_package *package_example = package_create(EXAMPLE, buffer_size);
    t_message_example *example = malloc(sizeof(t_message_example));

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

int recv_example_msg_entradasalida()
{
    log_info(logger_kernel, "<<<<< EXAMPLE RECIVE MESSAGE FROM ENTRADASALIDA >>>>");
    t_message_example *new_msg = malloc(sizeof(t_message_example));
    t_buffer *new_buffer = recive_full_buffer(fd_kernel_IO);

    example_deserialize_msg(new_buffer, new_msg);

    log_info(logger_kernel, "%s", new_msg->cadena);
    log_info(logger_kernel, "%d", new_msg->entero);
    free(new_msg->cadena);
    free(new_msg);
    buffer_destroy(new_buffer);

    return 0;
}