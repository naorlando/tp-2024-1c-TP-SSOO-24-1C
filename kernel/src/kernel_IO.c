#include "kernel_IO.h"
 

void atender_kernel_IO(){
        bool control_key = 1;
    while (control_key)
    {
        int cod_op = recibir_operacion(fd_kernel_IO);
        t_package *package = package_create(NULL_HEADER);
        package_recv(package, fd_kernel_IO);

        switch (cod_op)
        {

        case MSG_IO_KERNEL:
            package_destroy(package);
            log_info(logger_kernel, "Se recibio un mje del IO");
            break;
   
        case -1:
            log_error(logger_kernel, "el IO se desconecto. Terminando servidor");
            control_key = 0;
            break;
        default:
            log_warning(logger_kernel, "Operacion desconocida en dispatch. No quieras meter la pata");
            break;
        }
    }
}