#include "cpu_dispatch.h"

void atender_cpu_kernel_dispatch()
{
    bool control_key = 1;
    while (control_key)
    {
        int cod_op = recibir_operacion(fd_kernel_dispatch);
        // log_info(logger_cpu, string_itoa(cod_op));

        // package_recv(package, fd_kernel_dispatch);
        t_buffer *new_buffer; // malloc(sizeof(t_buffer));

        switch (cod_op)
        {

        case MSG_KERNEL_CPU_DISPATCH:

            // new_buffer = recive_full_buffer(fd_kernel_dispatch);
            //  printf("%.*s\n", new_buffer->size, (char *)new_buffer->stream);
            //   char *content = extract_string_buffer(new_buffer); // error segmentation revisar

            // log_info(logger_cpu, ">>CONTENT:%s", content);
            // free(content);
            //  package_destroy(package);
            log_info(logger_cpu, "Se recibio un mje del KERNEL");
            free(new_buffer);

            break;
        case EXAMPLE:
            log_info(logger_cpu, "<<<<< EXAMPLE >>>>");
            t_message_example *new_msg = malloc(sizeof(t_message_example));
            new_buffer = recive_full_buffer(fd_kernel_dispatch);
            example_deserialize_msg(new_buffer, new_msg);
            log_info(logger_cpu, "MENSAJE => %s", new_msg->cadena);
            log_info(logger_cpu, "ENTERO => %d", new_msg->entero);
            free(new_msg->cadena);
            free(new_msg);
            buffer_destroy(new_buffer);

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
