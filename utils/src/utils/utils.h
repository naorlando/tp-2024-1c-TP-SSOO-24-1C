#ifndef SHARED_UTILS_H
#define SHARED_UTILS_H

// Standard Library
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


uint32_t* convert_string_array_to_int_array(char** array_of_strings);
uint16_t arrayLength(char** array);
uint16_t intArrayLength(uint32_t* array);
char* uint8_to_string(uint8_t);

#endif