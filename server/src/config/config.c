#include <stdio.h>
#include <string.h>
#include "config.h"

static void quitarSalto(char *s) {
    s[strcspn(s, "\n")] = 0;
}

int cargarConfig(const char *ruta, Config *cfg) {
    FILE *f = fopen(ruta, "r");
    char linea[300];
    char clave[100];
    char valor[200];

    if (cfg == NULL) {
        return -1;
    }

    if (!f) {
        printf("No se pudo abrir config\n");
        return -1;
    }

    while (fgets(linea, sizeof(linea), f)) {
        quitarSalto(linea);

        if (linea[0] == '\0') continue;

        if (sscanf(linea, "%99[^=]=%199s", clave, valor) == 2) {
            if (strcmp(clave, "admin_user") == 0)
                strcpy(cfg->admin_user, valor);
            else if (strcmp(clave, "admin_pass") == 0)
                strcpy(cfg->admin_pass, valor);
            else if (strcmp(clave, "empleado_user") == 0)
                strcpy(cfg->empleado_user, valor);
            else if (strcmp(clave, "empleado_pass") == 0)
                strcpy(cfg->empleado_pass, valor);
            else if (strcmp(clave, "pasajero_user") == 0)
                strcpy(cfg->pasajero_user, valor);
            else if (strcmp(clave, "pasajero_pass") == 0)
                strcpy(cfg->pasajero_pass, valor);
            else if (strcmp(clave, "ruta_aeropuertos") == 0)
                strcpy(cfg->ruta_aeropuertos, valor);
            else if (strcmp(clave, "ruta_vuelos") == 0)
                strcpy(cfg->ruta_vuelos, valor);
            else if (strcmp(clave, "ruta_pasajeros") == 0)
                strcpy(cfg->ruta_pasajeros, valor);
            else if (strcmp(clave, "ruta_equipajes") == 0)
                strcpy(cfg->ruta_equipajes, valor);
            else if (strcmp(clave, "ruta_db") == 0)
                strcpy(cfg->ruta_db, valor);
        }
    }

    fclose(f);
    return 0;
}

