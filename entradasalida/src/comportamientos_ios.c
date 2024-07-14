#include "comportamientos_ios.h"

//===============================================
// FUNCIONES DE IO GENERICA
//===============================================

bool ejecutar_unidades_de_trabajo(t_io_generica* io_generica) 
{
    uint32_t tiempo_sleep = obtener_tiempo_sleep(io_generica);
    int tiempo_espera = tiempo_sleep * obtener_tiempo_unidad_trabajo(entradasalida_config);

    log_info(logger_entradasalida, "PID: %d - Esperando %d milisegundos", 
             obtener_pid_generica(io_generica), tiempo_espera);

    usleep(tiempo_espera * 1000);  // usleep usa microsegundos

    return true;
}

//===============================================
// FUNCIONES DE IO STDIN
//===============================================

void escribir_memoria(uint32_t direccion_fisica, char* valor, uint32_t tamanio) {
    t_package* package = package_create(MSG_IO_STDIN_MEMORIA, sizeof(t_buffer));
    t_buffer* buffer = get_buffer(package);
    buffer_add_uint32(buffer, direccion_fisica);
    buffer_add_uint32(buffer, tamanio);
    buffer_add_string(buffer, valor);
    package_send(package, fd_memoria);
    package_destroy(package);
}

//===============================================
// FUNCIONES DE IO STDOUT
//===============================================

char* leer_memoria(uint32_t direccion_fisica, uint32_t tamanio) {
    t_package* package = package_create(MSG_IO_STDOUT_MEMORIA, sizeof(t_buffer));
    t_buffer* buffer = get_buffer(package);
    buffer_add_uint32(buffer, direccion_fisica);
    buffer_add_uint32(buffer, tamanio);
    package_send(package, fd_memoria);
    package_destroy(package);

    t_package* response = package_create(MSG_MEMORIA_IO_STDOUT, 0);
    package_recv(response, fd_memoria);
    char* valor = extract_string_buffer(get_buffer(response));
    package_destroy(response);
    return valor;
}

