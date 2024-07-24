#include "request_memory.h"

void _data_read(uint32_t pid, t_datos_dir_logica *dir_logica, uint32_t frame, void *memory_value, uint32_t size_value)
{
    send_msg_memoria_data_read(pid, frame, dir_logica->desplazamiento_pagina, size_value, fd_memoria);

    int cod_op = recibir_operacion(fd_memoria);

    if (cod_op != MSG_MEMORIA_GENERIC_DATA_READ)
    {
        log_debug(logger_cpu, "Se espera recibir mensaje desde memoria de data read");
        exit(EXIT_FAILURE);
    }
    t_buffer *buffer = recive_full_buffer(fd_memoria);
    recv_msg_memoria_data(buffer, memory_value, size_value);
    buffer_destroy(buffer);
}
int read_from_memory(uint32_t pid, uint32_t logical_address, void *memory_value, uint32_t cantidad_paginas, uint32_t tamano)
{
    uint32_t frame;
    t_datos_dir_logica *dir_logica = crear_dir_logica(logical_address);
    uint32_t remaining_bytes = tamano;
    uint32_t bytes_read = 0;

    if (memory_value == NULL)
    {
        // Manejar el error de asignación de memoria
        free(dir_logica);
        return -1; // Error
    }

    while (remaining_bytes > 0)
    {
        // Obtener el marco correspondiente a la página actual
        if (!obtener_marco(pid, dir_logica->num_pagina, &frame))
        {
            // TLB MISS, obtener el marco desde la memoria
            send_msg_cpu_memoria_page(pid, dir_logica->num_pagina, fd_memoria);
            int cod_op = recibir_operacion(fd_memoria);
            t_buffer *buffer = recive_full_buffer(fd_memoria);

            if (cod_op == MSG_MEMORIA_CPU_FRAME)
            {
                recv_msg_memoria_cpu_frame(buffer, &frame);
                reemplazar(pid, dir_logica->num_pagina, frame); // Reemplazar en la TLB
                buffer_destroy(buffer);
            }
            else
            {
                log_debug(logger_cpu, "Se espera recibir mensaje desde memoria de page");
                free(dir_logica);
                return -1; // Error
            }
        }

        // Calcular bytes que se pueden leer en el frame actual
        uint32_t bytes_disponibles = (page_size - dir_logica->desplazamiento_pagina) + 1;
        uint32_t bytes_a_leer = (remaining_bytes < bytes_disponibles) ? remaining_bytes : bytes_disponibles;

        // Leer los bytes desde la memoria
        _data_read(pid, dir_logica, frame, (uint8_t *)memory_value + bytes_read, bytes_a_leer);

        // Actualizar contadores
        bytes_read += bytes_a_leer;
        remaining_bytes -= bytes_a_leer;

        // Mover al siguiente frame si es necesario
        if (remaining_bytes > 0)
        {
            dir_logica->num_pagina++;
            dir_logica->desplazamiento_pagina = 0;
        }
    }

    free(dir_logica);
    return 1; // Retorno que no fue page fault
}

// Ejemplo de la función _write_data que no necesita modificaciones
void _write_data(uint32_t pid, t_datos_dir_logica *dir_logica, uint32_t frame, void *write_value, uint32_t size_value)
{
    send_msg_memoria_data_write(pid, frame, dir_logica->desplazamiento_pagina, write_value, size_value, fd_memoria);
}

int write_into_memory(uint32_t pid, uint32_t logical_address, void *write_value, uint32_t cantidad_paginas, uint32_t tamano)
{
    uint32_t frame;
    t_datos_dir_logica *dir_logica = crear_dir_logica(logical_address);

    uint32_t remaining_bytes = tamano;
    uint32_t bytes_written = 0;
    uint32_t offset = 0;

    while (remaining_bytes > 0)
    {

        // Obtener el marco correspondiente a la página actual
        if (!obtener_marco(pid, dir_logica->num_pagina, &frame))
        {
            // TLB MISS, obtener el marco desde la memoria
            send_msg_cpu_memoria_page(pid, dir_logica->num_pagina, fd_memoria);
            int cod_op = recibir_operacion(fd_memoria);
            t_buffer *buffer = recive_full_buffer(fd_memoria);

            if (cod_op == MSG_MEMORIA_CPU_FRAME)
            {
                recv_msg_memoria_cpu_frame(buffer, &frame);
                reemplazar(pid, dir_logica->num_pagina, frame); // Reemplazar en la TLB
                buffer_destroy(buffer);
            }
            else
            {
                log_debug(logger_cpu, "Se espera recibir mensaje desde memoria de page");
                free(dir_logica);
                exit(EXIT_FAILURE);
            }
        }

        // Calcular bytes que se pueden escribir en el frame actual
        uint32_t bytes_disponibles = (page_size - dir_logica->desplazamiento_pagina) + 1;
        uint32_t bytes_a_escribir = (remaining_bytes < bytes_disponibles) ? remaining_bytes : bytes_disponibles;

        void *valor_asignable = malloc(bytes_a_escribir);

        memcpy(valor_asignable, write_value + offset, bytes_a_escribir);

        _write_data(pid, dir_logica, frame, valor_asignable, bytes_a_escribir);

        // Actualizar contadores
        bytes_written += bytes_a_escribir;
        remaining_bytes -= bytes_a_escribir;
        offset += bytes_a_escribir;

        // Mover al siguiente frame si es necesario
        if (remaining_bytes > 0)
        {
            dir_logica->num_pagina++;
            dir_logica->desplazamiento_pagina = 0;
        }
        free(valor_asignable);
    }

    free(dir_logica);
    return 1; // Retorno que no fue page fault
}

size_t obtener_tamano_registro(const char *nombre)
{
    if (strcmp(nombre, "AX") == 0 || strcmp(nombre, "BX") == 0 ||
        strcmp(nombre, "CX") == 0 || strcmp(nombre, "DX") == 0)
    {
        return sizeof(uint8_t); // Registros de 8 bits
    }
    else if (strcmp(nombre, "EAX") == 0 || strcmp(nombre, "EBX") == 0 ||
             strcmp(nombre, "ECX") == 0 || strcmp(nombre, "EDX") == 0 ||
             strcmp(nombre, "SI") == 0 || strcmp(nombre, "DI") == 0)
    {
        return sizeof(uint32_t); // Registros de 32 bits
    }
    else if (strcmp(nombre, "PC") == 0)
    {
        return sizeof(uint32_t); // Registros de 32 bits
    }
    return 0; // Registro no encontrado
}

void exec_mov_in(uint32_t direccion_logica, char *nombre, void *memory_value)
{

    uint32_t tamano = obtener_tamano_registro(nombre);
    uint32_t cantidad_paginas = traductor_cantidad_paginas(direccion_logica, tamano);

    // COMPLETAR FOR LEYENDO MEMORIA

    if (!read_from_memory(pcb_execute->pid, direccion_logica, memory_value, cantidad_paginas, tamano))
    {
        log_error(logger_cpu, "Error al leer la memoria en dirección lógica: %u\n", direccion_logica);
        return;
    }
}

void exec_mov_out(uint32_t direccion_logica, void *write_value, u_int32_t tamano)
{

    uint32_t cantidad_paginas = traductor_cantidad_paginas(direccion_logica, tamano);

    if (!write_into_memory(pcb_execute->pid, direccion_logica, write_value, cantidad_paginas, tamano))
    {
        log_error(logger_cpu, "Error al escribir la memoria en dirección lógica: %u\n", direccion_logica);
        return;
    }
}

uint8_t ajustar_tamano_proceso(uint32_t pid, uint32_t nuevo_tamano)
{
    send_msg_cpu_memoria_resize(pid, nuevo_tamano, fd_memoria);

    int cod_operacion = recibir_operacion(fd_memoria);

    if (cod_operacion != MSG_MEMORIA_CPU_RESIZE)
    {
        log_error(logger_cpu, "Error al recibir respuesta de RESIZE de memoria ");
    }
    t_buffer *buffer = recive_full_buffer(fd_memoria);
    uint8_t extracted_value = buffer_read_uint8(buffer);

    buffer_destroy(buffer);
    return extracted_value;
}

void copiar_cadena(uint32_t origen, uint32_t destino, int tamano)
{

    uint32_t cant_paginas_origen = traductor_cantidad_paginas(origen, tamano);
    uint32_t cant_paginas_destino = traductor_cantidad_paginas(destino, tamano);

    void *memory_value = malloc(tamano);
    if (read_from_memory(pcb_execute->pid, origen, &memory_value, cant_paginas_origen, tamano))
    {

        if (!write_into_memory(pcb_execute->pid, destino, memory_value, cant_paginas_destino, tamano))
        {
            log_error(logger_cpu, "Error COPY_STRING en pegar la información");
        }
    }
    else
    {
        log_error(logger_cpu, "Error COPY_STRING en leet la información");
    }
}

t_io_frames *exec_io_frames(uint32_t pid, uint32_t direccion_logica, uint32_t tamano)
{

    t_datos_dir_logica *dir_logica = crear_dir_logica(direccion_logica);
    uint32_t frame;
    uint32_t remaining_bytes = tamano;
    t_io_frames *io_frames = create_io_frames(pid, tamano);

    while (remaining_bytes > 0)
    {

        // Obtener el marco correspondiente a la página actual
        if (!obtener_marco(pid, dir_logica->num_pagina, &frame))
        {
            // TLB MISS, obtener el marco desde la memoria
            send_msg_cpu_memoria_page(pid, dir_logica->num_pagina, fd_memoria);
            int cod_op = recibir_operacion(fd_memoria);
            t_buffer *buffer = recive_full_buffer(fd_memoria);

            if (cod_op == MSG_MEMORIA_CPU_FRAME)
            {
                recv_msg_memoria_cpu_frame(buffer, &frame);
                reemplazar(pid, dir_logica->num_pagina, frame); // Reemplazar en la TLB
                buffer_destroy(buffer);
            }
            else
            {
                log_debug(logger_cpu, "Se espera recibir mensaje desde memoria de page");
                free(dir_logica);
                return NULL; // Error
            }
        }

        uint32_t bytes_disponibles = page_size - dir_logica->desplazamiento_pagina + 1;
        uint32_t bytes_a_escribir = (remaining_bytes < bytes_disponibles) ? remaining_bytes : bytes_disponibles;

        t_frame_data *frame_data = create_frame_data(frame, bytes_a_escribir, dir_logica->desplazamiento_pagina);

        add_frame_data(io_frames, frame_data);

        remaining_bytes -= bytes_a_escribir;

        if (remaining_bytes > 0)
        {
            dir_logica->num_pagina++;
            dir_logica->desplazamiento_pagina = 0;
        }
    }
    return io_frames;
}
