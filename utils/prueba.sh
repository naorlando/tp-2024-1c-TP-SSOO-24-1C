#!/bin/bash

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
    cd "$dir" || return
    make clean
    make
    cd - >/dev/null
}

# Función para ejecutar un módulo en una nueva terminal
run_module() {
    local executable="$1"
    local config_file="$2"
    if [ -z "$config_file" ]; then
        exo-open --launch TerminalEmulator --execute "$executable"
    else
        exo-open --launch TerminalEmulator --execute "$executable" "$config_file"
    fi
    sleep 2  # Esperar 2 segundos para que la ventana se abra correctamente
}

# Función para seleccionar el tipo de dispositivo de entrada/salida
select_dispositivo() {
    read -p "Ingrese el nombre de la interfaz de entrada/salida: " nombre_dispositivo

    echo "Seleccione el tipo de dispositivo de entrada/salida:"
    for tipo in "${TIPOS_DISPOSITIVOS[@]}"; do
        echo "$tipo"
    done
    read -p "Ingrese el número correspondiente: " opcion

    case $opcion in
        1)
            tipo_dispositivo="GENERICA"
            ;;
        2)
            tipo_dispositivo="STDIN"
            ;;
        3)
            tipo_dispositivo="STDOUT"
            ;;
        4)
            tipo_dispositivo="DIALFS"
            ;;
        *)
            echo "Opción inválida. Intentelo nuevamente."
            select_dispositivo
            return
            ;;
    esac

    config_file="$ENTRADASALIDA_DIR/cfg/interfaz_$tipo_dispositivo.config"
    run_module "$ENTRADASALIDA_EXECUTABLE" "$nombre_dispositivo" "$config_file"
}

# Ejecutar make clean y make en los directorios de los módulos
run_make "$CPU_DIR"
run_make "$KERNEL_DIR"
run_make "$MEMORIA_DIR"
run_make "$ENTRADASALIDA_DIR"

# Seleccionar dispositivos de entrada/salida a conectar
read -p "¿Cuántos dispositivos de entrada/salida desea conectar? " num_dispositivos

for ((i=1; i<=num_dispositivos; i++)); do
    select_dispositivo
done

# Ejecutar los módulos en el orden especificado
run_module "$MEMORIA_EXECUTABLE"
sleep 2  # Esperar 2 segundos para que la ventana se abra correctamente
run_module "$CPU_EXECUTABLE"
sleep 2  # Esperar 2 segundos para que la ventana se abra correctamente
run_module "$KERNEL_EXECUTABLE"

# Mantener las terminales abiertas hasta que el usuario las cierre manualmente
while true; do
    sleep 1
done