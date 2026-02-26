typedef struct Nodo {
    char key[256];              // Clave: máx 255 caracteres + '\0' [cite: 8, 36]
    char value1[256];           // Valor 1: máx 255 caracteres + '\0' [cite: 10, 36]
    int N_value2;               // Dimensión del vector (entre 1 y 32) [cite: 12, 13]
    float V_value2[32];         // Vector de floats: máx 32 elementos [cite: 11, 36]
    struct Paquete value3;      // Estructura con enteros x, y, z 
    
    struct Nodo *siguiente;     // Puntero al siguiente nodo (gestión de colisiones) 
} Nodo;
