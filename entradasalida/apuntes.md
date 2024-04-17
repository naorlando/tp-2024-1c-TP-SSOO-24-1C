# NOTAS tincho:
    -   cada modulo va a crear su propio logger.
    -   cada modulo va a crear su propia conexion de cliente si la necesita.

## logs:
- van cambiando de color:
log_info()
log_warning()
log_debug()
log_trace()

## CONSEJOS:
- evitar referencia circular.
- para ordenar y hacer prolijo el codeo: funciones STATIC: no visibles por el IDE, sirve para no sobre referenciar las cosas. En el archivo las STATIC van arriba de las funciones "normales". Y SE BORRAN LAS REFERENCIAS DEL .h .
podemos lograr el mismo efecto empezando las funciones con un _ , esto trae la ventaja de poder ordernar el archivo como tengamos

# codigo borrado:

## general: 

void init()
{
    _iniciar_logger();
    _iniciar_config();
    // imprimir_config();
}

void _iniciar_logger()
{
    // LOGGER OFICIAL:
    logger_entradasalida = log_create("entradasalida.log", "ENTRADASALIDA_LOG", 1, LOG_LEVEL_INFO);

    if (logger_entradasalida == NULL)
    {
        perror("No se pudo crear el archivo log para el modulo de entrada/salida");
        // exit(EXIT_FAILURE);
    }

    // LOGGER DEBUG:
    logger_entradasalida_debug = log_create("entradasalida.log", "ENTRADASALIDA_LOG", 1, LOG_LEVEL_TRACE);

    if (logger_entradasalida_debug == NULL)
    {
        perror("No se pudo crear el archivo log de debug para el modulo de entrada/salida");
        // exit(EXIT_FAILURE);
    }
}

void _iniciar_config()
{
    config_entradasalida = config_create("/home/utnso/tp-2024-1c-TP-SSOO-24-1C/entradasalida/cfg/entradasalida.config");

    if (config_entradasalida == NULL)
    {
        perror("No se pudo crear el archivo de configuracion para el modulo de entrada/salida");
        // exit(EXIT_FAILURE);
    }
    // ------------------------- para que? --------------------------------- //

    // Creo la estructura para guardar los datos del archivo de configuracion
    // entradasalida_config = crear_entradasalida_config();

    // if (entradasalida_config == NULL)
    // {
    //     log_error(logger_entradasalida, "Error al crear la estructura t_entradasalida_config");
    // }

    // cargar_entradasalida_config(entradasalida_config, config_entradasalida);

    // ------------------------- para que? ---------------------------------//
}

/*
    void imprimir_config() {
        log_trace(logger_entradasalida_debug, "PUERTO DE ESCUCHA DE LA ENTRADASALIDA: %d", entradasalida_config->PUERTO_ESCUCHA);
    }
 */

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
