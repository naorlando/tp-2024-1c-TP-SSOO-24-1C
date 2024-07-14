#ifndef COMPORTAMIENTOS_IOS_H_
#define COMPORTAMIENTOS_IOS_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include "utils/estructuras.h"
#include "utils/solicitudes_io.h"
#include "variables_globales.h"
#include "utils/protocolo.h"
#include "utils/buffer.h"

//===============================================
// FUNCIONES DE IO GENERICA
//===============================================

// Ejecuta las unidades de trabajo para una IO genérica
// Pre: io_generica debe ser un puntero válido a una estructura t_io_generica
// Post: Retorna true si la ejecución fue exitosa, false en caso contrario
bool ejecutar_unidades_de_trabajo(t_io_generica* io_generica);

//===============================================
// FUNCIONES DE IO STDIN
//===============================================

// Escribe en memoria el valor recibido desde la entrada estándar
// Pre: direccion_fisica debe ser una dirección válida, valor debe ser un puntero válido, tamanio debe ser > 0
// Post: El valor se escribe en la memoria en la dirección especificada
void escribir_memoria(uint32_t direccion_fisica, char* valor, uint32_t tamanio);

//===============================================
// FUNCIONES DE IO STDOUT
//===============================================

// Lee de memoria un valor para ser mostrado en la salida estándar
// Pre: direccion_fisica debe ser una dirección válida, tamanio debe ser > 0
// Post: Retorna un puntero a una cadena con el valor leído de memoria
char* leer_memoria(uint32_t direccion_fisica, uint32_t tamanio);

#endif /* COMPORTAMIENTOS_IOS_H_ */