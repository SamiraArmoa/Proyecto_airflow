/*
 * vuelo.h
 *
 *  Created on: 2 abr 2026
 *      Author: oier.artabe
 */

#ifndef SERVER_SRC_VUELO_VUELO_H_
#define SERVER_SRC_VUELO_VUELO_H_

#define MAX_VUELOS       200
#define MAX_ID_VUELO     20
#define MAX_FECHA        20
#define MAX_HORA         10
#define MAX_AEROLINEA    80
#define MAX_CODIGO       10

typedef struct {
    char id_vuelo[MAX_ID_VUELO];
    char aerolinea[MAX_AEROLINEA];
    char origen[MAX_CODIGO];
    char destino[MAX_CODIGO];
    char fecha[MAX_FECHA];
    char hora_salida[MAX_HORA];
    char hora_llegada[MAX_HORA];
    int capacidad;
    int asientos_disponibles;
} Vuelo;

int vuelo_crear(Vuelo lista[], int *total,
                const char *id, const char *aerolinea,
                const char *origen, const char *destino,
                const char *fecha, const char *hora_salida,
                const char *hora_llegada, int capacidad);

int vuelo_eliminar(Vuelo lista[], int *total, const char *id);

int vuelo_actualizar(Vuelo lista[], int total,
                     const char *id, const char *nueva_hora_salida,
                     const char *nueva_hora_llegada, int nueva_capacidad);

void vuelo_ver(const Vuelo lista[], int total);

int vuelo_buscar(const Vuelo lista[], int total, const char *id);

int vuelo_reservar_asiento(Vuelo lista[], int total, const char *id);
int vuelo_liberar_asiento(Vuelo lista[], int total, const char *id);

int vuelo_cargar_csv(Vuelo lista[], int *total, const char *ruta);
int vuelo_guardar_csv(const Vuelo lista[], int total, const char *ruta);

#endif /* SERVER_SRC_VUELO_VUELO_H_ */