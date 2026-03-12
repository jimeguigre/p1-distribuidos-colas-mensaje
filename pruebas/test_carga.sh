#!/bin/bash

# 1. Limpieza de colas y procesos antiguos
rm /dev/mqueue/* 2>/dev/null
killall servidor_mq 2>/dev/null

# 2. Compilar (por si acaso hubo cambios)
make

# 3. Iniciar el servidor
./servidor_mq &
SERV_PID=$!
sleep 1

echo "--- INICIANDO PRUEBA DE CARGA (500 OPERACIONES) ---"

# 4. Lanzar 10 clientes en paralelo
for i in {1..10}
do
   ./app_cliente &
done

# 5. Esperar a que todos los hilos terminen
wait

echo "--- PRUEBA FINALIZADA ---"

# Opcional: Matar el servidor al terminar
kill $SERV_PID 2>/dev/null