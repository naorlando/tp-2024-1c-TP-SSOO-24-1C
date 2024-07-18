#!/bin/bash

# Archivo de log
LOG_FILE="prueba_log.txt"

# Función para escribir en el log
log() {
    echo "$(date '+%Y-%m-%d %H:%M:%S') - $1" | tee -a "$LOG_FILE"
}

# Iniciar el log
echo "" > "$LOG_FILE"
log "Iniciando script"

# Rutas a las carpetas de los módulos
CPU_DIR="/home/utnso/tp-2024-1c-TP-SSOO-24-1C/cpu"
KERNEL_DIR="/home/utnso/tp-2024-1c-TP-SSOO-24-1C/kernel"
MEMORIA_DIR="/home/utnso/tp-2024-1c-TP-SSOO-24-1C/memoria"
ENTRADASALIDA_DIR="/home/utnso/tp-2024-1c-TP-SSOO-24-1C/entradasalida"

# Rutas a los ejecutables
CPU_EXECUTABLE="$CPU_DIR/bin/cpu"
KERNEL_EXECUTABLE="$KERNEL_DIR/bin/kernel"
MEMORIA_EXECUTABLE="$MEMORIA_DIR/bin/memoria"
ENTRADASALIDA_EXECUTABLE="$ENTRADASALIDA_DIR/bin/entradasalida"

log "Rutas de ejecutables configuradas"

# Tipos de dispositivos de entrada/salida disponibles
TIPOS_DISPOSITIVOS=(
    "1. Interfaz Genérica"
    "2. Interfaz STDIN"
    "3. Interfaz STDOUT"
    "4. Interfaz DialFS"
)

# Función para ejecutar make clean y make en un directorio
run_make() {
    local dir="$1"
    log "Ejecutando make clean y make en $dir"
    cd "$dir" || return
    make clean > /dev/null 2>&1
    make > make_output.txt 2>&1
    make_exit_code=$?

    if [ $make_exit_code -eq 0 ]; then
        log "Compilación exitosa en $dir"
    else
        log "Error en la compilación de $dir"
        exit 1
    fi

    cd - >/dev/null
}

# Función para ejecutar un módulo en una nueva terminal
run_module() {
    local executable="$1"
    local nombre_dispositivo="$2"
    local config_file="$3"
    log "Intentando ejecutar módulo: $executable"
    if [ -z "$config_file" ]; then
        log "Ejecutando sin archivo de configuración: $executable"
        xterm -e "$executable" &
    else
        log "Ejecutando con archivo de configuración: $executable \"$nombre_dispositivo\" $config_file"
        xterm -e "$executable \"$nombre_dispositivo\" $config_file" &
    fi
    sleep 2
    log "Esperando 2 segundos después de la ejecución"
}

# Función para seleccionar el tipo de dispositivo de entrada/salida
select_dispositivo() {
    log "Iniciando selección de dispositivo"
    read -p "Ingrese el nombre de la interfaz de entrada/salida: " nombre_dispositivo
    log "Nombre de interfaz ingresado: $nombre_dispositivo"
    
    echo "Seleccione el tipo de dispositivo de entrada/salida:"
    for tipo in "${TIPOS_DISPOSITIVOS[@]}"; do
        echo "$tipo"
    done
    
    read -p "Ingrese el número correspondiente: " opcion
    log "Opción seleccionada: $opcion"

    case $opcion in
        1)
            tipo_dispositivo="generica"
            ;;
        2)
            tipo_dispositivo="stdin"
            ;;
        3)
            tipo_dispositivo="stdout"
            ;;
        4)
            tipo_dispositivo="dialfs"
            ;;
        *)
            log "Opción inválida seleccionada"
            select_dispositivo
            return
            ;;
    esac

    config_file_lower="$ENTRADASALIDA_DIR/cfg/interfaz_${tipo_dispositivo}.config"
    config_file_upper="$ENTRADASALIDA_DIR/cfg/interfaz_${tipo_dispositivo^^}.config"

    if [ -f "$config_file_lower" ]; then
        config_file="$config_file_lower"
    elif [ -f "$config_file_upper" ]; then
        config_file="$config_file_upper"
    else
        log "Error: Archivo de configuración no encontrado: $config_file_lower o $config_file_upper"
        return
    fi

    log "Archivo de configuración encontrado: $config_file"
    
    log "Intentando ejecutar ENTRADASALIDA"
    if [ -x "$ENTRADASALIDA_EXECUTABLE" ]; then
        log "El ejecutable ENTRADASALIDA existe y tiene permisos de ejecución"
    else
        log "Error: El ejecutable ENTRADASALIDA no existe o no tiene permisos de ejecución"
        return
    fi
    
    log "Comando a ejecutar: $ENTRADASALIDA_EXECUTABLE \"$nombre_dispositivo\" $config_file"
    xterm -e "$ENTRADASALIDA_EXECUTABLE \"$nombre_dispositivo\" $config_file" &
    entradasalida_pid=$!
    log "ENTRADASALIDA iniciado con PID $entradasalida_pid"
    sleep 2
    if kill -0 $entradasalida_pid 2>/dev/null; then
        log "ENTRADASALIDA sigue ejecutándose después de 2 segundos"
    else
        log "Error: ENTRADASALIDA no está ejecutándose después de 2 segundos"
    fi
}

# Ejecutar make clean y make en los directorios de los módulos
log "Iniciando compilación de módulos"
run_make "$CPU_DIR"
run_make "$KERNEL_DIR"
run_make "$MEMORIA_DIR"
run_make "$ENTRADASALIDA_DIR"

log "Iniciando ejecución de módulos principales"
run_module "$MEMORIA_EXECUTABLE"
sleep 2
run_module "$CPU_EXECUTABLE"
sleep 2
run_module "$KERNEL_EXECUTABLE"
sleep 2

log "Solicitando número de dispositivos de entrada/salida"
read -p "¿Cuántos dispositivos de entrada/salida desea conectar? " num_dispositivos
log "Número de dispositivos solicitados: $num_dispositivos"

for ((i=1; i<=num_dispositivos; i++)); do
    log "Iniciando configuración del dispositivo $i"
    select_dispositivo
done

log "Todos los módulos iniciados. Entrando en bucle de espera."
# Mantener las terminales abiertas hasta que el usuario las cierre manualmente
while true; do
    sleep 1
done