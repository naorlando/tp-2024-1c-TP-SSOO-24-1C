#include <protocolo_kernel.h>

// void *serializar_pcb(t_PCB *pcb, int *size) {
//     int total_size = sizeof(pcb->pid) + sizeof(pcb->program_counter) + sizeof(pcb->estado) + sizeof(pcb->registros);
//     *size = total_size;  // Set the size of the serialized data

//     void *buffer = malloc(total_size);
//     int offset = 0;

//     memcpy(buffer + offset, &(pcb->pid), sizeof(pcb->pid));
//     offset += sizeof(pcb->pid);

//     memcpy(buffer + offset, &(pcb->program_counter), sizeof(pcb->program_counter));
//     offset += sizeof(pcb->program_counter);

//     memcpy(buffer + offset, &(pcb->estado), sizeof(pcb->estado));
//     offset += sizeof(pcb->estado);

//     memcpy(buffer + offset, pcb->registros, sizeof(pcb->registros));
//     offset += sizeof(pcb->registros);

//     return buffer;
// }

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

int send_pcb_cpu()
{
    t_PCB *pcb = pcb_create(0, 1);
    u_int32_t buffer_size = sizeof(pcb);
    t_package *package = package_create(MSG_KERNEL_CPU_DISPATCH, buffer_size);

    serialize_pcb(package->buffer, pcb);
    package_send(package, fd_cpu_dispatch);

    // pcb_destroy(pcb); no eliminar hasta que termine de ejecutar
    package_destroy(package);

    return 0;
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

void atender_kernel_memoria()
{
    bool control_key = 1;
    while (control_key)
    {
        int cod_op = recibir_operacion(fd_kernel_memoria);

        switch (cod_op)
        {

        case MSG_MEMORIA_KERNEL:

            log_info(logger_kernel, "Se recibio un mje del memoria");
            break;

        case -1:
            log_error(logger_kernel, "la memoria se desconecto. Terminando servidor");
            control_key = 0;
            break;
        default:
            log_warning(logger_kernel, "Operacion desconocida en memoria. No quieras meter la pata");
            break;
        }
    }
}

void atender_kernel_IO()
{
    bool control_key = 1;
    while (control_key)
    {
        int cod_op = recibir_operacion(fd_kernel_IO);

        switch (cod_op)
        {
        case EXAMPLE:
            // Se procesa el request
            recv_example_msg_entradasalida();
            control_key = false; // Cortamos la espera de solicitudes
            break;
        // TODO:
        /*
            Agregar operaciones a las que dara servicio el modulo
        */
        case MSG_IO_KERNEL:

            log_info(logger_kernel, "Se recibio un mje de IO");
            break;

        case -1:
            log_error(logger_kernel, "la IO se desconecto. Terminando servidor");
            control_key = 0;
            break;
        default:
            log_warning(logger_kernel, "Operacion desconocida en IO. No quieras meter la pata");
            break;
        }
    }
}

void atender_kernel_cpu_dispatch()
{
    bool control_key = 1;
    while (control_key)
    {
        int cod_op = recibir_operacion(fd_cpu_dispatch);

        switch (cod_op)
        {

        case MSG_CPU_DISPATCH_KERNEL:

            log_info(logger_kernel, "Se recibio un mje de CPU DISPATCH");
            break;

        case -1:
            log_error(logger_kernel, "CPU DISPATCH se desconecto. Terminando servidor");
            control_key = 0;
            break;
        default:
            log_warning(logger_kernel, "Operacion desconocida en dispatch. No quieras meter la pata");
            break;
        }
    }
}