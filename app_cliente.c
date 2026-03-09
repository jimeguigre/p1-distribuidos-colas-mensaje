//función main que llame a las funciones de la API
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "claves.h"

int main() {
    char *key = "clave_prueba";
    char v1[256] = "valor inicial 1";
    float v2[] = {1.1, 2.2, 3.3};
    int n2 = 3;
    struct Paquete v3 = {10, 20, 30};

    char res_v1[256];
    float res_v2[32];
    int res_n2;
    struct Paquete res_v3;

    // para las pruebas de concurrencia y para evitar colisiones entre clientes, cada cliente genera una clave única basada en su PID
    char clave_unica[256];
    sprintf(clave_unica, "clave_proceso_%d", getpid());

    printf("--- INICIANDO PLAN DE PRUEBAS ---\n");

    // inicializar el servicio
    if (destroy() == 0) {
        printf("Servicio inicializado/destruido con éxito.\n");
    } 

    // probar set_value
    if (set_value(clave_unica, v1, n2, v2, v3) == 0) {
        printf("Cliente %d: Inserción de '%s' correcta.\n", getpid(), clave_unica);
    } else {
        printf("Cliente %d: Error al insertar '%s'.\n", getpid(), clave_unica);
    }

    // probar exist
    if (exist(clave_unica) == 1) {
        printf("La clave '%s' existe en el sistema.\n", clave_unica);
    }

    // probar get_value (recuperar datos)
    // el cliente reserva espacio (256 chars y 32 floats) 
    if (get_value(key, res_v1, &res_n2, res_v2, &res_v3) == 0) {
        printf("Datos recuperados: v1 = '%s', n2 = %d, v3 = {%d,%d,%d}\n", 
                res_v1, res_n2, res_v3.x, res_v3.y, res_v3.z);
    }

    // probar modify_value
    char *nuevo_v1 = "valor modificado";
    v3.x = 99;
    if (modify_value(key, nuevo_v1, n2, v2, v3) == 0) {
        printf("Modificación de '%s' correcta.\n", key);
    }

    // verificar que los datos se han modificado
    if (get_value(key, res_v1, &res_n2, res_v2, &res_v3) == 0) {
        printf("Datos recuperados: v1 = '%s', n2 = %d, v3 = {%d,%d,%d}\n", 
                res_v1, res_n2, res_v3.x, res_v3.y, res_v3.z);
    }

    // probar delete_key
    if (delete_key(key) == 0) {
        printf("Borrado de '%s' correcto.\n", key);
    }

    // verificar que ya no existe
    if (exist(key) == 0) {
        printf("Confirmado: la clave '%s' ya no existe.\n", key);
    }

    printf("--- PRUEBAS FINALIZADAS ---\n");
    return 0;
}


