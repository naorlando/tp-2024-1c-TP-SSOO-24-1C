#include "cpu_dispatch.h"

// case de ejecucion de instrucciones:
void ejecutar_instruccion(t_instruction *instruccion, t_cpu_registers *cpu_registers)
{
    log_info(logger_cpu, "EJECUTANDO INSTRUCCION");
    log_info(logger_cpu, "INSTRUCCION => %d", instruccion->name);

    switch (instruccion->name)
    {
    case SET:
    {
        log_info(logger_cpu, "EJECUTANDO SET");
        char *reg = (char *)list_get(instruccion->params, 0);
        int value = atoi((char *)list_get(instruccion->params, 1));
        _establecer_registro(cpu_registers, reg, value);
        log_info(logger_cpu, "SET %s %d\n", reg, value);
        break;
    }
    case MOV_IN:
    {
        log_info(logger_cpu, "EJECUTANDO MOV IN");
        char *reg_datos = (char *)list_get(instruccion->params, 0);
        char *reg_direccion = (char *)list_get(instruccion->params, 1);

        uint32_t direccion_logica = _obtener_valor_registro(cpu_registers, reg_direccion);

        uint32_t valor_memoria = 0;
        exec_mov_in(direccion_logica, reg_datos, &valor_memoria);
        _establecer_registro(cpu_registers, reg_datos, valor_memoria);
        log_info(logger_cpu, "MOV_IN %s %s\n", reg_datos, reg_direccion);
        break;
    }
    case MOV_OUT:
    {
        log_info(logger_cpu, "EJECUTANDO MOV OUT");
        char *reg_direccion = (char *)list_get(instruccion->params, 0);
        char *reg_datos = (char *)list_get(instruccion->params, 1);

        uint32_t direccion_logica = _obtener_valor_registro(cpu_registers, reg_direccion);
        uint32_t valor_datos = _obtener_valor_registro(cpu_registers, reg_datos);

        exec_mov_out(direccion_logica, &valor_datos, obtener_tamano_registro(reg_datos));

        log_info(logger_cpu, "MOV_OUT %s %s\n", reg_direccion, reg_datos);
        break;
    }
    case SUM:
    {
        char *reg_dest = (char *)list_get(instruccion->params, 0);
        char *reg_src = (char *)list_get(instruccion->params, 1);
        uint8_t valor_src = _obtener_valor_registro(cpu_registers, reg_src);
        uint8_t valor_operacion = _obtener_valor_registro(cpu_registers, reg_dest) + valor_src;
        _establecer_registro(cpu_registers, reg_dest, valor_operacion);
        log_info(logger_cpu, "SUM %s %s\n", reg_dest, reg_src);
        break;
    }
    case SUB:
    {
        char *reg_dest = (char *)list_get(instruccion->params, 0);
        char *reg_src = (char *)list_get(instruccion->params, 1);
        uint32_t valor_src = _obtener_valor_registro(cpu_registers, reg_src);
        uint32_t valor_operacion = _obtener_valor_registro(cpu_registers, reg_dest) - valor_src;
        _establecer_registro(cpu_registers, reg_dest, valor_operacion);
        log_info(logger_cpu, "SUB %s %s\n", reg_dest, reg_src);
        break;
    }
    case JNZ:
    {
        char *reg = (char *)list_get(instruccion->params, 0);
        int instruction_index = atoi((char *)list_get(instruccion->params, 1));
        uint32_t valor_reg = _obtener_valor_registro(cpu_registers, reg);
        uint32_t valor_cero = 0;
        if (valor_reg != valor_cero)
        {
            cpu_registers->pc = instruction_index;
            log_info(logger_cpu, "el registro %s era igual a cero, por lo tanto se salto a %d (JNZ)\n", reg, instruction_index);
            // le restamos uno al PC ya que al finalizar Execute se le va a sumar 1 al PC: entonces esto se cancela.
            cpu_registers->pc--;
        }
        log_info(logger_cpu, "JNZ %s %d\n", reg, instruction_index);
        break;
    }
    case RESIZE:
    {
        log_info(logger_cpu, "EJECUTANDO RESIZE");
        int nuevo_tamano = atoi((char *)list_get(instruccion->params, 0));

        // Función para solicitar a la memoria el ajuste de tamaño
        if (!ajustar_tamano_proceso(pcb_execute->pid, nuevo_tamano))
        {
            // TODO: Si la memoria devuelve Out of Memory, devolver el contexto al Kernel
            
            informar_kernel_error("Out of Memory");
        }
        else
        {
            log_info(logger_cpu, "RESIZE %d\n", nuevo_tamano);
        }
        break;
    }
    case COPY_STRING:
    {
        int tamano = atoi((char *)list_get(instruccion->params, 0));

        uint32_t direccion_origen = cpu_registers->si;
        uint32_t direccion_destino = cpu_registers->di;

        copiar_cadena(direccion_origen, direccion_destino, tamano);

        log_info(logger_cpu, "COPY_STRING %d\n", tamano);
        break;
    }
    case IO_GEN_SLEEP:
    {
        char *interface = (char *)list_get(instruccion->params, 0);
        int units = atoi((char *)list_get(instruccion->params, 1));

        // Enviar el PCB al kernel con el tipo de interfaz
        log_info(logger_cpu, "IO_GEN_SLEEP %s %d\n", interface, units);
        solicitar_IO(instruccion);
        solicitud_io = true;
        break;
    }
    case WAIT:
    {
        log_info(logger_cpu, "WAIT\n");
        // variable con nombre de recurso
        char *nombre_recurso = (char *)list_get(instruccion->params, 0);
        // logica de WAIT de un recurso:
        // mandar mensaje a kernel para que haga cositas con el recurso
        handle_wait_or_signal(pcb_execute, nombre_recurso, WAIT);
        // activo flag:
        solicitud_recurso = true;
        break;
    }
    case SIGNAL:
    {
        log_info(logger_cpu, "SIGNAL\n");
        // variable con nombre de recurso
        char *nombre_recurso = (char *)list_get(instruccion->params, 0);
        // logica de SIGNAL de un recurso:
        // mandar mensaje a kernel para que haga cositas con el recurso
        handle_wait_or_signal(pcb_execute, nombre_recurso, SIGNAL);
        // activo flag:
        solicitud_recurso = true;
        break;
    }
    case IO_STDIN_READ:
    {
        char *interface = (char *)list_get(instruccion->params, 0);
        char *reg_direccion = (char *)list_get(instruccion->params, 1);
        char *reg_tamano = (char *)list_get(instruccion->params, 2);

        // Enviar el PCB al kernel con el tipo de interfaz
        log_info(logger_cpu, "IO_STDIN_READ %s %s %s\n", interface, reg_direccion, reg_tamano);
        solicitar_IO(instruccion);
        solicitud_io = true;
        break;
    }
    case IO_STDOUT_WRITE:
    {
        char *interface = (char *)list_get(instruccion->params, 0);
        int units = atoi((char *)list_get(instruccion->params, 1));

        // Enviar el PCB al kernel con el tipo de interfaz
        log_info(logger_cpu, "IO_STDOUT_WRITE %s %d\n", interface, units);
        solicitar_IO(instruccion);
        solicitud_io = true;
        break;
    }
    case EXIT:
    {
        log_info(logger_cpu, "EXIT\n");
        // sem_wait(&SEM_INTERRUPT); //BINARIO
        // interrupcion_pendiente = true;
        // tipo_de_interrupcion = EXIT_INTERRUPT;
        enviar_pcb_finalizado();
        llego_a_exit = true;
        break;
    }
    default:
        printf("Instrucción no reconocida\n");
        break;
    }
}

// FUNCIONES AUXILIARES DE EJECUCION DE INSTRUCCIONES:

// #############################################################################################################
// TODO: REVISAR MACHEO DE INT, debuggear.
// #############################################################################################################
// Función para obtener el puntero a un registro basado en su nombre:
uint32_t *_obtener_registro(t_cpu_registers *registros, const char *nombre)
{
    if (strcmp(nombre, "AX") == 0)
    {
        // log_info(logger_cpu, "AX = %u", (uint32_t)registros->ax);
        return (uint32_t *)&registros->ax;
        // uint32_t valor = (uint32_t)registros->ax;
        // return &valor;
    }
    if (strcmp(nombre, "BX") == 0)
    {
        // log_info(logger_cpu, "BX = %u", (uint32_t)registros->bx);
        return (uint32_t *)&registros->bx;
        // uint32_t valor = (uint32_t)registros->bx;
        // return &valor;
    }
    if (strcmp(nombre, "CX") == 0)
    {
        log_info(logger_cpu, "CX = %u", (uint32_t)registros->cx);
        return (uint32_t *)&registros->cx;
    }
    if (strcmp(nombre, "DX") == 0)
    {
        log_info(logger_cpu, "DX = %u", (uint32_t)registros->dx);
        return (uint32_t *)&registros->dx;
    }
    if (strcmp(nombre, "EAX") == 0)
    {
        log_info(logger_cpu, "EAX = %u", registros->eax);
        return &registros->eax;
    }
    if (strcmp(nombre, "EBX") == 0)
    {
        log_info(logger_cpu, "EBX = %u", registros->ebx);
        return &registros->ebx;
    }
    if (strcmp(nombre, "ECX") == 0)
    {
        log_info(logger_cpu, "ECX = %u", registros->ecx);
        return &registros->ecx;
    }
    if (strcmp(nombre, "EDX") == 0)
    {
        log_info(logger_cpu, "EDX = %u", registros->edx);
        return &registros->edx;
    }
    if (strcmp(nombre, "SI") == 0)
    {
        log_info(logger_cpu, "SI = %u", registros->si);
        return &registros->si;
    }
    if (strcmp(nombre, "DI") == 0)
    {
        log_info(logger_cpu, "DI = %u", registros->di);
        return &registros->di;
    }
    return NULL;
}

// Función para establecer el valor de un registro
// void _establecer_registro(t_cpu_registers *registros, const char *nombre, uint32_t valor) {
//     uint32_t *reg = _obtener_registro(registros, nombre);
//     if (reg) *reg = valor;
// }
void _establecer_registro(t_cpu_registers *registros, char *nombre, uint32_t valor)
{
    if (strcmp(nombre, "AX") == 0)
        registros->ax = valor;
    if (strcmp(nombre, "BX") == 0)
        registros->bx = valor;
    if (strcmp(nombre, "CX") == 0)
        registros->cx = valor;
    if (strcmp(nombre, "DX") == 0)
        registros->dx = valor;
    if (strcmp(nombre, "EAX") == 0)
        registros->eax = valor;
    if (strcmp(nombre, "EBX") == 0)
        registros->ebx = valor;
    if (strcmp(nombre, "ECX") == 0)
        registros->ecx = valor;
    if (strcmp(nombre, "EDX") == 0)
        registros->edx = valor;
    if (strcmp(nombre, "SI") == 0)
        registros->si = valor;
    if (strcmp(nombre, "DI") == 0)
        registros->di = valor;
}

// Función para obtener el valor de un registro
uint32_t _obtener_valor_registro(t_cpu_registers *registros, char *nombre)
{
    remove_newline(nombre);
    uint32_t *reg = _obtener_registro(registros, nombre);
    return reg ? *reg : 0;
}

// #############################################################################################################
// TP:
// Es importante tener en cuenta las siguientes aclaraciones:
// Una dirección lógica se traduce a una dirección física, pero al copiar un string/registro a memoria,
// podría estar presente en más de una página (ver sección de MMU).
// #############################################################################################################

void informar_kernel_error(const char *mensaje)
{
    // TODO: Implementa la lógica para informar al Kernel sobre un error.
    printf("Kernel error: %s\n", mensaje);
}

void solicitar_instruccion(uint32_t pid, uint32_t pc)
{
    // Pido la siguiente instruccion a memoria
    send_get_next_instruction_memoria(pid, pc);
}

void recibir_pcb()
{
    sem_wait(&SEM_SOCKET_KERNEL_DISPATCH);

    // Recibo el pcb que manda kernel para ejecutar sus instrucciones
    pcb_execute = recv_pcb_kernel();

    // Cargo el contexto de ejecucion del pcb en los registros de la cpu
    cargar_contexto_ejecucion(pcb_execute);

    // Pido a memoria que me mande las instrucciones del proceso
    solicitar_instruccion(pcb_execute->pid, pcb_execute->program_counter);
}

void manejar_ciclo_de_instruccion()
{
    // FETCH: Recibo la instruccion que manda memoria
    t_instruction *instruccion = recv_instruction_memoria();

    log_info(logger_cpu, "Instrucción recibida de memoria");
    log_info(logger_cpu, "antes: AX: %u", cpu_registers->ax);
    log_info(logger_cpu, "antes: BX: %u", cpu_registers->bx);
    log_info(logger_cpu, "antes: CX: %u", cpu_registers->cx);
    log_info(logger_cpu, "antes: DX: %u", cpu_registers->dx);
    log_info(logger_cpu, "antes: EAX: %u", cpu_registers->eax);
    log_info(logger_cpu, "antes: EBX: %u", cpu_registers->ebx);
    log_info(logger_cpu, "antes: ECX: %u", cpu_registers->ecx);
    log_info(logger_cpu, "antes: EDX: %u", cpu_registers->edx);
    log_info(logger_cpu, "antes: SI: %u", cpu_registers->si);
    log_info(logger_cpu, "antes: DI: %u", cpu_registers->di);

    // EXECUTE: Ejecuto la instruccion recibida
    ejecutar_instruccion(instruccion, cpu_registers);
    // sleep(5);
    // imprimir todos los cpu_registers:
    log_info(logger_cpu, "despues: AX: %u", cpu_registers->ax);
    log_info(logger_cpu, "despues: BX: %u", cpu_registers->bx);
    log_info(logger_cpu, "despues: CX: %u", cpu_registers->cx);
    log_info(logger_cpu, "despues: DX: %u", cpu_registers->dx);
    log_info(logger_cpu, "despues: EAX: %u", cpu_registers->eax);
    log_info(logger_cpu, "despues: EBX: %u", cpu_registers->ebx);
    log_info(logger_cpu, "despues: ECX: %u", cpu_registers->ecx);
    log_info(logger_cpu, "despues: EDX: %u", cpu_registers->edx);
    log_info(logger_cpu, "despues: SI: %u", cpu_registers->si);
    log_info(logger_cpu, "despues: DI: %u", cpu_registers->di);

    eliminar_instruccion(instruccion);

    // se debe hacer un "return;", si el proceso llego a exit
    if (llego_a_exit)
    {
        llego_a_exit = false; // seteamos en false para el siguiente pcb
        log_info(logger_cpu, "El PCB de pid <%d> se retira del ciclo de instruccion por una instruccion EXIT", pcb_execute->pid);
        return;
    }
    // se debe hacer un "return;", si el proceso maneja un recurso:
    if (solicitud_recurso)
    {
        solicitud_recurso = false;
        // log: se envio el proceso a kernel para el manejo de recursos
        log_info(logger_cpu, "El PCB de pid <%d> se envio al KERNEL para hacer un WAIT/SIGNAL un recurso (puede no volver) ", pcb_execute->pid);
        return;
    }

    // se debe hacer un "return;", si el proceso solicito una io
    if (solicitud_io)
    {
        solicitud_io = false;
        return;
    }

    aumentar_program_counter();

    // SOlo para seguir el flujo
    log_info(logger_cpu, "El PCB de pid <%d> tiene el pc en <%d>", pcb_execute->pid, pcb_execute->program_counter);

    // INTERRUPT: verificar y manejar interrupciones después de ejecutar la instrucción
    if (manejar_interrupcion())
        return;

    // SOlo para seguir el flujo
    // log_info(logger_cpu, "El PCB de pid <%d> solicito la siguiente intruccion a memoria con el pc en <%d>", pcb_execute->pid, pcb_execute->program_counter);

    // TODO: Se debe actualizar el PC antes de pedir la siguiente instruccion a memoria
    solicitar_instruccion(pcb_execute->pid, pcb_execute->program_counter);
}

bool manejar_interrupcion()
{
    pthread_mutex_lock(&MUTEX_INTERRUPT);
    if (interrupcion_pendiente)
    {
        log_info(logger_cpu, "Interrupción de %s recibida, devolviendo PCB al Kernel", get_string_from_interruption(tipo_de_interrupcion));
        cargar_contexto_ejecucion_a_pcb(pcb_execute);

        log_info(logger_cpu, "PCB enviado al Kernel");
        send_pcb_kernel_interruption(tipo_de_interrupcion); // aca esta la logica de cual mensaje enviar al kernel segun cual sea el tipo de interrupccion
        // sem_post(&SEM_INTERRUPT);
        interrupcion_pendiente = false;
        pthread_mutex_unlock(&MUTEX_INTERRUPT);
        sem_post(&SEM_SOCKET_KERNEL_DISPATCH);

        return true;
    }
    else
    {
        pthread_mutex_unlock(&MUTEX_INTERRUPT);
    }
    return false;
}

void solicitar_IO(t_instruction *instruccion)
{
    // t_interface interface = create_interface(pcb_execute, instruccion);
    // send_interface_kernel(/*interface*/);
    aumentar_program_counter();
    cargar_contexto_ejecucion_a_pcb(pcb_execute);

    switch (obtener_nombre_instruccion(instruccion))
    {
    case IO_GEN_SLEEP:
        send_solicitud_io_generica_kernel(pcb_execute, instruccion);
        break;
    case IO_STDIN_READ:
        send_solicitud_io_stdin_kernel(pcb_execute,instruccion);
        break;
    case IO_STDOUT_WRITE:

        break;
    case IO_FS_CREATE:

        break;
    case IO_FS_DELETE:

        break;
    case IO_FS_TRUNCATE:

        break;
    case IO_FS_WRITE:

        break;
    case IO_FS_READ:

        break;
    default:

        break;
    }
    log_info(logger_cpu, "El PCB de pid <%d> se envio al KERNEL para solicitar una IO", pcb_execute->pid);
    sem_post(&SEM_SOCKET_KERNEL_DISPATCH);
}

void cargar_contexto_ejecucion(t_PCB *pcb)
{
    t_cpu_registers *contexto = get_cpu_registers(pcb);

    // Cargo el contexto de ejecucion del pcb en la CPU
    cpu_registers->pc = contexto->pc;
    cpu_registers->ax = contexto->ax;
    cpu_registers->bx = contexto->bx;
    cpu_registers->cx = contexto->cx;
    cpu_registers->dx = contexto->dx;
    cpu_registers->eax = contexto->eax;
    cpu_registers->ebx = contexto->ebx;
    cpu_registers->ecx = contexto->ecx;
    cpu_registers->edx = contexto->edx;
    cpu_registers->si = contexto->si;
    cpu_registers->di = contexto->di;
}
// cargar contexto de ejecucion del cpu a los registros del pcb
void cargar_contexto_ejecucion_a_pcb(t_PCB *pcb)
{
    t_cpu_registers *contexto = get_cpu_registers(pcb);

    // Cargo el contexto de ejecucion de la CPU en el pcb
    contexto->pc = cpu_registers->pc;
    contexto->ax = cpu_registers->ax;
    contexto->bx = cpu_registers->bx;
    contexto->cx = cpu_registers->cx;
    contexto->dx = cpu_registers->dx;
    contexto->eax = cpu_registers->eax;
    contexto->ebx = cpu_registers->ebx;
    contexto->ecx = cpu_registers->ecx;
    contexto->edx = cpu_registers->edx;
    contexto->si = cpu_registers->si;
    contexto->di = cpu_registers->di;
}

void enviar_pcb_finalizado()
{
    cargar_contexto_ejecucion_a_pcb(pcb_execute);
    log_info(logger_cpu, "se envia un PCB al Kernel por EXIT con PC= %d", pcb_execute->program_counter);
    send_pcb_kernel();
    sem_post(&SEM_SOCKET_KERNEL_DISPATCH);

    // Controlo si llego una interrupcion
    if (interrupcion_pendiente)
    {
        pthread_mutex_lock(&MUTEX_INTERRUPT);
        interrupcion_pendiente = false; // la desestimo
        pthread_mutex_unlock(&MUTEX_INTERRUPT);
    }
}

void aumentar_program_counter()
{
    // actualizar PC:
    cpu_registers->pc++;
    pcb_execute->program_counter = cpu_registers->pc;
}

void handle_wait_or_signal(t_PCB *pcb, char *resource_name, t_name_instruction tipo_de_interrupcion)
{

    t_msg_header msg_header;
    if (tipo_de_interrupcion == WAIT)
    {
        msg_header = MSG_CPU_KERNEL_WAIT;
    }
    else if (tipo_de_interrupcion == SIGNAL)
    {
        msg_header = MSG_CPU_KERNEL_SIGNAL;
    }
    else
    {
        log_error(logger_cpu, "Tipo de interrupcion invalido para recurso.");
        return;
    }

    remove_newline(resource_name);

    // actualizar PC:
    aumentar_program_counter();
    cargar_contexto_ejecucion_a_pcb(pcb);

    t_manejo_recurso *t_manejo_recurso = manejo_recurso_create(pcb, resource_name);
    // Crear un paquete con el PCB y el nombre del recurso (t_manjejo_recurso) y enviarlo al Kernel:

    t_package *package = package_create(msg_header, get_manejo_recurso_size(t_manejo_recurso));
    serialize_manejo_recurso(package->buffer, t_manejo_recurso);

    // Enviar el paquete al Kernel:
    package_send(package, fd_kernel_dispatch);
    package_destroy(package);
    sem_post(&SEM_SOCKET_KERNEL_DISPATCH);

    manejo_recurso_destroy(t_manejo_recurso);
}
