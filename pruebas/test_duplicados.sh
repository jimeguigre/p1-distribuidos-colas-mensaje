#!/bin/bash

# 1. Limpieza inicial: Borramos colas viejas y matamos servidores antiguos
rm /dev/mqueue/* 2>/dev/null
killall servidor_mq 2>/dev/null

# 2. Iniciar el servidor en segundo plano
./servidor_mq &
sleep 1 # Esperar a que el servidor abra la cola /SERVIDOR

echo "--- LANZANDO 20 CLIENTES A LA VEZ ---"

# 3. Lanzar 20 clientes simultáneamente
for i in {1..20}
do
   ./app_cliente_test_duplicados & 
done

# 4. Esperar a que todos terminen
wait

echo "--- PRUEBA FINALIZADA ---"