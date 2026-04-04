/*
 * pasajero.h
 *
 *  Created on: 2 abr 2026
 *      Author: oier.artabe
 */

#ifndef SERVER_PASAJERO_PASAJERO_H_
#define SERVER_PASAJERO_PASAJERO_H_

#define MAX_PASAJEROS    2000
#define MAX_DNI          20
#define MAX_NOMBRE_P     80
#define MAX_APELLIDO_P   80
#define MAX_EMAIL        120
#define MAX_TELEFONO     20
#define MAX_VUELOS_P     10

typedef struct {
    char dni[MAX_DNI];
    char nombre[MAX_NOMBRE_P];
    char apellido[MAX_APELLIDO_P];
    char email[MAX_EMAIL];
    char telefono[MAX_TELEFONO];
    // vuelos asignados
    char vuelos[MAX_VUELOS_P][20];
    int  num_vuelos;
} Pasajero;


int  pasajero_crear(Pasajero lista[], int *total,
                    const char *dni, const char *nombre,
                    const char *apellido, const char *email,
                    const char *telefono);

int  pasajero_eliminar(Pasajero lista[], int *total, const char *dni);

int  pasajero_actualizar(Pasajero lista[], int total,
                         const char *dni, const char *nuevo_email,
                         const char *nuevo_telefono);

void pasajero_ver(const Pasajero lista[], int total);


int  pasajero_buscar(const Pasajero lista[], int total, const char *dni);


int  pasajero_asignar_vuelo(Pasajero lista[], int total,
                            const char *dni, const char *id_vuelo);
int  pasajero_quitar_vuelo(Pasajero lista[], int total,
                           const char *dni, const char *id_vuelo);


int  pasajero_cargar_csv(Pasajero lista[], int *total, const char *ruta);
int  pasajero_guardar_csv(const Pasajero lista[], int total, const char *ruta);

#endif /* SERVER_PASAJERO_PASAJERO_H_ */
