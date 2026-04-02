/*
 * aeropuerto.h
 *
 *  Created on: 2 abr 2026
 *      Author: oier.artabe
 */

#ifndef SERVER_SRC_AEROPUERTO_AEROPUERTO_H_
#define SERVER_SRC_AEROPUERTO_AEROPUERTO_H_

#define MAX_AEROPUERTOS 100
#define MAX_CODIGO      10
#define MAX_NOMBRE      100
#define MAX_CIUDAD      100
#define MAX_PAIS        100

typedef struct {
    char codigo[MAX_CODIGO];
    char nombre[MAX_NOMBRE];
    char ciudad[MAX_CIUDAD];
    char pais[MAX_PAIS];
    int  num_pistas;
} Aeropuerto;


int  aeropuerto_crear(Aeropuerto lista[], int *total,
                      const char *codigo, const char *nombre,
                      const char *ciudad, const char *pais,
                      int num_pistas);

int  aeropuerto_eliminar(Aeropuerto lista[], int *total,
                         const char *codigo);

int  aeropuerto_actualizar(Aeropuerto lista[], int total,
                           const char *codigo, const char *nuevo_nombre,
                           const char *nueva_ciudad, const char *nuevo_pais,
                           int nuevo_num_pistas);

void aeropuerto_ver(const Aeropuerto lista[], int total);


int  aeropuerto_buscar(const Aeropuerto lista[], int total,
                       const char *codigo);


int  aeropuerto_cargar_csv(Aeropuerto lista[], int *total,
                           const char *ruta);
int  aeropuerto_guardar_csv(const Aeropuerto lista[], int total,
                            const char *ruta);

#endif /* SERVER_SRC_AEROPUERTO_AEROPUERTO_H_ */
