
#include "equipaje.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static void trim_newline(char *s) {
    size_t len = strlen(s);
    if (len > 0 && (s[len-1] == '\n' || s[len-1] == '\r')) s[len-1] = '\0';
    if (len > 1 && (s[len-2] == '\r'))                      s[len-2] = '\0';
}


int equipaje_registrar(Equipaje lista[], int *total,
                       const char *id_eq, const char *dni_pasajero,
                       const char *tipo, float peso_kg, int facturado)
{
    if (*total >= MAX_EQUIPAJES) {
        fprintf(stderr, "[Equipaje] Lista llena.\n");
        return -1;
    }
    if (equipaje_buscar(lista, *total, id_eq) != -1) {
        fprintf(stderr, "[Equipaje] ID '%s' ya existe.\n", id_eq);
        return -1;
    }
    Equipaje *e = &lista[*total];
    strncpy(e->id_equipaje,  id_eq,        MAX_ID_EQ  - 1); e->id_equipaje[MAX_ID_EQ-1]   = '\0';
    strncpy(e->id_pasajero,  dni_pasajero, 19);              e->id_pasajero[19]             = '\0';
    strncpy(e->tipo,         tipo,         MAX_TIPO_EQ - 1); e->tipo[MAX_TIPO_EQ-1]         = '\0';
    e->peso_kg   = peso_kg;
    e->facturado = facturado;
    (*total)++;
    return 0;
}

int equipaje_eliminar(Equipaje lista[], int *total, const char *id_eq)
{
    int idx = equipaje_buscar(lista, *total, id_eq);
    if (idx == -1) {
        fprintf(stderr, "[Equipaje] ID '%s' no encontrado.\n", id_eq);
        return -1;
    }
    for (int i = idx; i < (*total) - 1; i++)
        lista[i] = lista[i + 1];
    (*total)--;
    return 0;
}

void equipaje_ver(const Equipaje lista[], int total)
{
    if (total == 0) {
        printf("No hay equipaje registrado.\n");
        return;
    }
    printf("\n%-12s %-12s %-12s %-8s %s\n",
           "ID Equipaje", "DNI Pasajero", "Tipo", "Peso(kg)", "Facturado");
    printf("%-70s\n", "----------------------------------------------------------------------");
    for (int i = 0; i < total; i++) {
        const Equipaje *e = &lista[i];
        printf("%-12s %-12s %-12s %-8.2f %s\n",
               e->id_equipaje, e->id_pasajero, e->tipo,
               e->peso_kg, e->facturado ? "Sí" : "No");
    }
    printf("\nTotal: %d bulto(s).\n", total);
}


int equipaje_buscar(const Equipaje lista[], int total, const char *id_eq)
{
    for (int i = 0; i < total; i++)
        if (strcmp(lista[i].id_equipaje, id_eq) == 0)
            return i;
    return -1;
}



void equipaje_ver_por_pasajero(const Equipaje lista[], int total,
                               const char *dni_pasajero)
{
    int encontrado = 0;
    printf("\nEquipaje del pasajero '%s':\n", dni_pasajero);
    printf("%-12s %-12s %-8s %s\n", "ID Equipaje", "Tipo", "Peso(kg)", "Facturado");
    printf("%-55s\n", "-------------------------------------------------------");
    for (int i = 0; i < total; i++) {
        if (strcmp(lista[i].id_pasajero, dni_pasajero) == 0) {
            const Equipaje *e = &lista[i];
            printf("%-12s %-12s %-8.2f %s\n",
                   e->id_equipaje, e->tipo, e->peso_kg,
                   e->facturado ? "Sí" : "No");
            encontrado++;
        }
    }
    if (!encontrado)
        printf("Sin equipaje registrado.\n");
    else
        printf("\n%d bulto(s).\n", encontrado);
}


int equipaje_cargar_csv(Equipaje lista[], int *total, const char *ruta)
{
    FILE *f = fopen(ruta, "r");
    if (!f) {
        fprintf(stderr, "[Equipaje] No se pudo abrir '%s'.\n", ruta);
        return -1;
    }
    char linea[512];
    if (fgets(linea, sizeof(linea), f) == NULL) { fclose(f); return 0; }

    int leidos = 0;
    while (fgets(linea, sizeof(linea), f)) {
        trim_newline(linea);
        if (linea[0] == '\0') continue;

        char id_eq[MAX_ID_EQ], dni[20], tipo[MAX_TIPO_EQ];
        float peso = 0.0f;
        int   fact = 0;

        char *tok = strtok(linea, ","); if (!tok) continue;
        strncpy(id_eq, tok, MAX_ID_EQ - 1); id_eq[MAX_ID_EQ-1] = '\0';

        tok = strtok(NULL, ","); if (!tok) continue;
        strncpy(dni, tok, 19); dni[19] = '\0';

        tok = strtok(NULL, ","); if (!tok) continue;
        strncpy(tipo, tok, MAX_TIPO_EQ - 1); tipo[MAX_TIPO_EQ-1] = '\0';

        tok = strtok(NULL, ",");
        if (tok) peso = (float)atof(tok);

        tok = strtok(NULL, ",");
        if (tok) fact = atoi(tok);

        if (equipaje_registrar(lista, total, id_eq, dni, tipo, peso, fact) == 0)
            leidos++;
    }
    fclose(f);
    printf("[Equipaje] %d registros cargados desde '%s'.\n", leidos, ruta);
    return leidos;
}

int equipaje_guardar_csv(const Equipaje lista[], int total, const char *ruta)
{
    FILE *f = fopen(ruta, "w");
    if (!f) {
        fprintf(stderr, "[Equipaje] No se pudo escribir '%s'.\n", ruta);
        return -1;
    }
    fprintf(f, "id_equipaje,dni_pasajero,tipo,peso_kg,facturado\n");
    for (int i = 0; i < total; i++) {
        const Equipaje *e = &lista[i];
        fprintf(f, "%s,%s,%s,%.2f,%d\n",
                e->id_equipaje, e->id_pasajero, e->tipo,
                e->peso_kg, e->facturado);
    }
    fclose(f);
    return 0;
}

