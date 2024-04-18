#include "utils.h"

uint32_t* convert_string_array_to_int_array(char** array_of_strings){
    uint16_t strings_array_length = arrayLength(array_of_strings);
    uint32_t* array_of_numbers = malloc(sizeof(uint32_t) * (strings_array_length + 1));
    
    for(uint16_t i = 0; array_of_strings[i] != NULL; i++) {
        array_of_numbers[i] = atoi(array_of_strings[i]);
    }
    array_of_numbers[strings_array_length] = 0;

    return array_of_numbers;
}

uint16_t arrayLength(char** array){
    uint16_t i = 0;
    while(array[i] != NULL){
        i++;
    }
    return i;
}

uint16_t intArrayLength(uint32_t* array){
    uint16_t i = 0;
    while(array[i] != 0){
        i++;
    }
    return i;
}