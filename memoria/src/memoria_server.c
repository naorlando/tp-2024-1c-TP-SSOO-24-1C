#include "memoria_server.h"

void requests_cpu() {
    bool esperar = true;

    while(esperar) {
        int cod_operacion = recibir_operacion(fd_cpu);

        switch(cod_operacion){
            case EXAMPLE:
                // Se procesa el request
                recv_example_msg_cpu();
                //esperar = false; //Cortamos la espera de solicitudes
            break;
            case MSG_NEXT_INSTRUCTION_CPU:
                t_next_instruction* next = recv_next_instruction();

                if(next == NULL) {
                    log_error(logger_memoria, "ERROR: Ha surgido un problema al recibir la solicitud de siguiente instruccion.");
                }

                t_proceso* proceso = obtener_proceso(obtener_pid_process(next));

                if(proceso == NULL) {
                    log_error(logger_memoria, "ERROR: Ha surgido un problema al buscar el proceso en la memoria.");
                }else{
                    t_instruction* instruction = obtener_siguiente_instruccion(proceso, obtener_pc_process(next));

                    if(instruction != NULL){
                        send_instrution(instruction);
                    }
                }
            break;                     
            case MSG_CPU_MEMORIA:

                log_info(logger_memoria, "Se recibio un mje del cpu");
            break;
            case -1:
                log_error(logger_memoria, "ERROR: Ha surgido un problema inesperado, se desconecto el modulo de memoria.");
                esperar = false; //Cortamos la espera de solicitudes
            break;
            default:
                log_warning(logger_memoria, "WARNING: El modulo de memoria ha recibido una solicitud con una operacion desconocida");
            break;
        }
    }
}

void requests_kernel() {
    bool esperar = true;

    while(esperar) {
        int cod_operacion = recibir_operacion(fd_kernel);

        switch(cod_operacion){
            case EXAMPLE:
                // Se procesa el request
                recv_example_msg_kernel();
                //esperar = false; //Cortamos la espera de solicitudes
            break;

            //TODO:
            /*
                Agregar operaciones a las que dara servicio el modulo
            */             
            case MSG_KERNEL_MEMORIA:

            case MSG_KERNEL_CREATE_PROCESS:

                t_new_process* new_process= recv_process_kernel();

                if(new_process == NULL) {
                    log_error(logger_memoria, "ERROR: Ha surgido un problema al recibir el nuevo proceso.");
                }
                
                if(add_process_to_memory(new_process)) {
                    log_info(logger_memoria, "Se ha creado correctamente en memoria el proceso con pid %d", new_process->pid);
                }else {
                    log_warning(logger_memoria, "WARNING CREATE PROCESS: No se ha podido crear el proceso en memoria");
                }

                //TODO: Hacer funcion para eliminar un t_new_process
            break;
            case -1:
                log_error(logger_memoria, "ERROR: Ha surgido un problema inesperado, se desconecto el modulo de memoria.");
                esperar = false; //Cortamos la espera de solicitudes
            break;
            default:
                log_warning(logger_memoria, "WARNING: El modulo de memoria ha recibido una solicitud con una operacion desconocida");
            break;
        }
    }
}

void requests_entradasalida() {
    bool esperar = true;

    while(esperar) {
        int cod_operacion = recibir_operacion(fd_entradasalida);

        switch(cod_operacion){
            case EXAMPLE:
                // Se procesa el request
                recv_example_msg_entradasalida();
                //esperar = false; //Cortamos la espera de solicitudes
            break;
            //TODO:
            /*
                Agregar operaciones a las que dara servicio el modulo
            */
            case MSG_IO_MEMORIA:

                log_info(logger_memoria, "Se recibio un mje del IO");
            break;
 
            case -1:
                log_error(logger_memoria, "ERROR: Ha surgido un problema inesperado, se desconecto el modulo de memoria.");
                esperar = false; //Cortamos la espera de solicitudes
            break;
            default:
                log_warning(logger_memoria, "WARNING: El modulo de memoria ha recibido una solicitud con una operacion desconocida");
            break;
        }
    }
}