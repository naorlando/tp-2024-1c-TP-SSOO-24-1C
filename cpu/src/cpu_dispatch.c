#include "cpu_dispatch.h"

int fd_server_dispatch;
int fd_kernel_dispatch;

void atender_cpu_kernel_dispatch()
{
    bool control_key = 1;
    while (control_key)
    {
        int cod_op = recibir_operacion(logger_cpu, fd_kernel_dispatch);
        // log_info(logger_cpu, string_itoa(cod_op));
        // t_package *package = package_create(NULL_HEADER);
        // package_recv(package, fd_kernel_dispatch);
        t_buffer * new_buffer;
        switch (cod_op)
        {

        case MSG_KERNEL_CPU_DISPATCH:
            
            new_buffer = recive_full_buffer(fd_kernel_dispatch);
            printf("%.*s\n", new_buffer->size, (char *)new_buffer->stream);
            //char * lcontent = extract_string_buffer(new_buffer); // error segmentation revisar
            
            //log_info(logger_cpu,content);
            // package_destroy(package);
            log_info(logger_cpu, "Se recibio un mje del KERNEL");
            break;
        case -1:
            log_error(logger_cpu, "el KERNEL se desconecto. Terminando servidor");
            control_key = 0;
            break;
        default:
            log_warning(logger_cpu, "Operacion desconocida en dispatch. No quieras meter la pata");
            break;
        }
    }
}
