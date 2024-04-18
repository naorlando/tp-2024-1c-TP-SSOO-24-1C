#include "cpu_interrupt.h"


int fd_server_interrupt;
int fd_kernel_interrupt;

void atender_cpu_kernel_interrupt(t_log* logger)
{
    // while (1)
    // {
    //     int cod_op = recibir_operacion(logger,fd_kernel_interrupt);
    //     switch (cod_op)
    //     {
    //     case MENSAJE:
    //         // recibir_mensaje(cliente_fd);
    //         break;
    //     case PAQUETE:
    //         // lista = recibir_paquete(cliente_fd);
    //         // log_info(logger, "Me llegaron los siguientes valores:\n");
    //         // list_iterate(lista, (void *)iterator);
    //         break;
    //     case -1:
    //         log_error(logger, "el KERNEL se desconecto. Terminando servidor");
    //         return EXIT_FAILURE;
    //     default:
    //         log_warning(logger, "Operacion desconocida. No quieras meter la pata");
    //         break;
    //     }
    // }
}
