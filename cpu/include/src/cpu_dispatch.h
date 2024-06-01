#ifndef CPU_DISPATCH_H_
#define CPU_DISPATCH_H_

#include "variables_globales.h"
#include "utils/instruccion.h"
#include "utils/estructuras.h"

void ejecutar_instruccion(t_instruction*, t_cpu_registers*);

#endif