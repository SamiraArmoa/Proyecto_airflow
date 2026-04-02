/*
 * aeropuerto.c
 *
 *  Created on: 2 abr 2026
 *      Author: oier.artabe
 */

#include "aeropuerto.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static void trim_newline(char *s) {
    size_t len = strlen(s);
    if (len > 0 && (s[len-1] == '\n' || s[len-1] == '\r'))
        s[len-1] = '\0';
    if (len > 1 && (s[len-2] == '\r'))
        s[len-2] = '\0';
}


int aeropuerto_crear(Aeropuerto lista[], int *total,
                     const char *codigo, const char *nombre,
                     const char *ciudad, const char *pais,
                     int num_pistas)
{
    if (*total >= MAX_AEROPUERTOS) {
        fprintf(stderr, "[Aeropuerto] Lista llena.\n");
        return -1;
    }
    if (aeropuerto_buscar(lista, *total, codigo) != -1) {
        fprintf(stderr, "[Aeropuerto] Código '%s' ya existe.\n", codigo);
        return -1;
    }

    Aeropuerto *a = &lista[*total];
    strncpy(a->codigo,    codigo,  MAX_CODIGO  - 1);  a->codigo[MAX_CODIGO-1]   = '\0';
    strncpy(a->nombre,    nombre,  MAX_NOMBRE  - 1);  a->nombre[MAX_NOMBRE-1]   = '\0';
    strncpy(a->ciudad,    ciudad,  MAX_CIUDAD  - 1);  a->ciudad[MAX_CIUDAD-1]   = '\0';
    strncpy(a->pais,      pais,    MAX_PAIS    - 1);  a->pais[MAX_PAIS-1]       = '\0';
    a->num_pistas = num_pistas;
    (*total)++;
    return 0;
}

int aeropuerto_eliminar(Aeropuerto lista[], int *total, const char *codigo)
{
    int idx = aeropuerto_buscar(lista, *total, codigo);
    if (idx == -1) {
        fprintf(stderr, "[Aeropuerto] Código '%s' no encontrado.\n", codigo);
        return -1;
    }

    for (int i = idx; i < (*total) - 1; i++)
        lista[i] = lista[i + 1];
    (*total)--;
    return 0;
}

int aeropuerto_actualizar(Aeropuerto lista[], int total,
                          const char *codigo, const char *nuevo_nombre,
                          const char *nueva_ciudad, const char *nuevo_pais,
                          int nuevo_num_pistas)
{
    int idx = aeropuerto_buscar(lista, total, codigo);
    if (idx == -1) {
        fprintf(stderr, "[Aeropuerto] Código '%s' no encontrado.\n", codigo);
        return -1;
    }
    Aeropuerto *a = &lista[idx];
    if (nuevo_nombre  && nuevo_nombre[0]  != '\0')
        strncpy(a->nombre,  nuevo_nombre,  MAX_NOMBRE - 1),  a->nombre[MAX_NOMBRE-1]  = '\0';
    if (nueva_ciudad  && nueva_ciudad[0]  != '\0')
        strncpy(a->ciudad,  nueva_ciudad,  MAX_CIUDAD - 1),  a->ciudad[MAX_CIUDAD-1]  = '\0';
    if (nuevo_pais    && nuevo_pais[0]    != '\0')
        strncpy(a->pais,    nuevo_pais,    MAX_PAIS   - 1),  a->pais[MAX_PAIS-1]      = '\0';
    if (nuevo_num_pistas > 0)
        a->num_pistas = nuevo_num_pistas;
    return 0;
}

void aeropuerto_ver(const Aeropuerto lista[], int total)
{
    if (total == 0) {
        printf("No hay aeropuertos registrados.\n");
        return;
    }
    printf("\n%-8s %-40s %-25s %-20s %s\n",
           "Código", "Nombre", "Ciudad", "País", "Pistas");
    printf("%s\n", "-----------------------------------------------------------"
                   "-----------------------------");
    for (int i = 0; i < total; i++) {
        const Aeropuerto *a = &lista[i];
        printf("%-8s %-40s %-25s %-20s %d\n",
               a->codigo, a->nombre, a->ciudad, a->pais, a->num_pistas);
    }
    printf("\nTotal: %d aeropuerto(s).\n", total);
}



int aeropuerto_buscar(const Aeropuerto lista[], int total, const char *codigo)
{
    for (int i = 0; i < total; i++)
        if (strcmp(lista[i].codigo, codigo) == 0)
            return i;
    return -1;
}



int aeropuerto_cargar_csv(Aeropuerto lista[], int *total, const char *ruta)
{
    FILE *f = fopen(ruta, "r");
    if (!f) {
        fprintf(stderr, "[Aeropuerto] No se pudo abrir '%s'.\n", ruta);
        return -1;
    }

    char linea[512];

    if (fgets(linea, sizeof(linea), f) == NULL) { fclose(f); return 0; }

    int leidos = 0;
    while (fgets(linea, sizeof(linea), f)) {
        trim_newline(linea);
        if (linea[0] == '\0') continue;

        char codigo[MAX_CODIGO], nombre[MAX_NOMBRE];
        char ciudad[MAX_CIUDAD], pais[MAX_PAIS];
        int  pistas = 0;


        char *tok = strtok(linea, ",");
        if (!tok) continue;
        strncpy(codigo, tok, MAX_CODIGO - 1); codigo[MAX_CODIGO-1] = '\0';

        tok = strtok(NULL, ","); if (!tok) continue;
        strncpy(nombre, tok, MAX_NOMBRE - 1); nombre[MAX_NOMBRE-1] = '\0';

        tok = strtok(NULL, ","); if (!tok) continue;
        strncpy(ciudad, tok, MAX_CIUDAD - 1); ciudad[MAX_CIUDAD-1] = '\0';

        tok = strtok(NULL, ","); if (!tok) continue;
        strncpy(pais, tok, MAX_PAIS - 1); pais[MAX_PAIS-1] = '\0';

        tok = strtok(NULL, ",");
        if (tok) pistas = atoi(tok);

        if (aeropuerto_crear(lista, total, codigo, nombre, ciudad, pais, pistas) == 0)
            leidos++;
    }
    fclose(f);
    printf("[Aeropuerto] %d registros cargados desde '%s'.\n", leidos, ruta);
    return leidos;
}

int aeropuerto_guardar_csv(const Aeropuerto lista[], int total, const char *ruta)
{
    FILE *f = fopen(ruta, "w");
    if (!f) {
        fprintf(stderr, "[Aeropuerto] No se pudo escribir '%s'.\n", ruta);
        return -1;
    }
    fprintf(f, "codigo,nombre,ciudad,pais,num_pistas\n");
    for (int i = 0; i < total; i++) {
        const Aeropuerto *a = &lista[i];
        fprintf(f, "%s,%s,%s,%s,%d\n",
                a->codigo, a->nombre, a->ciudad, a->pais, a->num_pistas);
    }
    fclose(f);
    return 0;
}
