#include <kernel.h>

int main(int argc, char *argv[])
{

    init();

    // ############## Socket ###################

    char *server_port = string_itoa(kernel_config->PUERTO_ESCUCHA);
    int fd_server = iniciar_servidor(logger_kernel, NULL, NULL, server_port);

    if (fd_server != -1)
    {
        log_info(logger_kernel, "%s server listo escuchando en puerto %s", SERVERNAME, server_port);
    }
    else
    {
        log_error(logger_kernel, "Error al iniciar %s server en puerto %s", SERVERNAME, server_port);
    }

    char *puerto_cpu_dispatch = string_itoa(kernel_config->PUERTO_CPU_DISPATCH);
    char *cpu_client = "CPU";

    char *puerto_memoria = string_itoa(kernel_config->PUERTO_MEMORIA);
    char *memoria_client = "MEMORIA";

    int conexion = crear_conexion(logger_kernel, cpu_client, kernel_config->IP_CPU, puerto_cpu_dispatch);
    int conexion_memoria = crear_conexion(logger_kernel, memoria_client, kernel_config->IP_MEMORIA, puerto_memoria);

    // char *mensaje = "Este es un mensaje que lo envia el kernel!!!";
    // t_package *package = package_create(MSG_KERNEL_CPU_DISPATCH);

    // package->buffer = malloc(sizeof(t_buffer));
    // package->buffer->size = strlen(mensaje) + 1;
    // package->buffer->stream = malloc(package->buffer->size);
    // memcpy(package->buffer->stream, mensaje, package->buffer->size);
    // int bytes = package->buffer->size + 2 * sizeof(int);

    // serializar_paquete(package, bytes);

    // package_send(package, conexion);

    // package_destroy(package);

    // ejemplo serializado custom
    t_package *package_example = package_create(EXAMPLE);
    t_message_example *example = malloc(sizeof(t_message_example));
    char *cadena = "COMPRIMIDO EN BYTES";
    example->cadena = malloc(strlen(cadena) + 1);
    strcpy(example->cadena, cadena);
    example->entero = 5;

    example_serialize_msg(package_example->buffer, example);

    package_send(package_example, conexion);

    free(example->cadena);
    free(example);
    package_destroy(package_example);

    // ###########################

 

    pthread_t hilo_cpu_dispatch;
    pthread_create(&hilo_cpu_dispatch, NULL, (void *)atender_kernel_cpu_dispatch, NULL);
    // pthread_detach(hilo_kernel_dispatch);
    pthread_join(hilo_cpu_dispatch, NULL);
}
