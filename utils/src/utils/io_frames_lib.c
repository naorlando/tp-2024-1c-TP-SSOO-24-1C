#include "io_frames_lib.h"

// Funciones para t_frame_data
t_frame_data *create_frame_data(uint32_t frame, uint32_t tamano, uint32_t desplazamiento)
{
    t_frame_data *new_frame_data = (t_frame_data *)malloc(sizeof(t_frame_data));
    if (new_frame_data == NULL)
    {
        return NULL;
    }
    new_frame_data->frame = frame;
    new_frame_data->tamano = tamano;
    new_frame_data->desplazamiento = desplazamiento;
    return new_frame_data;
}

uint32_t get_frame(t_frame_data *frame_data)
{
    return frame_data->frame;
}

uint32_t get_tamano(t_frame_data *frame_data)
{
    return frame_data->tamano;
}

uint32_t get_desplazamiento(t_frame_data *frame_data)
{
    return frame_data->desplazamiento;
}


// Funciones para t_io_frames
t_io_frames *create_io_frames(uint32_t pid, uint32_t tamano_total)
{
    t_io_frames *new_io_frames = (t_io_frames *)malloc(sizeof(t_io_frames));
    if (new_io_frames == NULL)
    {
        return NULL;
    }
    new_io_frames->pid = pid;
    new_io_frames->tamano_total = tamano_total;
    new_io_frames->frames_data = list_create();
    return new_io_frames;
}

uint32_t get_pid_io_frames(t_io_frames *io_frames)
{
    return io_frames->pid;
}

uint32_t get_tamano_total_io_frames(t_io_frames *io_frames)
{
    return io_frames->tamano_total;
}

void add_frame_data(t_io_frames *io_frames, t_frame_data *frame_data)
{
    list_add(io_frames->frames_data, frame_data);
}

t_frame_data *get_frame_data(t_io_frames *io_frames, int index)
{
    return (t_frame_data *)list_get(io_frames->frames_data, index);
}

uint32_t get_bytes_io_frames(t_io_frames* io_frames) {
    uint32_t total_size = sizeof(io_frames->pid) + sizeof(io_frames->tamano_total)+sizeof(uint32_t);
    
    int count_frames= list_size(io_frames->frames_data);

    total_size += (count_frames * get_bytes_frame_data());
    
    return total_size;
}

uint32_t get_bytes_frame_data()
{
    return sizeof(uint32_t)*3;
}

// Función para liberar memoria de t_io_frames
void destroy_io_frames(t_io_frames *io_frames)
{
    list_destroy_and_destroy_elements(io_frames->frames_data, free);
    free(io_frames);
}