#include <protocolo_entrada.h>


int send_example_kernel()
{
    t_package *package_example = package_create(EXAMPLE);
    t_message_example *example = malloc(sizeof(t_message_example));
    char *cadena = "CPU ENVIO MENSAJE A KERNEL";
    example->cadena = malloc(strlen(cadena) + 1);
    strcpy(example->cadena, cadena);
    example->entero = 7;

    example_serialize_msg(package_example->buffer, example);

    package_send(package_example, fd_kernel);

    free(example->cadena);
    free(example);
    package_destroy(package_example);
    return 0;
}

int send_example_memoria()
{
    t_package *package_example = package_create(EXAMPLE);
    t_message_example *example = malloc(sizeof(t_message_example));
    char *cadena = "CPU ENVIO MENSAJE A MEMORIA";
    example->cadena = malloc(strlen(cadena) + 1);
    strcpy(example->cadena, cadena);
    example->entero = 9;

    example_serialize_msg(package_example->buffer, example);

    package_send(package_example, fd_memoria);

    free(example->cadena);
    free(example);
    package_destroy(package_example);
    return 0;
}