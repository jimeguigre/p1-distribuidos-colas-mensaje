#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "claves.h"


#define MAX_HASH 1031 // Número primo para reducir colisiones

typedef struct Nodo {
    char key[256];              // Clave: máx 255 caracteres + '\0' [cite: 8, 36]
    char value1[256];           // Valor 1: máx 255 caracteres + '\0' [cite: 10, 36]
    int N_value2;               // Dimensión del vector (entre 1 y 32) [cite: 12, 13]
    float V_value2[32];         // Vector de floats: máx 32 elementos [cite: 11, 36]
    struct Paquete value3;      // Estructura con enteros x, y, z 
    struct Nodo *siguiente;     // Puntero al siguiente nodo (gestión de colisiones) 
} Nodo;


// La tabla hash global
static Nodo *tabla[MAX_HASH]; 

// Función hash simple (djb2)
unsigned int funcion_hash(char *str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;
    return hash % MAX_HASH;
}



// código para set_value: 
int set_value(char *key, char *value1, int N_value2, float *V_value2, struct Paquete value3) {
    // 1. Validar N_value2 [cite: 31]
    if (N_value2 < 1 || N_value2 > 32) return -1;

    // 2. Calcular índice hash
    unsigned int idx = funcion_hash(key);

    // 3. Comprobar si la clave ya existe [cite: 31]
    Nodo *actual = tabla[idx];
    while (actual != NULL) {
        if (strcmp(actual->key, key) == 0) return -1;
        actual = actual->siguiente;
    }

    // 4. Crear nuevo nodo (Sin límite de elementos [cite: 77, 144])
    Nodo *nuevo = (Nodo *)malloc(sizeof(Nodo));
    if (nuevo == NULL) return -1;

    // 5. Copiar datos
    strncpy(nuevo->key, key, 255);
    strncpy(nuevo->value1, value1, 255);
    nuevo->N_value2 = N_value2;
    memcpy(nuevo->V_value2, V_value2, N_value2 * sizeof(float));
    nuevo->value3 = value3;

    // 6. Insertar al principio de la lista del índice idx
    nuevo->siguiente = tabla[idx];
    tabla[idx] = nuevo;

    return 0; // Éxito [cite: 30]
}