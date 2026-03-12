#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../claves.h"

int main() {
    char v1[256];
    int n2;
    float v2[32];
    struct Paquete v3;

    printf("Lector [%d]: Iniciando 5000 lecturas de verificación...\n", getpid());

    for (int i = 0; i < 5000; i++) {
        if (get_value("CLAVE_TEST", v1, &n2, v2, &v3) == 0) {
            
            // Verificación de CONSISTENCIA:
            // Si el texto es ESTADO_A, el paquete x debe ser 1.
            // Si el texto es ESTADO_B, el paquete x debe ser 9.
            if (strcmp(v1, "ESTADO_A") == 0) {
                if (v3.x != 1) {
                    printf("\n[!!!] INCONSISTENCIA detectada: v1=ESTADO_A pero v3.x=%d\n", v3.x);
                    return -1;
                }
            } else if (strcmp(v1, "ESTADO_B") == 0) {
                if (v3.x != 9) {
                    printf("\n[!!!] INCONSISTENCIA detectada: v1=ESTADO_B pero v3.x=%d\n", v3.x);
                    return -1;
                }
            }
        }
        if (i % 1000 == 0) printf("Lectura %d completada...\n", i);
    }

    printf("\n[OK] Lector [%d]: 5000 lecturas consistentes. El Mutex funciona.\n", getpid());
    return 0;
}