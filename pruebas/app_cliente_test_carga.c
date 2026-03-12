//función main que llame a las funciones de la API
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../claves.h"

int main() {
    char key[256];
    float v2[1] = {5.5};
    struct Paquete p = {0,0,0};
    int n2;
    char v1_res[256];

    for (int i = 0; i < 50; i++) {
        sprintf(key, "multi_%d_%d", getpid(), i);
        
        // 1. Insertar
        set_value(key, "data", 1, v2, p);
        
        // 2. Comprobar inmediatamente que existe y es correcto
        if (get_value(key, v1_res, &n2, v2, &p) != 0) {
            printf("[ERROR] ¡Memoria corrompida en iteración %d!\n", i);
            exit(-1);
        }
    }
    printf("Cliente %d: 50 ciclos de Set/Get completados con éxito.\n", getpid());
    return 0;
}