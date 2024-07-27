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

int escribir_memoria_stdin(uint32_t pid, uint32_t direccion_logica, uint32_t tamanio, char* valor) {
    // t_package* package = package_create(MSG_IO_STDIN_MEMORIA, sizeof(t_buffer));
    // t_buffer* buffer = get_buffer(package);
    // buffer_add_uint32(buffer, pid);
    // buffer_add_uint32(buffer, direccion_logica);
    // buffer_add_uint32(buffer, tamanio);
    // buffer_add_string(buffer, valor);
    // int resultado = package_send(package, fd_memoria);
    // package_destroy(package);

    // // Esperar confirmación de Memoria
    // t_package* respuesta = package_create(0, 0);
    // package_recv(respuesta, fd_memoria);
    // int operacion_exitosa = (respuesta->msg_header == MSG_MEMORIA_IO_OK);
    // package_destroy(respuesta);

    // return (resultado > 0 && operacion_exitosa) ? 0 : -1;
    return 0;
}

//===============================================
// FUNCIONES DE IO STDOUT
//===============================================

char* leer_memoria_stdout(uint32_t pid, uint32_t direccion_logica, uint32_t tamanio) {
    // t_package* package = package_create(MSG_IO_STDOUT_MEMORIA, sizeof(t_buffer));
    // t_buffer* buffer = get_buffer(package);
    // buffer_add_uint32(buffer, pid);
    // buffer_add_uint32(buffer, direccion_logica);
    // buffer_add_uint32(buffer, tamanio);
    // package_send(package, fd_memoria);
    // package_destroy(package);

    // t_package* response = package_create(MSG_MEMORIA_IO_STDOUT, 0);
    // package_recv(response, fd_memoria);
    // char* valor = NULL;
    // if (response->msg_header == MSG_MEMORIA_IO_STDOUT) {
    //     valor = extract_string_buffer(get_buffer(response));
    // }
    // package_destroy(response);
    // return valor;
    return 0;
}

