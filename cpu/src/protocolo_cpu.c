#include "protocolo_cpu.h"
int send_example_memoria()
{
    t_package *package_example = package_create(EXAMPLE);
    t_message_example *example = malloc(sizeof(t_message_example));
    char *cadena = "CPU ENVIO MENSAJE A MEMORIA";
    example->cadena = malloc(strlen(cadena) + 1);
    strcpy(example->cadena, cadena);
    example->entero = 5;

    example_serialize_msg(package_example->buffer, example);

    package_send(package_example, fd_memoria);

    free(example->cadena);
    free(example);
    package_destroy(package_example);
    return 0;
}