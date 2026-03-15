//función main que llame a las funciones de la API
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../claves.h"

#include <sys/time.h>

struct timeval start, end;
double tiempo_total;


int main() {
    char key[256];
    float v2[1] = {5.5};
    struct Paquete p = {0,0,0};
    int n2;
    char v1_res[256];
    gettimeofday(&start, NULL); // Marca de tiempo inicial

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
    gettimeofday(&end, NULL);   // Marca de tiempo final

    // Calcular tiempo en segundos
    tiempo_total = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;

    printf("\n--- ESTADÍSTICAS DE RENDIMIENTO ---\n");
    printf("Tiempo total: %.4f segundos\n", tiempo_total);
    printf("Operaciones por segundo (Throughput): %.2f ops/sec\n", 500 / tiempo_total);
    printf("Latencia media: %.4f ms/op\n", (tiempo_total / 500) * 1000);
    printf("Cliente %d: 50 ciclos de Set/Get completados con éxito.\n", getpid());
    return 0;

}

