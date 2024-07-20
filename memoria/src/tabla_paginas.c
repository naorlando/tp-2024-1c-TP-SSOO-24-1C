#include "tabla_paginas.h"

// extern memory_t * espacio_memoria ;

// t_dictionary *diccionario_tablas_de_paginas_por_proceso;
// pthread_mutex_t MUTEX_DICCIONARIO_TABLAS_PAGINAS;

// pthread_mutex_t MUTEX_BIT_ARRAY_MEMORY;
// bool is_structure_created = false;

uint8_t resize(uint32_t pid, uint32_t new_size)
{
    uint32_t new_page_count = calculate_page_count(new_size);

    // Verificar si el proceso existe
    if (!process_exists(pid))
    {
        // Verificar si hay suficientes frames disponibles antes de crear el proceso
        if (!has_sufficient_frames(new_page_count))
        {
            log_error(logger_memoria, "No hay suficientes marcos disponibles para crear el proceso PID: %d", pid);
            return 0;
        }
        create_process(pid, new_page_count);

        // Asignar marcos a las nuevas páginas
        for (uint32_t i = 0; i < new_page_count; i++)
        {
            agregar_pagina_a_memoria(pid, i);
        }
        return 1;
    }

    // Encontrar la tabla del PID
    pthread_mutex_lock(&MUTEX_DICCIONARIO_TABLAS_PAGINAS);
    t_pid_table *pid_table = _find_pid(pid, diccionario_tablas_de_paginas_por_proceso);
    pthread_mutex_unlock(&MUTEX_DICCIONARIO_TABLAS_PAGINAS);

    uint32_t old_page_count = list_size(pid_table->paginas);

    if (new_page_count > old_page_count)
    {
        // Verificar si hay suficientes frames disponibles antes de aumentar el tamaño
        uint32_t required_frames = new_page_count - old_page_count;
        if (!has_sufficient_frames(required_frames))
        {
            log_error(logger_memoria, "No hay suficientes marcos disponibles para redimensionar el proceso PID: %d", pid);
            return 0;
        }

        // Aumentar el tamaño
        for (uint32_t i = old_page_count; i < new_page_count; i++)
        {
            t_entrada_tabla_de_paginas *pagina = _create_default_page();
            list_add(pid_table->paginas, pagina);

            // Asignar un marco libre a la página
            agregar_pagina_a_memoria(pid, i);
        }
    }
    else if (new_page_count < old_page_count)
    {
        // Disminuir el tamaño
        for (uint32_t i = new_page_count; i < old_page_count; i++)
        {
            t_entrada_tabla_de_paginas *pagina = list_get(pid_table->paginas, i);
            if (pagina->presencia)
            {

                _set_bitarray_value_memory(pagina->frame, false);
            }
            free(pagina);
        }

        // Redimensionar la lista
        while (list_size(pid_table->paginas) > new_page_count)
        {
            list_remove(pid_table->paginas, list_size(pid_table->paginas) - 1);
        }
    }

    log_info(logger_memoria, "PID: %d - Tamaño cambiado a %d páginas", pid, new_page_count);
    return 1;
}

// Función para crear la estructura de tablas de páginas
void create_page_tables_structure()
{
    if (is_structure_created)
    {
        log_error(logger_memoria, "Se está tratando de crear dos veces la estructura de tablas de pags. Aborting...");
        exit(EXIT_FAILURE);
    }

    diccionario_tablas_de_paginas_por_proceso = dictionary_create();
    pthread_mutex_init(&MUTEX_DICCIONARIO_TABLAS_PAGINAS, NULL);
    _initialize_bit_array_memory();
    is_structure_created = true;
}

// Función para crear un proceso con una cantidad específica de páginas
void create_process(uint32_t pid, uint32_t num_paginas)
{
    if (!is_structure_created)
    {
        log_error(logger_memoria, "Se debe crear primero la lista de tablas y luego crear un proceso. Aborting...");
        exit(EXIT_FAILURE);
    }

    if (num_paginas > espacio_memoria->num_frames)
    {
        log_error(logger_memoria, "Número de páginas solicitado excede la memoria disponible. Aborting...");
        exit(EXIT_FAILURE);
    }

    pthread_mutex_lock(&MUTEX_DICCIONARIO_TABLAS_PAGINAS);
    t_pid_table *pid_table = malloc(sizeof(t_pid_table));
    pid_table->pid = pid;
    pid_table->paginas = list_create();

    for (uint32_t i = 0; i < num_paginas; i++)
    {
        t_entrada_tabla_de_paginas *pagina = _create_default_page();
        list_add(pid_table->paginas, pagina);
    }

    dictionary_put(diccionario_tablas_de_paginas_por_proceso, uint32_to_string(pid), pid_table);
    pthread_mutex_unlock(&MUTEX_DICCIONARIO_TABLAS_PAGINAS);
}

// Función para obtener los datos de una página dada un PID y un número de página
t_entrada_tabla_de_paginas *get_page_data(uint32_t pid, uint32_t page_number)
{
    pthread_mutex_lock(&MUTEX_DICCIONARIO_TABLAS_PAGINAS);
    t_pid_table *pid_table = _find_pid(pid, diccionario_tablas_de_paginas_por_proceso);
    pthread_mutex_unlock(&MUTEX_DICCIONARIO_TABLAS_PAGINAS);

    if (pid_table == NULL)
    {
        log_error(logger_memoria, "Proceso con PID: %d no encontrado. Aborting...", pid);
        exit(EXIT_FAILURE);
    }

    return list_get(pid_table->paginas, page_number);
}

// Función para obtener el marco de una página dada un PID y un número de página
bool get_page_frame(uint32_t pid, uint32_t page_number, uint32_t *frame_response)
{
    t_entrada_tabla_de_paginas *pagina = get_page_data(pid, page_number);
    if (!pagina->presencia)
    {
        return false;
    }
    else
    {
        *frame_response = pagina->frame;
        return true;
    }
}

// Función para agregar una página a la memoria
void agregar_pagina_a_memoria(uint32_t pid, uint32_t page_number)
{
    t_entrada_tabla_de_paginas *pagina_a_agregar = get_page_data(pid, page_number);
    int32_t frame_disponible = _frame_disponible();

    if (frame_disponible != -1)
    {

        _set_bitarray_value_memory(frame_disponible, true);

        pagina_a_agregar->presencia = true;
        pagina_a_agregar->uso = true;
        pagina_a_agregar->modificado = false;
        pagina_a_agregar->frame = frame_disponible;

        log_info(logger_memoria, "PID: %d - Página: %d - Marco: %d agregado a memoria", pid, page_number, frame_disponible);
    }
    else
    {
        log_error(logger_memoria, "No hay marcos disponibles en memoria para agregar la página");
    }
}

// Función para finalizar un proceso dado su PID
void finalizar_proceso(uint32_t pid)
{
    pthread_mutex_lock(&MUTEX_DICCIONARIO_TABLAS_PAGINAS);
    t_pid_table *proceso_obtenido = dictionary_remove(diccionario_tablas_de_paginas_por_proceso, (char *)&pid);
    pthread_mutex_unlock(&MUTEX_DICCIONARIO_TABLAS_PAGINAS);

    if (proceso_obtenido == NULL)
    {
        log_error(logger_memoria, "Proceso con PID: %d no encontrado. Aborting...", pid);
        return;
    }

    uint32_t cant_paginas_proceso = list_size(proceso_obtenido->paginas);
    for (uint32_t i = 0; i < cant_paginas_proceso; i++)
    {
        t_entrada_tabla_de_paginas *pagina = list_get(proceso_obtenido->paginas, i);
        if (pagina->presencia)
        {
            _set_bitarray_value_memory(pagina->frame, false);
        }
        free(pagina);
    }

    list_destroy(proceso_obtenido->paginas);
    free(proceso_obtenido);

    log_info(logger_memoria, "Proceso con PID: %d finalizado y memoria liberada", pid);
}

// Función interna para encontrar una tabla de páginas por PID
t_pid_table *_find_pid(uint32_t pid, t_dictionary *diccionario)
{
    return dictionary_get(diccionario, uint32_to_string(pid));
}

// Función interna para obtener el valor del bitarray en una posición dada
bool _get_bitarray_value_memory(uint32_t position)
{
    pthread_mutex_lock(&MUTEX_BIT_ARRAY_MEMORY);
    bool res = bitarray_marcos_de_memoria[position];
    pthread_mutex_unlock(&MUTEX_BIT_ARRAY_MEMORY);
    return res;
}

// Función interna para establecer el valor del bitarray en una posición dada
void _set_bitarray_value_memory(uint32_t position, bool value)
{
    pthread_mutex_lock(&MUTEX_BIT_ARRAY_MEMORY);
    bitarray_marcos_de_memoria[position] = value;
    pthread_mutex_unlock(&MUTEX_BIT_ARRAY_MEMORY);
}

// Función interna para encontrar un marco disponible en memoria
int32_t _frame_disponible()
{
    for (int i = 0; i < espacio_memoria->num_frames; i++)
    {
        if (!_get_bitarray_value_memory(i))
            return i;
    }
    return -1;
}

// Otras funciones internas...

t_entrada_tabla_de_paginas *_create_default_page()
{
    t_entrada_tabla_de_paginas *pagina = malloc(sizeof(t_entrada_tabla_de_paginas));
    if (pagina == NULL)
    {
        log_error(logger_memoria, "Error al asignar memoria para la página.");
        exit(EXIT_FAILURE); // Opcional: puedes manejar esto de otra manera si prefieres no terminar el programa
    }
    pagina->frame = UINT32_MAX; // Valor por defecto para indicar que aún no se ha asignado un frame
    pagina->modificado = false;
    pagina->presencia = false;
    pagina->uso = true;
    return pagina;
}

bool *_initialize_bit_array_memory()
{
    pthread_mutex_init(&MUTEX_BIT_ARRAY_MEMORY, NULL);
    uint32_t cantidad_marcos = espacio_memoria->num_frames;
    bitarray_marcos_de_memoria = malloc(cantidad_marcos * sizeof(bool));
    if (bitarray_marcos_de_memoria == NULL)
    {
        log_error(logger_memoria, "Error al asignar memoria para el bitarray de marcos.");
        exit(EXIT_FAILURE); // Opcional: puedes manejar esto de otra manera si prefieres no terminar el programa
    }
    for (uint32_t i = 0; i < cantidad_marcos; i++)
    {
        bitarray_marcos_de_memoria[i] = false;
    }
    return bitarray_marcos_de_memoria;
}

bool process_exists(uint32_t pid)
{
    pthread_mutex_lock(&MUTEX_DICCIONARIO_TABLAS_PAGINAS);
    t_pid_table *pid_table = _find_pid(pid, diccionario_tablas_de_paginas_por_proceso);
    pthread_mutex_unlock(&MUTEX_DICCIONARIO_TABLAS_PAGINAS);
    return pid_table != NULL;
}
bool has_sufficient_frames(uint32_t required_frames)
{
    uint32_t available_frames = 0;
    for (uint32_t i = 0; i < espacio_memoria->num_frames; i++)
    {
        if (!bitarray_marcos_de_memoria[i])
        {
            available_frames++;
            if (available_frames >= required_frames)
            {
                return true;
            }
        }
    }
    return false;
}

uint32_t calculate_page_count(uint32_t size)
{
    return (size + espacio_memoria->page_size - 1) / espacio_memoria->page_size;
}