#ifndef UTILS_H_
#define UTILS_H_


// Standard Library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h> // Para las macros de formato como PRIu8, PRIu16, etc.
#include <limits.h> // Para los valores máximos como UINT8_MAX, UINT16_MAX, etc.

int get_max_length_uint(uint64_t );
uint32_t* convert_string_array_to_int_array(char** array_of_strings);
uint16_t arrayLength(char** array);
uint16_t intArrayLength(uint32_t* array);
char* uint8_to_string(uint8_t);
char* uint32_to_string(uint32_t);

char** split(const char*, const char*);
void array_string_destroy(char**);

// Concatena dos cadenas de texto con un separador en el medio.
// Pre: Los punteros a str1, str2 y separator deben ser válidos y no NULL.
// Post: Retorna un puntero a una nueva cadena que es la concatenación de str1, separator y str2. 
char* join(const char*, const char*, const char*);

// Duplica una cadena dada. 
// Pre: El puntero a la cadena `s` debe ser válido y no NULL.
// Post: Retorna un puntero a la nueva cadena asignada que es una duplicación de la que se pasa
// por parametro. Si la asignación de memoria falla, retorna NULL.
char* my_strdup(const char*);


// TODO: localizarlo en utils?
// Obtiene el valor de un registro dado un nombre.
// Pre: Los registros deben ser válidos y no NULL.
// Post: Retorna el valor del registro correspondiente al nombre dado.
void remove_newline(char *);

#define GET_MAX_LENGTH(type) get_max_length_uint(type##_MAX)

#endif