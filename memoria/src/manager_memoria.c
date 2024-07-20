#include "manager_memoria.h"

void retornar_siguiente_instruccion() 
{
    t_next_instruction *next = recv_next_instruction_cpu();

    if (next == NULL)
    {
        log_error(logger_memoria, "ERROR: Ha surgido un problema al recibir la solicitud de siguiente instruccion.");
    }

    t_proceso *proceso = obtener_proceso(obtener_pid_process(next));

    if (proceso == NULL)
    {
        log_error(logger_memoria, "ERROR: Ha surgido un problema al buscar el proceso en la memoria.");
    }
    else
    {
        t_instruction *instruction = obtener_siguiente_instruccion(proceso, obtener_pc_process(next));

        if (instruction != NULL)
        {
            send_instrution_cpu(instruction);
        }
    }
}

void crear_imagen_proceso()
{
    t_new_process *new_process = recv_new_process_kernel();

    if (new_process == NULL)
    {
        log_error(logger_memoria, "ERROR: Ha surgido un problema al recibir el nuevo proceso.");
    }

    if (add_process_to_memory(new_process))
    {
        log_info(logger_memoria, "Se ha creado correctamente en memoria el proceso con pid %d", new_process->pid);
    }
    else
    {
        log_warning(logger_memoria, "WARNING CREATE PROCESS: No se ha podido crear el proceso en memoria");
    }

    // TODO: Hacer funcion para eliminar un t_new_process
    destroy_new_process(new_process);
}

int process_message_cpu_handshake()
{
    t_buffer *buffer = recive_full_buffer(fd_cpu);
    uint32_t handshake = 1;

    recv_msg_cpu_memoria_handshake(buffer, &handshake);

    if (handshake != 0)
    {
        log_error(logger_memoria, "SE DEBE RECIBIR EL HANDSHAKE DE CPU ANTES QUE CUALQUIER MSJ");
        exit(EXIT_FAILURE);
    } else{
        handshake_cpu = true;
        send_msg_memoria_cpu_init(memoria_config->TAM_PAGINA, fd_cpu);
    }

    buffer_destroy(buffer);

    return EXIT_SUCCESS;
}

// MSG_CPU_MEMORIA_PAGE
int process_message_cpu_page()
{
    t_buffer *buffer = recive_full_buffer(fd_cpu);
    uint32_t pid = 0;
    uint32_t page_number = 0;
    recv_msg_cpu_memoria_page(buffer, &pid, &page_number);
    t_entrada_tabla_de_paginas *pagina = get_page_data(pid, page_number);
    send_msg_memoria_cpu(fd_cpu, pagina->frame);
    return 0;
}

// MSG_CPU_MEMORIA_DATA_READ
int process_message_cpu_data_read()
{
    t_buffer *buffer = recive_full_buffer(fd_cpu);
    uint32_t pid = 0;
    uint32_t page_number = 0;
    uint32_t frame = 0;
    uint32_t offset = 0;
    uint32_t size_value = 0;

    recv_msg_cpu_memoria_data_read(buffer, &pid, &frame, &offset, &size_value);
    // t_entrada_tabla_de_paginas *pagina = get_page_data(pid, page_number);
    // pagina->uso = 1;
    void *value = read_data(frame, offset, size_value);

    send_msg_memoria_cpu_data_read(value, size_value, fd_cpu);

    log_info(logger_memoria, "PID: %u - Accion: LEER <%u> BYTES - Direccion fisica: (Page: %u | Marco: %u | Desplazamiento: %u", pid, size_value, page_number, frame, offset);
    buffer_destroy(buffer);
    free(value);
    return 0;
}

// MSG_CPU_MEMORIA_DATA_WRITE
int process_message_cpu_data_write()
{
    t_buffer *buffer = recive_full_buffer(fd_cpu);
    uint32_t pid = 0;
    uint32_t page_number = 0;
    uint32_t frame = 0;
    uint32_t offset = 0;
    uint32_t value_size = 0;

    recv_msg_cpu_memoria_data_write(buffer, &pid, &page_number, &frame, &offset, &value_size);

    void *value = malloc(value_size);
    buffer_read_data(buffer, value, value_size);
    t_entrada_tabla_de_paginas *pagina = get_page_data(pid, page_number);
    pagina->uso = 1;
    pagina->modificado = 1;
    write_data(frame, offset, value, value_size);

    log_info(logger_memoria, "PID: %u - Accion: ESCRIBIR <%u> BYTES- Direccion fisica: (Page: %u | Marco: %u | Desplazamiento: %u",
             pid, value_size, page_number, frame, offset);
    buffer_destroy(buffer);
    free(value);
    return 0;
}

// RESIZE
int process_message_cpu_resize()
{
    t_buffer *buffer = recive_full_buffer(fd_cpu);
    uint32_t pid = 0;
    uint32_t new_size;

    recv_msg_cpu_memoria_resize(buffer, &pid, &new_size);
    uint8_t resize_response = resize(pid, new_size);
    send_msg_cpu_memoria_resize(resize_response, fd_cpu);

    log_info(logger_memoria, "PID: %u - Accion: RESIZE - Nuevo tamaño (<%u>)",
             pid, new_size);
    buffer_destroy(buffer);
    return 0;
}