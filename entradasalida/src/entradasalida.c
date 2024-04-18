#include <entradasalida.h>


int main(int argc, char *argv[]) {
    
    // CONEXIONES: nos vamos a conectar a Memoria y a Kernel.

    init();
    //Conexion como cliente a MEMORIA:

    conexion_io_memoria = crear_conexion(logger_entradasalida,"MEMORIA", IP_MEMORIA, PUERTO_MEMORIA);
    
    if (conexion_io_memoria != -1) {
        log_info(logger_entradasalida,"CONEXION establecida con memoria");
    } else {
        log_error(logger_entradasalida,"ERROR EN LA Conexion establecida con memoria");
    }

    // char *puerto_cpu_dispatch = string_itoa(kernel_config->PUERTO_CPU_DISPATCH);
    // char *cpu_client = "CPU";
    // int conexion = crear_conexion(logger_kernel, cpu_client, kernel_config->IP_CPU, puerto_cpu_dispatch);

    liberar_conexion(conexion_io_memoria);
    if (conexion_io_memoria == -1) {
        log_info(logger_entradasalida,"CONEXION LIBERADA con memoria");
    } else {
        log_error(logger_entradasalida,"ERROR EN LA LIBERACION de la conexion establecida con memoria");
    }

    return EXIT_SUCCESS;
}

// ENTRADA SALIDA_config.c :

// t_entradasalida_config *crear_entradasalida_config()
// {
//     t_entradasalida_config *ec = malloc(sizeof(t_entradasalida_config));

//     if (ec == NULL)
//     {
//         return NULL;
//     }

//     return ec;
// }

// void destruir(t_entradasalida_config *ec)
// {
//     free(ec);
// }

// bool cargar_entradasalida_config(t_entradasalida_config *ec, t_config *cf)
// {
//     ec->IP_MEMORIA = strdup(config_get_string_value(cf, "IP_MEMORIA"));
//     ec->PUERTO_MEMORIA = config_get_int_value(cf, "PUERTO_MEMORIA");
//     ec->IP_CPU = strdup(config_get_string_value(cf, "IP_CPU"));
//     ec->PUERTO_CPU_DISPATCH = config_get_int_value(cf, "PUERTO_CPU_DISPATCH");
//     ec->PUERTO_CPU_INTERRUPT = config_get_int_value(cf, "PUERTO_CPU_INTERRUPT");
//     ec->PUERTO_ESCUCHA = config_get_int_value(cf, "PUERTO_ESCUCHA");
//     ec->ALGORITMO_PLANIFICACION = strdup(config_get_string_value(cf, "ALGORITMO_PLANIFICACION"));
//     ec->GRADO_MAX_MULTIPROGRAMACION = config_get_int_value(cf, "GRADO_MAX_MULTIPROGRAMACION");
//     ec->DISPOSITIVOS_IO = config_get_array_value(cf, "DISPOSITIVOS_IO");
//     char **tiempos_io = config_get_array_value(cf, "TIEMPOS_IO");
//     // kc->TIEMPOS_IO = conversor_string_to_array(tiempos_io); crear un parser
//     string_array_destroy(tiempos_io);
//     ec->QUANTUM_RR = config_get_int_value(cf, "QUANTUM_RR");

//     return true;
// }
