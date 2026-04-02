/*
 * equipaje.h
 *
 *  Created on: 2 abr 2026
 *      Author: oier.artabe
 */

#ifndef SERVER_SRC_EQUIPAJE_EQUIPAJE_H_
#define SERVER_SRC_EQUIPAJE_EQUIPAJE_H_

#define MAX_EQUIPAJES     1000
#define MAX_ID_EQ         20
#define MAX_TIPO_EQ       30   // "maleta", "mochila", "bulto", ...

typedef struct {
    char id_equipaje[MAX_ID_EQ];
    char id_pasajero[20];      // DNI del propietario
    char tipo[MAX_TIPO_EQ];
    float peso_kg;
    int  facturado;            // 0 = no facturado y 1 = facturado
} Equipaje;

int  equipaje_registrar(Equipaje lista[], int *total,
                        const char *id_eq, const char *dni_pasajero,
                        const char *tipo, float peso_kg, int facturado);

int  equipaje_eliminar(Equipaje lista[], int *total, const char *id_eq);

void equipaje_ver(const Equipaje lista[], int total);


int  equipaje_buscar(const Equipaje lista[], int total, const char *id_eq);

void equipaje_ver_por_pasajero(const Equipaje lista[], int total,
                               const char *dni_pasajero);


int  equipaje_cargar_csv(Equipaje lista[], int *total, const char *ruta);
int  equipaje_guardar_csv(const Equipaje lista[], int total, const char *ruta);

#endif /* SERVER_SRC_EQUIPAJE_EQUIPAJE_H_ */
