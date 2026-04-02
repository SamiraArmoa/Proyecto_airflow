 /*
 * pasajero.c
 *
 *  Created on: 2 abr 2026
 *      Author: oier.artabe
 */


#include "pasajero.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static void trim_newline(char *s) {
    size_t len = strlen(s);
    if (len > 0 && (s[len-1] == '\n' || s[len-1] == '\r')) s[len-1] = '\0';
    if (len > 1 && (s[len-2] == '\r'))                      s[len-2] = '\0';
}


int pasajero_crear(Pasajero lista[], int *total,
                   const char *dni, const char *nombre,
                   const char *apellido, const char *email,
                   const char *telefono)
{
    if (*total >= MAX_PASAJEROS) {
        fprintf(stderr, "[Pasajero] Lista llena.\n");
        return -1;
    }
    if (pasajero_buscar(lista, *total, dni) != -1) {
        fprintf(stderr, "[Pasajero] DNI '%s' ya existe.\n", dni);
        return -1;
    }
    Pasajero *p = &lista[*total];
    strncpy(p->dni,      dni,      MAX_DNI      - 1); p->dni[MAX_DNI-1]           = '\0';
    strncpy(p->nombre,   nombre,   MAX_NOMBRE_P - 1); p->nombre[MAX_NOMBRE_P-1]   = '\0';
    strncpy(p->apellido, apellido, MAX_APELLIDO_P-1); p->apellido[MAX_APELLIDO_P-1]= '\0';
    strncpy(p->email,    email,    MAX_EMAIL    - 1); p->email[MAX_EMAIL-1]        = '\0';
    strncpy(p->telefono, telefono, MAX_TELEFONO - 1); p->telefono[MAX_TELEFONO-1]  = '\0';
    p->num_vuelos = 0;
    (*total)++;
    return 0;
}

int pasajero_eliminar(Pasajero lista[], int *total, const char *dni)
{
    int idx = pasajero_buscar(lista, *total, dni);
    if (idx == -1) {
        fprintf(stderr, "[Pasajero] DNI '%s' no encontrado.\n", dni);
        return -1;
    }
    for (int i = idx; i < (*total) - 1; i++)
        lista[i] = lista[i + 1];
    (*total)--;
    return 0;
}

int pasajero_actualizar(Pasajero lista[], int total,
                        const char *dni, const char *nuevo_email,
                        const char *nuevo_telefono)
{
    int idx = pasajero_buscar(lista, total, dni);
    if (idx == -1) {
        fprintf(stderr, "[Pasajero] DNI '%s' no encontrado.\n", dni);
        return -1;
    }
    Pasajero *p = &lista[idx];
    if (nuevo_email    && nuevo_email[0]    != '\0')
        strncpy(p->email,    nuevo_email,    MAX_EMAIL    - 1), p->email[MAX_EMAIL-1]       = '\0';
    if (nuevo_telefono && nuevo_telefono[0] != '\0')
        strncpy(p->telefono, nuevo_telefono, MAX_TELEFONO - 1), p->telefono[MAX_TELEFONO-1] = '\0';
    return 0;
}

void pasajero_ver(const Pasajero lista[], int total)
{
    if (total == 0) {
        printf("No hay pasajeros registrados.\n");
        return;
    }
    printf("\n%-12s %-15s %-20s %-30s %-15s %s\n",
           "DNI", "Nombre", "Apellido", "Email", "Teléfono", "Vuelos");
    printf("%-110s\n", "----------------------------------------------------------------------"
                       "--------------------------------------------");
    for (int i = 0; i < total; i++) {
        const Pasajero *p = &lista[i];
        // vuelos como lista separada por '|'
        char vuelos_str[256] = "-";
        if (p->num_vuelos > 0) {
            vuelos_str[0] = '\0';
            for (int j = 0; j < p->num_vuelos; j++) {
                if (j > 0) strncat(vuelos_str, "|", 255 - strlen(vuelos_str));
                strncat(vuelos_str, p->vuelos[j], 255 - strlen(vuelos_str));
            }
        }
        printf("%-12s %-15s %-20s %-30s %-15s %s\n",
               p->dni, p->nombre, p->apellido, p->email, p->telefono, vuelos_str);
    }
    printf("\nTotal: %d pasajero(s).\n", total);
}



int pasajero_buscar(const Pasajero lista[], int total, const char *dni)
{
    for (int i = 0; i < total; i++)
        if (strcmp(lista[i].dni, dni) == 0)
            return i;
    return -1;
}



int pasajero_asignar_vuelo(Pasajero lista[], int total,
                            const char *dni, const char *id_vuelo)
{
    int idx = pasajero_buscar(lista, total, dni);
    if (idx == -1) {
        fprintf(stderr, "[Pasajero] DNI '%s' no encontrado.\n", dni);
        return -1;
    }
    Pasajero *p = &lista[idx];
    if (p->num_vuelos >= MAX_VUELOS_P) {
        fprintf(stderr, "[Pasajero] Límite de vuelos por pasajero alcanzado.\n");
        return -1;
    }
    //comprobamos el duplicado
    for (int j = 0; j < p->num_vuelos; j++)
        if (strcmp(p->vuelos[j], id_vuelo) == 0) {
            fprintf(stderr, "[Pasajero] Vuelo '%s' ya asignado al pasajero.\n", id_vuelo);
            return -1;
        }
    strncpy(p->vuelos[p->num_vuelos], id_vuelo, 19);
    p->vuelos[p->num_vuelos][19] = '\0';
    p->num_vuelos++;
    return 0;
}

int pasajero_quitar_vuelo(Pasajero lista[], int total,
                           const char *dni, const char *id_vuelo)
{
    int idx = pasajero_buscar(lista, total, dni);
    if (idx == -1) return -1;
    Pasajero *p = &lista[idx];
    for (int j = 0; j < p->num_vuelos; j++) {
        if (strcmp(p->vuelos[j], id_vuelo) == 0) {
            for (int k = j; k < p->num_vuelos - 1; k++)
                strncpy(p->vuelos[k], p->vuelos[k+1], 19);
            p->num_vuelos--;
            return 0;
        }
    }
    fprintf(stderr, "[Pasajero] Vuelo '%s' no encontrado en pasajero '%s'.\n", id_vuelo, dni);
    return -1;
}



int pasajero_cargar_csv(Pasajero lista[], int *total, const char *ruta)
{
    FILE *f = fopen(ruta, "r");
    if (!f) {
        fprintf(stderr, "[Pasajero] No se pudo abrir '%s'.\n", ruta);
        return -1;
    }
    char linea[512];
    if (fgets(linea, sizeof(linea), f) == NULL) { fclose(f); return 0; }

    int leidos = 0;
    while (fgets(linea, sizeof(linea), f)) {
        trim_newline(linea);
        if (linea[0] == '\0') continue;

        char dni[MAX_DNI], nombre[MAX_NOMBRE_P];
        char apellido[MAX_APELLIDO_P], email[MAX_EMAIL], tel[MAX_TELEFONO];

        /* formato: dni,nombre,apellido,email,telefono */
        char *tok = strtok(linea, ","); if (!tok) continue;
        strncpy(dni, tok, MAX_DNI - 1); dni[MAX_DNI-1] = '\0';

        tok = strtok(NULL, ","); if (!tok) continue;
        strncpy(nombre, tok, MAX_NOMBRE_P - 1); nombre[MAX_NOMBRE_P-1] = '\0';

        tok = strtok(NULL, ","); if (!tok) continue;
        strncpy(apellido, tok, MAX_APELLIDO_P - 1); apellido[MAX_APELLIDO_P-1] = '\0';

        tok = strtok(NULL, ","); if (!tok) continue;
        strncpy(email, tok, MAX_EMAIL - 1); email[MAX_EMAIL-1] = '\0';

        tok = strtok(NULL, ","); if (!tok) { tel[0] = '\0'; }
        else { strncpy(tel, tok, MAX_TELEFONO - 1); tel[MAX_TELEFONO-1] = '\0'; }

        if (pasajero_crear(lista, total, dni, nombre, apellido, email, tel) == 0)
            leidos++;
    }
    fclose(f);
    printf("[Pasajero] %d registros cargados desde '%s'.\n", leidos, ruta);
    return leidos;
}

int pasajero_guardar_csv(const Pasajero lista[], int total, const char *ruta)
{
    FILE *f = fopen(ruta, "w");
    if (!f) {
        fprintf(stderr, "[Pasajero] No se pudo escribir '%s'.\n", ruta);
        return -1;
    }
    fprintf(f, "dni,nombre,apellido,email,telefono\n");
    for (int i = 0; i < total; i++) {
        const Pasajero *p = &lista[i];
        fprintf(f, "%s,%s,%s,%s,%s\n",
                p->dni, p->nombre, p->apellido, p->email, p->telefono);
    }
    fclose(f);
    return 0;
}


