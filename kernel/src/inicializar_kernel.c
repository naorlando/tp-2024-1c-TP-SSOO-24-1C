#include "inicializar_kernel.h"

t_log *logger_kernel;
t_log *logger_kernel_debug;
t_config *config_kernel;
t_kernel_config *kernel_config;

int fd_cpu_dispatch;
int fd_cpu_interrupt;
int fd_kernel_memoria;
int fd_server;

char *cpu_dispatch_port;
char *cpu_interrupt_port; 
char *memoria_port;
char *server_port;

t_dictionary *table_pcb;
int identificador_PID = 1;
t_datos_hilo* datos_hilo_quantum;
bool interrupcion_enviada = false;
bool planificador_status = true;
t_dictionary* io_connections;
t_dictionary *recursos_dictionary;
//pthread_mutex_t mutex_recursos;

pthread_mutex_t mutex_pid = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t MUTEX_READY;
pthread_mutex_t MUTEX_EXIT;
pthread_mutex_t MUTEX_NEW;
pthread_mutex_t MUTEX_EXECUTE;
pthread_mutex_t MUTEX_DICTIONARY;
pthread_mutex_t MUTEX_RECURSOS;

sem_t SEM_READY;
sem_t BLOQUEADOR;
sem_t SEM_EXIT;
sem_t SEM_NEW;
sem_t SEM_MULTIPROGRAMACION;
sem_t SEM_CPU; 

t_queue *COLA_READY;
t_queue *COLA_AUX_READY;
t_queue *COLA_EXIT;
t_queue *COLA_NEW;
t_PCB *EXECUTE;
t_list *LISTA_COLAS_DISPOSITIVOS;

void init()
{
    _iniciar_logger();
    _iniciar_config();
    _init_table_pcb();
    imprimir_config();
    initialize_lists();
    initialize_mutexes();
    initialize_semaphores();
    inicializar_planificadores();
    inicializar_dictionarios();
    inicializar_recursos();
}

void _iniciar_logger()
{
    logger_kernel = log_create("kernel.log", "KERNEL_LOG", 1, LOG_LEVEL_INFO);

    if (logger_kernel == NULL)
    {
        perror("No se pudo crear el archivo log para el modulo de kernel");
        // exit(EXIT_FAILURE);
    }

    logger_kernel_debug = log_create("kernel.log", "KERNEL_LOG", 1, LOG_LEVEL_TRACE);

    if (logger_kernel_debug == NULL)
    {
        perror("No se pudo crear el archivo log de debug para el modulo de kernel");
        // exit(EXIT_FAILURE);
    }
}

void _iniciar_config()
{
    config_kernel = config_create("/home/utnso/tp-2024-1c-TP-SSOO-24-1C/kernel/cfg/kernel.config");

    if (config_kernel == NULL)
    {
        perror("No se pudo crear el archivo de configuracion para el modulo de kernel");
        // exit(EXIT_FAILURE);
    }

    // Creo la estructura para guardar los datos del archivo de configuracion
    kernel_config = crear_kernel_config();

    if (kernel_config == NULL)
    {
        log_error(logger_kernel, "Error al crear la estructura t_kernel_config");
    }

    if(cargar_kernel_config(kernel_config, config_kernel)) {
        log_info(logger_kernel, "La estructura t_kernel_config fue cargado correctamente");
    } else {
        log_error(logger_kernel, "Error al cargar la estructura t_kernel_config");
    }
}

void imprimir_config()
{
    log_trace(logger_kernel_debug, "PUERTO DE ESCUCHA DE LA KERNEL: %d", obtener_puerto_escucha(kernel_config));
}

void _init_table_pcb()
{
    create_pcb_table();

    if (table_pcb != NULL)
    {
        log_info(logger_kernel, "Se creo correctamente la tabla de pcb.");
    }
    else
    {
        log_error(logger_kernel, "Error al crear la tabla de pcb.");
    }
}

void inicializar_planificadores()
{

    pthread_t THREAD_LARGO_PLAZO;
    if (!pthread_create(&THREAD_LARGO_PLAZO, NULL, (void *) planificador_largo_plazo, NULL))
        pthread_detach(THREAD_LARGO_PLAZO);
    else
    {
        log_error(logger_kernel, "ERROR CRITICO INICIANDO EL PLANIFICADOR DE LARGO PLAZO. ABORTANDO.");
        exit(EXIT_FAILURE);
    }

   
    pthread_t THREAD_CORTO_PLAZO;
    if (!pthread_create(&THREAD_CORTO_PLAZO, NULL, (void *) planificador_corto_plazo, NULL))
        pthread_detach(THREAD_CORTO_PLAZO);
    else
    {
        log_error(logger_kernel, "ERROR CRITICO INICIANDO EL PLANIFICADOR DE CORTO PLAZO. ABORTANDO.");
        exit(EXIT_FAILURE);
    }
}


void initialize_lists()
{
    COLA_READY = queue_create();
    COLA_AUX_READY = queue_create();
    COLA_EXIT = queue_create();
    COLA_NEW = queue_create();
    LISTA_COLAS_DISPOSITIVOS = list_create();
}

void inicializar_dictionarios()
{
    io_connections = dictionary_create();
    recursos_dictionary = dictionary_create();
}

void inicializar_recursos() {

    char **nombres_recursos = config_get_array_value(kernel_config, "RECURSOS");
    char **instancias_recursos = config_get_array_value(kernel_config, "INSTANCIAS_RECURSOS");

    for (int i = 0; nombres_recursos[i] != NULL; i++) {
        t_recurso *recurso = malloc(sizeof(t_recurso));
        recurso->nombre = strdup(nombres_recursos[i]);
        recurso->instancias = atoi(instancias_recursos[i]);
        recurso->cola_bloqueados = queue_create();

        dictionary_put(recursos_dictionary, recurso->nombre, recurso);
    }

    string_array_destroy(nombres_recursos);
    string_array_destroy(instancias_recursos);
}