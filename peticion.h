#ifndef PETICION_H
#define PETICION_H

#include "claves.h"

struct mensaje_peticion {
    int operacion;          // 0 para set, 1 para get, 2 para modify, 3 para delete, 4 para exist, 5 para destroy
    char q_cliente[256];    // Nombre de la cola donde el cliente espera respuesta
    char key[256];
    char value1[256];
    int N_value2;
    float V_value2[32];
    struct Paquete value3;
};

struct mensaje_respuesta {
    int resultado;          // 0 para éxito, -1 para error de servicio, -2 para comunicaciones
    char value1[256];
    int N_value2;
    float V_value2[32];
    struct Paquete value3;
};

#endif 