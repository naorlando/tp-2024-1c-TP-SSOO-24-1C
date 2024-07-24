#ifndef TABLA_PAGINAS_H
#define TABLA_PAGINAS_H



// Standard library
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

// Commons
#include <commons/log.h>
#include <commons/collections/dictionary.h>
#include <commons/collections/list.h>

// Libraries
#include <semaphore.h>
#include <pthread.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>


// Project
#include "utils/utils.h"
#include "variables_globales.h"





typedef struct {
    uint32_t frame;        // Número de marco en la memoria física
    bool presencia;       // Indica si la página está en memoria
} t_entrada_tabla_de_paginas;

// Estructura para la tabla de páginas de un proceso
typedef struct {
    uint32_t pid;          // Identificador del proceso
    t_list *paginas;      // Lista de entradas de la tabla de páginas
} t_pid_table;

// Estructura para manejar el espacio de memoria


// Variables Globales

extern t_dictionary *diccionario_tablas_de_paginas_por_proceso;
extern pthread_mutex_t MUTEX_DICCIONARIO_TABLAS_PAGINAS;
extern bool *bitarray_marcos_de_memoria;
extern pthread_mutex_t MUTEX_BIT_ARRAY_MEMORY;
extern bool is_structure_created;

// Función para crear la estructura de tablas de páginas
void create_page_tables_structure();

// Función para crear un proceso con una cantidad específica de páginas
void create_process(uint32_t pid, uint32_t num_paginas);

// Función para obtener los datos de una página dada un PID y un número de página
t_entrada_tabla_de_paginas *get_page_data(uint32_t pid, uint32_t page_number);

// Función para obtener el marco de una página dada un PID y un número de página
bool get_page_frame(uint32_t pid, uint32_t page_number, uint32_t *frame_response);

// Función para agregar una página a la memoria
void agregar_pagina_a_memoria(uint32_t pid, uint32_t page_number);

// Función para finalizar un proceso dado su PID
void finalizar_proceso(uint32_t pid);

// Función para cambiar el tamaño de la memoria de un proceso
uint8_t resize(uint32_t pid, uint32_t new_size); 

// Funciones Internas
t_pid_table *_find_pid(uint32_t pid, t_dictionary *diccionario);
t_entrada_tabla_de_paginas *_create_default_page();
void _set_bitarray_value_memory(uint32_t position, bool value);
bool* _initialize_bit_array_memory();
int32_t _frame_disponible();
bool process_exists(uint32_t pid);
bool has_sufficient_frames(uint32_t required_frames);
uint32_t calculate_page_count(uint32_t size);

#endif /* TABLA_PAGINAS_H_ */