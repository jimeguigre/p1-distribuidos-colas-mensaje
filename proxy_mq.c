//proxy-mq.c: implementa la misma API que claves.h pero en lugar de guardar datos suplanta a las funciones originales. 
//empaqueta los argumentos 
//envía argumentos al servidor por colas de mensaje
//espera la respuesta y la devuelve a app-cliente.c

#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "claves.h"
#include "peticion.h" 
#include <string.h>

int set_value(char *key, char *value1, int N_value2, float *V_value2, struct Paquete value3) {
    mqd_t q_servidor, q_cliente;
    struct mensaje_peticion pet;
    struct mensaje_respuesta res;
    char nombre_cola[256];

    // 1. Nombre de cola único
    sprintf(nombre_cola, "/q_cli_%d", getpid());

    // 2. Atributos: el servidor debe enviar un mensaje de tamaño mensaje_respuesta
    struct mq_attr attr = { .mq_maxmsg = 1, .mq_msgsize = sizeof(struct mensaje_respuesta) };
    
    // Abrir/Crear cola del cliente
    mq_unlink(nombre_cola); // Limpiar por si acaso quedó una de un crash previo
    q_cliente = mq_open(nombre_cola, O_CREAT | O_RDONLY, 0666, &attr);
    if (q_cliente == -1) {
        perror("Error al crear cola cliente");
        return -1;
    }

    // 3. Preparar petición
    memset(&pet, 0, sizeof(pet)); // Limpiar estructura
    pet.operacion = 0; 
    strncpy(pet.q_cliente, nombre_cola, 255);
    strncpy(pet.key, key, 255);
    strncpy(pet.value1, value1, 255);
    pet.N_value2 = N_value2;
    if (N_value2 > 0 && V_value2 != NULL)
        memcpy(pet.V_value2, V_value2, N_value2 * sizeof(float));
    pet.value3 = value3;

    // 4. Enviar al servidor
    q_servidor = mq_open("/SERVIDOR", O_WRONLY);
    if (q_servidor == -1) {
        mq_close(q_cliente);
        mq_unlink(nombre_cola);
        return -2;
    }

    if (mq_send(q_servidor, (const char *)&pet, sizeof(pet), 0) == -1) {
        perror("Error mq_send");
        // Limpiar...
        return -2;
    }

    // 5. Recibir respuesta
    if (mq_receive(q_cliente, (char *)&res, sizeof(res), NULL) == -1) {
        perror("Error mq_receive");
        // Limpiar...
        return -2;
    }

    // 6. Limpieza ordenada
    mq_close(q_servidor);
    mq_close(q_cliente);
    mq_unlink(nombre_cola);

    return res.resultado;
}


int get_value(char *key, char *value1, int *N_value2, float *V_value2, struct Paquete *value3) {
    // Implementación similar a set_value pero con operacion = 1 y lectura de respuesta
    mqd_t q_servidor, q_cliente;
    struct mensaje_peticion pet;
    struct mensaje_respuesta res;
    char nombre_cola[256];

    // configurar cola de respuesta única para este cliente
    sprintf(nombre_cola, "/cola_cliente_%d", getpid());
    struct mq_attr attr = { .mq_maxmsg = 1, .mq_msgsize = sizeof(struct mensaje_respuesta) };
    q_cliente = mq_open(nombre_cola, O_CREAT | O_RDONLY, 0666, &attr);

    // preparar la petición
    pet.operacion = 1; // Código para GET
    strncpy(pet.q_cliente, nombre_cola, 255);
    strncpy(pet.key, key, 255);

    // enviar al servidor
    q_servidor = mq_open("/SERVIDOR", O_WRONLY);
    if (q_servidor == -1){
        mq_close(q_cliente);
        mq_unlink(nombre_cola);
        return -2; // Error de comunicaciones
    }

    mq_send(q_servidor, (const char *)&pet, sizeof(pet), 0);

    // recibir respuesta (bloqueante)
    mq_receive(q_cliente, (char *)&res, sizeof(res), NULL);


    if (res.resultado == 0) {
        // Copiamos lo que envió el servidor a los punteros que nos dio el cliente
        strcpy(value1, res.value1);
        *N_value2 = res.N_value2;
        memcpy(V_value2, res.V_value2, res.N_value2 * sizeof(float));
        *value3 = res.value3;
    }

    // limpieza
    mq_close(q_servidor);
    mq_close(q_cliente);
    mq_unlink(nombre_cola);

    return res.resultado; // Devuelve el 0 o -1 que envió el servidor
}

int modify_value(char *key, char *value1, int N_value2, float *V_value2, struct Paquete value3) {
    // Implementación similar a set_value pero con operacion = 2
    mqd_t q_servidor, q_cliente;
    struct mensaje_peticion pet;
    struct mensaje_respuesta res;
    char nombre_cola[256];

    // configurar cola de respuesta única para este cliente
    sprintf(nombre_cola, "/cola_cliente_%d", getpid());
    struct mq_attr attr = { .mq_maxmsg = 1, .mq_msgsize = sizeof(struct mensaje_respuesta) };
    q_cliente = mq_open(nombre_cola, O_CREAT | O_RDONLY, 0666, &attr);

    // preparar la petición
    pet.operacion = 2; // Código para MODIFY
    strcpy(pet.q_cliente, nombre_cola);
    strncpy(pet.key, key, 255);
    strncpy(pet.value1, value1, 255);
    pet.N_value2 = N_value2;
    memcpy(pet.V_value2, V_value2, N_value2 * sizeof(float));
    pet.value3 = value3;

    // enviar al servidor
    q_servidor = mq_open("/SERVIDOR", O_WRONLY);
    if (q_servidor == -1) return -2; // Error de comunicaciones

    mq_send(q_servidor, (const char *)&pet, sizeof(pet), 0);

    // recibir respuesta (bloqueante)
    mq_receive(q_cliente, (char *)&res, sizeof(res), NULL);

    // limpieza
    mq_close(q_servidor);
    mq_close(q_cliente);
    mq_unlink(nombre_cola);

    return res.resultado; // Devuelve el 0 o -1 que envió el servidor
}

int delete_key(char *key) {
    // Implementación similar a set_value pero con operacion = 3
    mqd_t q_servidor, q_cliente;
    struct mensaje_peticion pet;
    struct mensaje_respuesta res;
    char nombre_cola[256];

    // configurar cola de respuesta única para este cliente
    sprintf(nombre_cola, "/cola_cliente_%d", getpid());
    struct mq_attr attr = { .mq_maxmsg = 1, .mq_msgsize = sizeof(struct mensaje_respuesta) };
    q_cliente = mq_open(nombre_cola, O_CREAT | O_RDONLY, 0666, &attr);

    // preparar la petición
    pet.operacion = 3; // Código para DELETE
    strcpy(pet.q_cliente, nombre_cola);
    strncpy(pet.key, key, 255);

    // enviar al servidor
    q_servidor = mq_open("/SERVIDOR", O_WRONLY);
    if (q_servidor == -1) return -2; // Error de comunicaciones

    mq_send(q_servidor, (const char *)&pet, sizeof(pet), 0);

    // recibir respuesta (bloqueante)
    mq_receive(q_cliente, (char *)&res, sizeof(res), NULL);

    // limpieza
    mq_close(q_servidor);
    mq_close(q_cliente);
    mq_unlink(nombre_cola);

    return res.resultado; // Devuelve el 0 o -1 que envió el servidor
    return -1; // Placeholder
}

int exist(char *key) {
    // Implementación similar a set_value pero con operacion = 4
    mqd_t q_servidor, q_cliente;
    struct mensaje_peticion pet;
    struct mensaje_respuesta res;
    char nombre_cola[256];

    // configurar cola de respuesta única para este cliente
    sprintf(nombre_cola, "/cola_cliente_%d", getpid());
    struct mq_attr attr = { .mq_maxmsg = 1, .mq_msgsize = sizeof(struct mensaje_respuesta) };
    q_cliente = mq_open(nombre_cola, O_CREAT | O_RDONLY, 0666, &attr);

    // preparar la petición
    pet.operacion = 4; // Código para EXIST
    strcpy(pet.q_cliente, nombre_cola);
    strncpy(pet.key, key, 255);

    // enviar al servidor
    q_servidor = mq_open("/SERVIDOR", O_WRONLY);
    if (q_servidor == -1) return -2; // Error de comunicaciones

    mq_send(q_servidor, (const char *)&pet, sizeof(pet), 0);

    // recibir respuesta (bloqueante)
    mq_receive(q_cliente, (char *)&res, sizeof(res), NULL);

    // limpieza
    mq_close(q_servidor);
    mq_close(q_cliente);
    mq_unlink(nombre_cola);

    return res.resultado; // Devuelve el 0 o -1 que envió el servidor
}

int destroy() {
    // Implementación similar a set_value pero con operacion = 5
    mqd_t q_servidor, q_cliente;
    struct mensaje_peticion pet;
    struct mensaje_respuesta res;
    char nombre_cola[256];

    // configurar cola de respuesta única para este cliente
    sprintf(nombre_cola, "/cola_cliente_%d", getpid());
    struct mq_attr attr = { .mq_maxmsg = 1, .mq_msgsize = sizeof(struct mensaje_respuesta) };
    q_cliente = mq_open(nombre_cola, O_CREAT | O_RDONLY, 0666, &attr);

    // preparar la petición
    pet.operacion = 5; // Código para DESTROY
    strcpy(pet.q_cliente, nombre_cola);

    // enviar al servidor
    q_servidor = mq_open("/SERVIDOR", O_WRONLY);
    if (q_servidor == -1) return -2; // Error de comunicaciones

    mq_send(q_servidor, (const char *)&pet, sizeof(pet), 0);

    // recibir respuesta (bloqueante)
    mq_receive(q_cliente, (char *)&res, sizeof(res), NULL);

    // limpieza
    mq_close(q_servidor);
    mq_close(q_cliente);
    mq_unlink(nombre_cola);

    return res.resultado; // Devuelve el 0 o -1 que envió el servidor
    return -1; // Placeholder
}

