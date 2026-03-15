#!/bin/bash

# Limpieza de colas y procesos antiguos
rm /dev/mqueue/* 2>/dev/null
killall servidor_mq 2>/dev/null

# Iniciar el servidor
./servidor_mq &
SERV_PID=$!
sleep 1

echo "--- INICIANDO PRUEBA DE CARGA (500 OPERACIONES) ---"

# Lanzar 10 clientes en paralelo
for i in {1..100}
do
   ./app_cliente_test_carga &
done

# 5. Esperar a que todos los hilos terminen
wait

echo "--- PRUEBA FINALIZADA ---"