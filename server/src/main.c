#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "usuario/usuario.h"
#include "config/config.h"
#include "../data/db/db.h"

#define OPCION_SALIR 7

typedef enum {
    ROL_INVALIDO = -1,
    ROL_ADMINISTRADOR = 0,
    ROL_EMPLEADO = 1,
    ROL_PASAJERO = 2
} RolUsuario;

static Config configSistema;
static void *db = NULL;

void limpiarBufferEntrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

void pausarPantalla(void) {
    printf("\nPulsa ENTER para continuar...");
    getchar();
}

int leerCadena(char *buffer, int tam) {
    if (!buffer || tam <= 0) return -1;

    if (!fgets(buffer, tam, stdin)) return -1;

    buffer[strcspn(buffer, "\n")] = '\0';
    return 0;
}

int leerEntero(int *valor) {
    char buffer[64];

    if (!valor) return -1;

    if (!fgets(buffer, sizeof(buffer), stdin)) return -1;

    return (sscanf(buffer, "%d", valor) == 1) ? 0 : -1;
}

int confirmarAccion(const char *msg) {
    char r[4];

    printf("%s (s/n): ", msg);

    if (leerCadena(r, sizeof(r)) != 0) return 0;

    return (r[0] == 's' || r[0] == 'S');
}

void mostrarCabecera(const char *t) {
    printf("\n==============================\n");
    printf("%s\n", t);
    printf("==============================\n");
}

int loginSistema(const Config *cfg) {
    char usuario[100];
    char password[100];

    mostrarCabecera("LOGIN");

    printf("Usuario: ");
    if (leerCadena(usuario, sizeof(usuario)) != 0) return ROL_INVALIDO;

    printf("Password: ");
    if (leerCadena(password, sizeof(password)) != 0) return ROL_INVALIDO;

    if (strcmp(usuario, cfg->admin_user) == 0 &&
        strcmp(password, cfg->admin_pass) == 0) {
        return ROL_ADMINISTRADOR;
    }

    if (strcmp(usuario, cfg->empleado_user) == 0 &&
        strcmp(password, cfg->empleado_pass) == 0) {
        return ROL_EMPLEADO;
    }

    if (strcmp(usuario, cfg->pasajero_user) == 0 &&
        strcmp(password, cfg->pasajero_pass) == 0) {
        return ROL_PASAJERO;
    }

    return ROL_INVALIDO;
}

void mostrarMenuAdmin(void) {
    mostrarCabecera("MENU ADMIN");

    printf("1. Cargar CSV en BD\n");
    printf("2. Ver aeropuertos\n");
    printf("3. Ver vuelos\n");
    printf("4. Ver pasajeros\n");
    printf("5. Ver equipaje\n");
    printf("6. Cambiar usuario\n");
    printf("7. Salir\n");
}

int leerOpcion(int *op) {
    printf("\nOpcion: ");
    return leerEntero(op);
}

void accionCargarCSV(void) {
    int a = db_cargar_aeropuertos_csv(db, configSistema.ruta_aeropuertos);
    int v = db_cargar_vuelos_csv(db, configSistema.ruta_vuelos);
    int u = db_cargar_usuarios_csv(db, configSistema.ruta_pasajeros);
    int e = db_cargar_equipajes_csv(db, configSistema.ruta_equipajes);

    printf("\nCargados en BD:\n");
    printf("Aeropuertos: %d\n", a);
    printf("Vuelos: %d\n", v);
    printf("Usuarios: %d\n", u);
    printf("Equipajes: %d\n", e);
}

void ejecutarMenuEmpleado(void) {
    int op = 0;

    do {
        mostrarCabecera("MENU EMPLEADO");

        printf("1. Ver vuelos (BD)\n");
        printf("2. Cambiar usuario\n");

        if (leerOpcion(&op) != 0) {
            printf("Opcion invalida\n");
            pausarPantalla();
            continue;
        }

        switch (op) {
            case 1:
                db_vuelo_listar(db);
                break;

            case 2:
                printf("Cambiando usuario...\n");
                break;

            default:
                printf("Opcion no valida\n");
                break;
        }

        if (op != 2) pausarPantalla();

    } while (op != 2);
}

void ejecutarMenuPasajero(void) {
    int op = 0;

    do {
        mostrarCabecera("MENU PASAJERO");

        printf("1. Ver vuelos (BD)\n");
        printf("2. Cambiar usuario\n");

        if (leerOpcion(&op) != 0) {
            printf("Opcion invalida\n");
            pausarPantalla();
            continue;
        }

        switch (op) {
            case 1:
                db_vuelo_listar(db);
                break;

            case 2:
                printf("Cambiando usuario...\n");
                break;

            default:
                printf("Opcion no valida\n");
                break;
        }

        if (op != 2) pausarPantalla();

    } while (op != 2);
}

void ejecutarMenuAdmin(void) {
    int opcion = 0;

    do {
        mostrarMenuAdmin();

        if (leerOpcion(&opcion) != 0) {
            printf("Opcion invalida\n");
            pausarPantalla();
            continue;
        }

        switch (opcion) {
            case 1:
                accionCargarCSV();
                break;

            case 2:
                db_aeropuerto_listar(db);
                break;

            case 3:
                db_vuelo_listar(db);
                break;

            case 4:
                db_usuario_listar(db);
                break;

            case 5:
                db_equipaje_listar(db);
                break;

            case 6:
                printf("Cambiando usuario...\n");
                return;

            case 7:
                if (confirmarAccion("¿Salir del programa?")) {
                    db_cerrar(db);
                    exit(0);
                }
                break;

            default:
                printf("Opcion no implementada\n");
                break;
        }

        if (opcion != OPCION_SALIR) {
            pausarPantalla();
        }

    } while (opcion != OPCION_SALIR);
}

int main(void) {
    int rol;

    if (cargarConfig("config.txt", &configSistema) != 0) {
        printf("Error cargando config.txt\n");
        return 1;
    }

    printf("[INFO] Config cargada correctamente\n");
    printf("[INFO] Base de datos: %s\n", configSistema.ruta_db);

    db = db_abrir(configSistema.ruta_db);

    if (!db) {
        printf("Error abriendo BD\n");
        return 1;
    }

    db_activar_fk(db);
    db_inicializar(db);

    while (1) {
        rol = loginSistema(&configSistema);

        if (rol == ROL_INVALIDO) {
            printf("Login incorrecto\n");
            pausarPantalla();
            continue;
        }

        if (rol == ROL_ADMINISTRADOR) {
            ejecutarMenuAdmin();
        }
        else if (rol == ROL_EMPLEADO) {
            ejecutarMenuEmpleado();
        }
        else if (rol == ROL_PASAJERO) {
            ejecutarMenuPasajero();
        }
    }

    db_cerrar(db);
    return 0;
}