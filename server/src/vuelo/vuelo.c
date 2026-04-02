/*
 * vuelo.c
 *
 *  Created on: 2 abr 2026
 *      Author: oier.artabe
 */


#include "vuelo.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static void trim_newline(char *s) {
    size_t len = strlen(s);
    if (len > 0 && (s[len-1] == '\n' || s[len-1] == '\r')) s[len-1] = '\0';
    if (len > 1 && (s[len-2] == '\r'))                      s[len-2] = '\0';
}


int vuelo_crear(Vuelo lista[], int *total,
                const char *id, const char *aerolinea,
                const char *origen, const char *destino,
                const char *fecha, const char *hora_salida,
                const char *hora_llegada, int capacidad)
{
    if (*total >= MAX_VUELOS) {
        fprintf(stderr, "[Vuelo] Lista llena.\n");
        return -1;
    }
    if (vuelo_buscar(lista, *total, id) != -1) {
        fprintf(stderr, "[Vuelo] ID '%s' ya existe.\n", id);
        return -1;
    }
    Vuelo *v = &lista[*total];
    strncpy(v->id_vuelo,      id,           MAX_ID_VUELO  - 1); v->id_vuelo[MAX_ID_VUELO-1]   = '\0';
    strncpy(v->aerolinea,     aerolinea,    MAX_AEROLINEA - 1); v->aerolinea[MAX_AEROLINEA-1] = '\0';
    strncpy(v->origen,        origen,       MAX_CODIGO    - 1); v->origen[MAX_CODIGO-1]        = '\0';
    strncpy(v->destino,       destino,      MAX_CODIGO    - 1); v->destino[MAX_CODIGO-1]       = '\0';
    strncpy(v->fecha,         fecha,        MAX_FECHA     - 1); v->fecha[MAX_FECHA-1]          = '\0';
    strncpy(v->hora_salida,   hora_salida,  MAX_HORA      - 1); v->hora_salida[MAX_HORA-1]     = '\0';
    strncpy(v->hora_llegada,  hora_llegada, MAX_HORA      - 1); v->hora_llegada[MAX_HORA-1]    = '\0';
    v->capacidad           = capacidad;
    v->asientos_disponibles = capacidad;
    (*total)++;
    return 0;
}

int vuelo_eliminar(Vuelo lista[], int *total, const char *id)
{
    int idx = vuelo_buscar(lista, *total, id);
    if (idx == -1) {
        fprintf(stderr, "[Vuelo] ID '%s' no encontrado.\n", id);
        return -1;
    }
    for (int i = idx; i < (*total) - 1; i++)
        lista[i] = lista[i + 1];
    (*total)--;
    return 0;
}

int vuelo_actualizar(Vuelo lista[], int total,
                     const char *id, const char *nueva_hora_salida,
                     const char *nueva_hora_llegada, int nueva_capacidad)
{
    int idx = vuelo_buscar(lista, total, id);
    if (idx == -1) {
        fprintf(stderr, "[Vuelo] ID '%s' no encontrado.\n", id);
        return -1;
    }
    Vuelo *v = &lista[idx];
    if (nueva_hora_salida  && nueva_hora_salida[0]  != '\0')
        strncpy(v->hora_salida,  nueva_hora_salida,  MAX_HORA - 1), v->hora_salida[MAX_HORA-1]  = '\0';
    if (nueva_hora_llegada && nueva_hora_llegada[0] != '\0')
        strncpy(v->hora_llegada, nueva_hora_llegada, MAX_HORA - 1), v->hora_llegada[MAX_HORA-1] = '\0';
    if (nueva_capacidad > 0) {
        int usados = v->capacidad - v->asientos_disponibles;
        v->capacidad = nueva_capacidad;
        v->asientos_disponibles = nueva_capacidad - usados;
        if (v->asientos_disponibles < 0) v->asientos_disponibles = 0;
    }
    return 0;
}

void vuelo_ver(const Vuelo lista[], int total)
{
    if (total == 0) {
        printf("No hay vuelos registrados.\n");
        return;
    }
    printf("\n%-12s %-20s %-6s %-6s %-12s %-7s %-7s %s/%s\n",
           "ID", "Aerolínea", "Orig", "Dest", "Fecha",
           "Salida", "Llegada", "Cap", "Disp");
    printf("%-100s\n", "----------------------------------------------------------------------"
                       "--------------------------------");
    for (int i = 0; i < total; i++) {
        const Vuelo *v = &lista[i];
        printf("%-12s %-20s %-6s %-6s %-12s %-7s %-7s %d/%d\n",
               v->id_vuelo, v->aerolinea, v->origen, v->destino,
               v->fecha, v->hora_salida, v->hora_llegada,
               v->capacidad, v->asientos_disponibles);
    }
    printf("\nTotal: %d vuelo(s).\n", total);
}


int vuelo_buscar(const Vuelo lista[], int total, const char *id)
{
    for (int i = 0; i < total; i++)
        if (strcmp(lista[i].id_vuelo, id) == 0)
            return i;
    return -1;
}


int vuelo_reservar_asiento(Vuelo lista[], int total, const char *id)
{
    int idx = vuelo_buscar(lista, total, id);
    if (idx == -1) return -1;
    if (lista[idx].asientos_disponibles <= 0) {
        fprintf(stderr, "[Vuelo] Vuelo '%s' sin asientos disponibles.\n", id);
        return -1;
    }
    lista[idx].asientos_disponibles--;
    return 0;
}

int vuelo_liberar_asiento(Vuelo lista[], int total, const char *id)
{
    int idx = vuelo_buscar(lista, total, id);
    if (idx == -1) return -1;
    if (lista[idx].asientos_disponibles < lista[idx].capacidad)
        lista[idx].asientos_disponibles++;
    return 0;
}


int vuelo_cargar_csv(Vuelo lista[], int *total, const char *ruta)
{
    FILE *f = fopen(ruta, "r");
    if (!f) {
        fprintf(stderr, "[Vuelo] No se pudo abrir '%s'.\n", ruta);
        return -1;
    }
    char linea[512];
    if (fgets(linea, sizeof(linea), f) == NULL) { fclose(f); return 0; }

    int leidos = 0;
    while (fgets(linea, sizeof(linea), f)) {
        trim_newline(linea);
        if (linea[0] == '\0') continue;

        char id[MAX_ID_VUELO], aerolinea[MAX_AEROLINEA];
        char origen[MAX_CODIGO], destino[MAX_CODIGO];
        char fecha[MAX_FECHA], h_sal[MAX_HORA], h_lle[MAX_HORA];
        int  cap = 0;

        char *tok = strtok(linea, ","); if (!tok) continue;
        strncpy(id, tok, MAX_ID_VUELO - 1); id[MAX_ID_VUELO-1] = '\0';

        tok = strtok(NULL, ","); if (!tok) continue;
        strncpy(aerolinea, tok, MAX_AEROLINEA - 1); aerolinea[MAX_AEROLINEA-1] = '\0';

        tok = strtok(NULL, ","); if (!tok) continue;
        strncpy(origen, tok, MAX_CODIGO - 1); origen[MAX_CODIGO-1] = '\0';

        tok = strtok(NULL, ","); if (!tok) continue;
        strncpy(destino, tok, MAX_CODIGO - 1); destino[MAX_CODIGO-1] = '\0';

        tok = strtok(NULL, ","); if (!tok) continue;
        strncpy(fecha, tok, MAX_FECHA - 1); fecha[MAX_FECHA-1] = '\0';

        tok = strtok(NULL, ","); if (!tok) continue;
        strncpy(h_sal, tok, MAX_HORA - 1); h_sal[MAX_HORA-1] = '\0';

        tok = strtok(NULL, ","); if (!tok) continue;
        strncpy(h_lle, tok, MAX_HORA - 1); h_lle[MAX_HORA-1] = '\0';

        tok = strtok(NULL, ",");
        if (tok) cap = atoi(tok);

        if (vuelo_crear(lista, total, id, aerolinea, origen, destino,
                        fecha, h_sal, h_lle, cap) == 0)
            leidos++;
    }
    fclose(f);
    printf("[Vuelo] %d registros cargados desde '%s'.\n", leidos, ruta);
    return leidos;
}

int vuelo_guardar_csv(const Vuelo lista[], int total, const char *ruta)
{
    FILE *f = fopen(ruta, "w");
    if (!f) {
        fprintf(stderr, "[Vuelo] No se pudo escribir '%s'.\n", ruta);
        return -1;
    }
    fprintf(f, "id_vuelo,aerolinea,origen,destino,fecha,hora_salida,hora_llegada,capacidad\n");
    for (int i = 0; i < total; i++) {
        const Vuelo *v = &lista[i];
        fprintf(f, "%s,%s,%s,%s,%s,%s,%s,%d\n",
                v->id_vuelo, v->aerolinea, v->origen, v->destino,
                v->fecha, v->hora_salida, v->hora_llegada, v->capacidad);
    }
    fclose(f);
    return 0;
}
