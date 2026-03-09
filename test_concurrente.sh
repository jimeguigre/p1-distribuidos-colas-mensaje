#!/bin/bash
# Lanzar 5 clientes en segundo plano
for i in {1..5}
do
   echo "Lanzando cliente $i..."
   ./app_cliente & 
done

# Esperar a que todos terminen
wait
echo "--- Todas las pruebas finalizadas ---"