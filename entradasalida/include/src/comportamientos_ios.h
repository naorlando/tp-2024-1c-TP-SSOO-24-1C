#ifndef COMPORTAMIENTOS_IOS_H_
#define COMPORTAMIENTOS_IOS_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#include "variables_globales.h"
#include <readline/readline.h>
#include "utils/solicitudes_io.h"
#include "utils/estructuras.h"
#include "utils/protocolo.h"

bool ejecutar_unidades_de_trabajo(t_io_generica *);
char *leer_entrada_limitada(uint32_t);
void escribir_memoria(t_io_frames *, char *);
char *leer_memoria(t_io_frames *);

#endif /* COMPORTAMIENTOS_IOS_H_ */