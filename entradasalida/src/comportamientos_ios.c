#include "comportamientos_ios.h"

bool ejecutar_unidades_de_trabajo(t_io_generica* io_generica) 
{
    uint32_t tiempo_sleep = obtener_tiempo_sleep(io_generica);

    int tiempo_espera = tiempo_sleep * obtener_tiempo_unidad_trabajo(entradasalida_config);

    log_info(logger_entradasalida, "Esperando %d milisegundos", tiempo_espera);

    sleep(tiempo_espera);

    return true;
}


