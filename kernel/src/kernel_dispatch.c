#include "kernel_dispatch.h"
 
int fd_cpu_dispatch;

void atender_kernel_cpu_dispatch(){
        bool control_key = 1;
    while (control_key)
    {
        int cod_op = recibir_operacion(logger_kernel, fd_cpu_dispatch);
        t_package *package = package_create(NULL_HEADER);
        package_recv(package, fd_cpu_dispatch);

        switch (cod_op)
        {

        case MSG_CPU_KERNEL_DISPATCH:
            package_destroy(package);
            log_info(logger_kernel, "Se recibio un mje del CPU");
            break;
   
        case -1:
            log_error(logger_kernel, "el CPU se desconecto. Terminando servidor");
            control_key = 0;
            break;
        default:
            log_warning(logger_kernel, "Operacion desconocida en dispatch. No quieras meter la pata");
            break;
        }
    }
}