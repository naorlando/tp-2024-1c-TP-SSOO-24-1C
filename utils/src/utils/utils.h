#ifndef UTILS_H_
#define UTILS_H_


// Standard Library
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h> // Para las macros de formato como PRIu8, PRIu16, etc.
#include <limits.h> // Para los valores máximos como UINT8_MAX, UINT16_MAX, etc.

int get_max_length_uint(uint64_t );
uint32_t* convert_string_array_to_int_array(char** array_of_strings);
uint16_t arrayLength(char** array);
uint16_t intArrayLength(uint32_t* array);
char* uint8_to_string(uint8_t);
char* uint32_to_string(uint32_t);

#define GET_MAX_LENGTH(type) get_max_length_uint(type##_MAX)

#endif