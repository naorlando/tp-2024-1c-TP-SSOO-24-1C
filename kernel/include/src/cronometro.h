// cronometro.h
#ifndef CRONOMETRO_H
#define CRONOMETRO_H

#include "commons/temporal.h"
#include <stdlib.h>
#include <stdint.h>

void cronometro_iniciar(void);
void cronometro_detener(void);
void cronometro_reiniciar(void);
uint32_t cronometro_obtener_tiempo(void);
void cronometro_destruir(void);

#endif // CRONOMETRO_H