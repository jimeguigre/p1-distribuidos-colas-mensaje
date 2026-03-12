#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "../claves.h"

int main() {
    // Definimos dos estados distintos para la misma clave
    char *v1_a = "ESTADO_A";
    float v2_a[1] = {1.1};
    struct Paquete v3_a = {1, 1, 1};

    char *v1_b = "ESTADO_B";
    float v2_b[1] = {9.9};
    struct Paquete v3_b = {9, 9, 9};

    // Inicializamos la clave en el servidor
    if (set_value("CLAVE_TEST", v1_a, 1, v2_a, v3_a) != 0) {
        // Si ya existe de una prueba anterior, la modificamos
        modify_value("CLAVE_TEST", v1_a, 1, v2_a, v3_a);
    }

    printf("Escritor [%d]: Alternando estados de 'CLAVE_TEST' sin parar...\n", getpid());

    while(1) {
        // Alternar a estado B
        modify_value("CLAVE_TEST", v1_b, 1, v2_b, v3_b);
        // Alternar a estado A
        modify_value("CLAVE_TEST", v1_a, 1, v2_a, v3_a);
    }

    return 0;
}