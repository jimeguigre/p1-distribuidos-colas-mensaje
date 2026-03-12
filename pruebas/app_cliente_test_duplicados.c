//función main que llame a las funciones de la API
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../claves.h"

int main() {
    char *key_conflictiva = "SOLO_UNO";
    struct Paquete p = {1, 1, 1};
    float v2[3] = {1.0, 2.0, 3.0};

    printf("Cliente %d intentando insertar clave duplicada...\n", getpid());
    
    // Si el mutex funciona, el servidor procesará una tras otra.
    // La primera devolverá 0, las demás -1.
    int res = set_value(key_conflictiva, "valor", 3, v2, p);
    
    if (res == 0) {
        printf("[OK] Cliente %d: ¡Fui el primero en insertar!\n", getpid());
    } else {
        printf("[EXPECTED] Cliente %d: Error, la clave ya existe (Mutex OK).\n", getpid());
    }
    return 0;
}