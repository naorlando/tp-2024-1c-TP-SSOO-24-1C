#include "request_memory.h"

void _data_read(uint32_t pid, t_datos_dir_logica *dir_logica, uint32_t frame, uint32_t *memory_value)
{
    send_msg_cpu_memoria_data_read(pid, dir_logica->num_pagina, frame, dir_logica->desplazamiento_pagina, fd_memoria);

    t_package *paquete = package_create(NULL_HEADER, sizeof(uint32_t));
    package_recv(paquete, fd_memoria);

    if (paquete->msg_header != MSG_MEMORIA_CPU_DATA_READ)
    {
        log_debug(logger_cpu, "Se espera recibir mensaje desde memoria de data read");
        exit(EXIT_FAILURE);
    }

    recv_msg_memoria_cpu_data(paquete->buffer, memory_value);
    package_destroy(paquete);
}

int read_from_memory(uint32_t pid, uint32_t logical_address, uint32_t *memory_value, uint32_t cantidad_paginas)
{
    uint32_t frame;
    t_datos_dir_logica *dir_logica = crear_dir_logica(logical_address);
    uint32_t offset = 0;
    // Inicialmente, el buffer es NULL
    void *buffer = NULL;
    size_t buffer_size = 0;
    uint32_t aux_value = 0;

    for (uint32_t i = 0; i < cantidad_paginas; ++i)
    {
        if (obtener_marco(pid, dir_logica->num_pagina, &frame))
        {
            // TLB HIT
            _data_read(pid, dir_logica, frame, &aux_value);
            buffer_size = buffer_size + sizeof(aux_value);
            buffer = realloc(buffer, buffer_size);
            memcpy((uint32_t *)buffer + offset, &aux_value, sizeof(aux_value));
            offset = offset + sizeof(aux_value);
        }
        else
        {
            // TLB MISS
            send_msg_cpu_memoria_page(pid, dir_logica->num_pagina, fd_memoria);

            t_package *paquete = package_create(NULL_HEADER, sizeof(uint32_t));
            package_recv(paquete, fd_memoria);

            if (paquete->msg_header == MSG_MEMORIA_CPU_FRAME)
            {
                recv_msg_memoria_cpu_frame(paquete->buffer, &frame);
                reemplazar(pid, dir_logica->num_pagina, frame); // Reemplazamos en la TLB
                _data_read(pid, dir_logica, frame, &aux_value);
                buffer_size = buffer_size + sizeof(aux_value);
                buffer = realloc(buffer, buffer_size);
                memcpy((uint32_t *)buffer + offset, &aux_value, sizeof(aux_value));
                offset = offset + sizeof(aux_value);
                package_destroy(paquete);
            }
            else
            {
                log_debug(logger_cpu, "Se espera recibir mensaje desde memoria de page");
                exit(EXIT_FAILURE);
            }
        }

        // Actualizar el número de página y el desplazamiento después de cada iteración
        if (i < cantidad_paginas - 1) // Verificar si no es la última iteración
        {
            dir_logica->num_pagina += 1;
            dir_logica->desplazamiento_pagina = 0;
        }

        // Copiar el contenido del buffer auxiliar a memory_value
        memcpy(memory_value, buffer, buffer_size);

        free(buffer);
        free(dir_logica);
    }

    free(dir_logica);
    // Retorno que no fue page fault
    return 1;
}

// Ejemplo de la función _write_data que no necesita modificaciones
void _write_data(uint32_t pid, t_datos_dir_logica *dir_logica, uint32_t frame, uint32_t write_value)
{
    send_msg_cpu_memoria_data_write(pid, dir_logica->num_pagina, frame, dir_logica->desplazamiento_pagina, write_value, fd_memoria);
}

int write_into_memory(uint32_t pid, uint32_t logical_address, uint32_t write_value, uint32_t cantidad_paginas)
{
    uint32_t frame;
    t_datos_dir_logica *dir_logica = crear_dir_logica(logical_address);
    uint32_t remaining_bytes = sizeof(write_value);
    uint32_t bytes_written = 0;
    uint8_t *write_ptr = (uint8_t *)&write_value;

    for (uint32_t i = 0; i < cantidad_paginas; ++i)
    {
        uint32_t bytes_to_write = (i == 0) ? (page_size - dir_logica->desplazamiento_pagina) : page_size;
        if (bytes_to_write > remaining_bytes)
        {
            bytes_to_write = remaining_bytes;
        }

        uint32_t partial_value = 0;
        memcpy(&partial_value, write_ptr + bytes_written, bytes_to_write);

        if (obtener_marco(pid, dir_logica->num_pagina, &frame))
        {
            // TLB HIT
            _write_data(pid, dir_logica, frame, partial_value);
        }
        else
        {
            // TLB MISS
            send_msg_cpu_memoria_page(pid, dir_logica->num_pagina, fd_memoria);

            t_package *paquete = package_create(NULL_HEADER, sizeof(uint32_t));
            package_recv(paquete, fd_memoria);

            if (paquete->msg_header == MSG_MEMORIA_CPU_FRAME)
            {
                recv_msg_memoria_cpu_frame(paquete->buffer, &frame);
                reemplazar(pid, dir_logica->num_pagina, frame); // Reemplazamos en la TLB
                _write_data(pid, dir_logica, frame, partial_value);
                package_destroy(paquete);
            }
            else
            {
                log_debug(logger_cpu, "Se espera recibir mensaje desde memoria de page");
                free(dir_logica);
                exit(EXIT_FAILURE);
            }
        }

        bytes_written += bytes_to_write;
        remaining_bytes -= bytes_to_write;

        if (remaining_bytes == 0)
        {
            break;
        }

        // Actualizar el número de página y el desplazamiento después de cada iteración
        if (i < cantidad_paginas - 1) // Verificar si no es la última iteración
        {
            dir_logica->num_pagina += 1;
            dir_logica->desplazamiento_pagina = 0;
        }
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

void exec_mov_in(uint32_t direccion_logica, char *nombre, uint32_t *memory_value)
{

    uint32_t cantidad_paginas = traductor_cantidad_paginas(direccion_logica, obtener_tamano_registro(nombre));

    // COMPLETAR FOR LEYENDO MEMORIA

    if (read_from_memory(pcb_execute->pid, direccion_logica, memory_value, cantidad_paginas))
    {
        _establecer_registro(cpu_registers, nombre, *memory_value);
    }
    else
    {
        log_error(logger_cpu, "Error al leer la memoria en dirección lógica: %u\n", direccion_logica);
        return;
    }
}

void exec_mov_out(uint32_t direccion_logica, uint32_t write_value, u_int32_t tamano)
{

    uint32_t cantidad_paginas = traductor_cantidad_paginas(direccion_logica, tamano);

    // COMPLETAR FOR LEYENDO MEMORIA

    if (!write_into_memory(pcb_execute->pid, direccion_logica, write_value, cantidad_paginas))
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

    return extracted_value;
}

void copiar_cadena(uint32_t origen, uint32_t destino, int tamano)
{

   
    uint32_t cant_paginas_origen = traductor_cantidad_paginas(origen, tamano);
    uint32_t cant_paginas_destino = traductor_cantidad_paginas(destino, tamano);

    uint32_t memory_value = 0;
    if (read_from_memory(pcb_execute->pid, origen, &memory_value, cant_paginas_origen))
    {

        if (!write_into_memory(pcb_execute->pid, destino, memory_value, cant_paginas_destino))
        {
            log_error(logger_cpu, "Error COPY_STRING en pegar la información");
        }
    }
    else
    {
        log_error(logger_cpu, "Error COPY_STRING en leet la información");
    }
}


void exec_io_stdin_read(uint32_t interfaz ,uint32_t direccion_logica , uint32_t tamano ){

     
     
    //  if (obtener_marco(pid, dir_logica->num_pagina, &frame))
    //     {
    //         //TLB HIT
          
    //     }
    //     else
    //     {
    //         // TLB MISS
    //         send_msg_cpu_memoria_page(pid, dir_logica->num_pagina, fd_memoria);

    //         t_package *paquete = package_create(NULL_HEADER, sizeof(uint32_t));
    //         package_recv(paquete, fd_memoria);



}