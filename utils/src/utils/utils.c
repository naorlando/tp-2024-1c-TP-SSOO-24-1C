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

char* uint8_to_string(uint8_t value) {
    // Un valor uint8_t puede ser de 0 a 255, así que el máximo número de dígitos es 3
    // Añadimos 1 para el terminador nulo '\0'
    int length_uint = GET_MAX_LENGTH(UINT8)+1;
    char* str = malloc(length_uint * sizeof(char));

    if (str == NULL) {
        return NULL;
    }

    // Convierte el valor uint8_t a una cadena de texto
    snprintf(str, length_uint, "%u", value);

    return str;
}


int get_max_length_uint(uint64_t max_value) {
    int length = 0;
    while (max_value > 0) {
        length++;
        max_value /= 10;
    }
    return length;
}

char* uint32_to_string(uint32_t value) {
    // Un valor uint8_t puede ser de 0 a 255, así que el máximo número de dígitos es 3
    // Añadimos 1 para el terminador nulo '\0'
    int length_uint = GET_MAX_LENGTH(UINT32)+1;
    char* str = malloc(length_uint * sizeof(char));

    if (str == NULL) {
        return NULL;
    }

    // Convierte el valor uint8_t a una cadena de texto
    snprintf(str, length_uint, "%u", value);

    return str;
}

char** split(const char* str, const char* delimiter) {
    if (str == NULL || delimiter == NULL) return NULL;

    size_t count = 0;
    char* temp_str = strdup(str);
    char* token = strtok(temp_str, delimiter);

    while (token != NULL) {
        count++;
        token = strtok(NULL, delimiter);
    }

    free(temp_str);

    char** result = malloc((count + 1) * sizeof(char*));
    if (result == NULL) return NULL;

    temp_str = strdup(str);
    token = strtok(temp_str, delimiter);
    size_t idx = 0;
    
    while (token != NULL) {
        result[idx++] = strdup(token);
        token = strtok(NULL, delimiter);
    }
    result[idx] = NULL;

    free(temp_str);
    return result;
}

void array_string_destroy(char** array) {
    if (array == NULL) return;

    for (size_t i = 0; array[i] != NULL; i++) {
        free(array[i]);
    }
    free(array);
}

char* join(const char* str1, const char* str2, const char* separator) {
    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);
    size_t len_sep = strlen(separator);

    // Calcular el tamaño total de la nueva cadena
    size_t total_len = len1 + len2 + len_sep + 1;

    char* result = (char*)malloc(total_len * sizeof(char));

    if (result == NULL) {
        return NULL;
    }

    // Copiar la primera cadena al resultado
    strcpy(result, str1);

    // Añadir el separador al resultado
    strcat(result, separator);

    // Añadir la segunda cadena al resultado
    strcat(result, str2);

    return result;
}

char* my_strdup(const char* s) {
    // Calcula la longitud de la cadena original incluyendo el carácter nulo
    size_t len = strlen(s) + 1;

    // Asigna memoria para la nueva cadena
    char *dup = (char *)malloc(len);

    // Verifica si la asignación de memoria fue exitosa
    if (dup == NULL) {
        return NULL;
    }

    // Copia la cadena original a la nueva ubicación de memoria
    memcpy(dup, s, len);

    // Asegúrate de que la nueva cadena termine con un carácter nulo
    dup[len - 1] = '\0';

    // Devuelve el puntero a la nueva cadena
    return dup;
}

void remove_newline(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

char* lista_a_string(t_list* pid_list) {
    if (list_is_empty(pid_list)) {
        return strdup("[]");
    }

    char* pid_string = strdup("[");
    for (int i = 0; i < list_size(pid_list); i++) {
        uint32_t* pid = list_get(pid_list, i);
        char* pid_char = malloc(12); // Espacio suficiente para un entero de 32 bits y caracteres adicionales
        sprintf(pid_char, "%d", *pid);
        pid_string = realloc(pid_string, strlen(pid_string) + strlen(pid_char) + 3); // +3 para la coma, espacio y el corchete final
        strcat(pid_string, pid_char);
        if (i < list_size(pid_list) - 1) {
            strcat(pid_string, ", ");
        }
        free(pid_char);
    }
    strcat(pid_string, "]");

    return pid_string;
}