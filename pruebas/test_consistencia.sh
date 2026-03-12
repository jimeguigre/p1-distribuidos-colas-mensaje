#!/bin/bash

# Asegurar que las librerías se encuentran en el path actual
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.

# 1. Limpieza de colas y procesos antiguos
rm /dev/mqueue/* 2>/dev/null
killall servidor_mq 2>/dev/null
killall escritor 2>/dev/null

# 2. Iniciar el servidor
./servidor_mq &
sleep 1

echo "--- INICIANDO PRUEBA DE CONSISTENCIA ---"

# 3. Lanzar el escritor en segundo plano
./cliente_escritor &
ESC_PID=$!

# 4. Ejecutar el lector (este es el que manda y termina solo)
./cliente_lector
RESULTADO=$?

# 5. Finalizar
kill $ESC_PID
killall servidor_mq 2>/dev/null

if [ $RESULTADO -eq 0 ]; then
    echo "--- PRUEBA SUPERADA ---"
else
    echo "--- PRUEBA FALLIDA: Se detectaron datos mezclados ---"
fi