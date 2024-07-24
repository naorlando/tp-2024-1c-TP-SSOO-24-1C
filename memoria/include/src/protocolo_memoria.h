#ifndef PROTOCOLO_MEMORIA_H_
#define PROTOCOLO_MEMORIA_H_

#include <commons/log.h>
#include "variables_globales.h"
#include "utils/protocolo.h"
#include "gestion_procesos.h" 

int recv_example_msg_cpu();

int recv_example_msg_kernel();

int recv_example_msg_entradasalida();

t_new_process* recv_new_process_kernel();

t_next_instruction* recv_next_instruction_cpu();

void send_instrution_cpu(t_instruction*);

// --   PAGE & FRAME   --
int send_msg_memoria_cpu(int fd, uint32_t frame);

// CPU -> MEMORIA :: MSG_CPU_MEMORIA_PAGE
int recv_msg_cpu_memoria_page(t_buffer* buffer, uint32_t* pid, uint32_t* page);
// MEMORIA -> CPU :: MSG_MEMORIA_CPU_FRAME
int recv_msg_memoria_cpu_frame(t_buffer* buffer, uint32_t* frame);

 
// MEMORIA -> GENERIC :: MSG_MEMORIA_GENERIC_DATA_READ
int send_msg_memoria_generic_data_read(void* value, uint32_t size_value ,int fd);


// --   READ DATA    --
// CPU -> MEMORIA :: MSG_CPU_MEMORIA_DATA_READ
int recv_msg_memoria_data_read(t_buffer* buffer, uint32_t* pid,  uint32_t* frame, uint32_t* offset, uint32_t* value_size);

int recv_msg_memoria_data_write(t_buffer* buffer, uint32_t* pid,  uint32_t* frame, uint32_t* offset, uint32_t* value_size);

// -- RESIZE --
int recv_msg_cpu_memoria_resize(t_buffer* buffer, uint32_t* pid, uint32_t* new_size);

int send_msg_cpu_memoria_resize(uint8_t resize_response,int fd);

int recv_msg_cpu_memoria_handshake(t_buffer *buffer, uint32_t *handshake);


//END PROCESS
void recv_msg_kernel_memoria_end_process(t_buffer *buffer, uint32_t *pid);

#endif /*PROTOCOLO_MEMORIA_H_*/