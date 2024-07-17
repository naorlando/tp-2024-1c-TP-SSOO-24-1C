#ifndef PROTOCOLO_MEMORIA_H_
#define PROTOCOLO_MEMORIA_H_

#include <commons/log.h>
#include <utils/sockets.h>
#include "variables_globales.h"
#include "utils/protocolo.h"
#include "utils/buffer.h"
#include "gestion_procesos.h" 

int recv_example_msg_cpu();

int recv_example_msg_kernel();

int recv_example_msg_entradasalida();

t_new_process* recv_process_kernel();

t_next_instruction* recv_next_instruction();

void send_instrution(t_instruction*);

// --   PAGE & FRAME   --
// CPU -> MEMORIA :: MSG_CPU_MEMORIA_PAGE
int recv_msg_cpu_memoria_page(t_buffer* buffer, uint32_t* pid, uint32_t* page_table_id, uint32_t* page);
// MEMORIA -> CPU :: MSG_MEMORIA_CPU_FRAME
int recv_msg_memoria_cpu_frame(t_buffer* buffer, uint32_t* frame);

 
// MEMORIA -> CPU :: MSG_MEMORIA_CPU_DATA_READ
int send_msg_memoria_cpu_data_read(uint32_t value, int fd);


// --   READ DATA    --
// CPU -> MEMORIA :: MSG_CPU_MEMORIA_DATA_READ
int recv_msg_cpu_memoria_data_read(t_buffer* buffer, uint32_t* pid, uint32_t* page, uint32_t* frame, uint32_t* offset);

int recv_msg_cpu_memoria_data_write(t_buffer* buffer, uint32_t* pid, uint32_t* page, uint32_t* frame, uint32_t* offset, uint32_t* value);

// -- RESIZE --
int recv_msg_cpu_memoria_resize(t_buffer* buffer, uint32_t* pid, uint32_t* new_size);

int send_msg_cpu_memoria_resize(uint8_t resize_response,int fd);


#endif /*PROTOCOLO_MEMORIA_H_*/