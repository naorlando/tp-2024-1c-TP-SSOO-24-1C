#include "comportamientos_ios.h"

bool ejecutar_unidades_de_trabajo(t_io_generica *io_generica)
{
    uint32_t tiempo_sleep = obtener_tiempo_sleep(io_generica);

    int tiempo_espera = tiempo_sleep * obtener_tiempo_unidad_trabajo(entradasalida_config);

    log_info(logger_entradasalida, "Esperando %d milisegundos", tiempo_espera);

    sleep(tiempo_espera / 1000);

    return true;
}

char *leer_entrada_limitada(uint32_t tamanio_limite)
{
    char *input = NULL;
    size_t len = 0;

    do
    {
        if (input != NULL)
        {
            free(input);
        }

        input = readline("Ingrese un texto: ");
        if (input == NULL)
        {
            log_error(logger_entradasalida, "Error al leer la entrada\n");
            return NULL;
        }

        len = strlen(input);
        if (len > tamanio_limite - 1)
        {
            log_error(logger_entradasalida, "La entrada excede el tamaño máximo permitido (%u caracteres). Inténtelo de nuevo.\n", tamanio_limite - 1);
        }
    } while (len > tamanio_limite - 1);

    input[tamanio_limite -1] = "\0";

    return input;
}

// Función auxiliar que escribe en memoria el valor recibido
void escribir_memoria(t_io_frames *io_frames, char *entrada)
{
    t_list *frames = io_frames->frames_data;
    uint32_t size_lista = list_size(frames);
    size_t offset = 0;

    for (size_t i = 0; i < size_lista; i++)
    {
        t_frame_data *frame_actual = get_frame_data(io_frames, i);
        uint32_t tamano_frame = get_tamano(frame_actual);

        void * write_value = malloc(tamano_frame);
        
        if(write_value == NULL){
            return;
        }
        memcpy(write_value,entrada+offset,tamano_frame);
        send_msg_memoria_data_write(get_pid_io_frames(io_frames), get_frame(frame_actual), get_desplazamiento(frame_actual), write_value, tamano_frame, fd_memoria);

        offset += tamano_frame;
        free(write_value);
    }
    
}
