
#include "cronometro.h"

static t_temporal* cronometro = NULL;

void cronometro_iniciar(void) {
    if (cronometro == NULL) {
        cronometro = temporal_create();
    } else {
        cronometro->elapsed_ms = 0;
        temporal_resume(cronometro);
    }
}

void cronometro_detener(void) {
    if (cronometro != NULL) {
        temporal_stop(cronometro);
    }
}

void cronometro_reiniciar(void) {
    if (cronometro != NULL) {
        cronometro->elapsed_ms = 0;
    }
}

uint32_t cronometro_obtener_tiempo(void) {
    if (cronometro == NULL) {
        return 0;
    }
    return (uint32_t)temporal_gettime(cronometro);
}

// Función opcional para liberar el cronómetro global
void cronometro_destruir(void) {
    if (cronometro != NULL) {
        temporal_destroy(cronometro);
        cronometro = NULL;
    }
}