#ifndef IO_FRAMES_LIB_H
#define IO_FRAMES_LIB_H

#include <stdint.h>
#include <commons/collections/list.h>
#include <stdio.h>
#include <stdlib.h>

// Definición de las estructuras
typedef struct
{
    uint32_t pid;
    uint32_t tamano_total;
    t_list *frames_data;
} t_io_frames;

typedef struct
{
    uint32_t frame;
    uint32_t tamano;
    uint32_t desplazamiento;
} t_frame_data;

// Funciones para t_frame_data
t_frame_data *create_frame_data(uint32_t frame, uint32_t tamano, uint32_t desplazamiento);
uint32_t get_frame(t_frame_data *frame_data);

uint32_t get_tamano(t_frame_data *frame_data);

uint32_t get_desplazamiento(t_frame_data *frame_data);

// Funciones para t_io_frames
t_io_frames *create_io_frames(uint32_t pid, uint32_t);
uint32_t get_pid_io_frames(t_io_frames *io_frames);
uint32_t get_tamano_total_io_frames(t_io_frames *io_frames);
void add_frame_data(t_io_frames *io_frames, t_frame_data *frame_data);
t_frame_data *get_frame_data(t_io_frames *io_frames, int index);
uint32_t get_bytes_frame_data();
uint32_t get_bytes_io_frames(t_io_frames*);
// Función para liberar memoria de t_io_frames
void destroy_io_frames(t_io_frames *io_frames);

#endif // IO_FRAMES_LIB_H