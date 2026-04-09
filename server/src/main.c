#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "usuario/usuario.h"
#include "config/config.h"
#include "../data/db/db.h"

#define OPCION_MIN 1
#define OPCION_SALIR 21

typedef enum {
    ROL_INVALIDO = -1,
    ROL_ADMINISTRADOR = 0,
    ROL_EMPLEADO = 1,
    ROL_PASAJERO = 2
} RolUsuario;

static Config configSistema;


static void *db = NULL;


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
    printf("\n===\n%s\n===\n", t);
}



int loginSistema(const Config *cfg) {
    char usuario[100];
    char password[100];

    mostrarCabecera("LOGIN");
    printf("Usuario: ");
    if (leerCadena(usuario, sizeof(usuario)) != 0) return ROL_INVALIDO;
    printf("Password: ");
    if (leerCadena(password, sizeof(password)) != 0) return ROL_INVALIDO;

    if (strcmp(usuario, cfg->admin_user)    == 0 &&
        strcmp(password, cfg->admin_pass)   == 0) return ROL_ADMINISTRADOR;

    if (strcmp(usuario, cfg->empleado_user) == 0 &&
        strcmp(password, cfg->empleado_pass)== 0) return ROL_EMPLEADO;

    if (strcmp(usuario, cfg->pasajero_user) == 0 &&
        strcmp(password, cfg->pasajero_pass)== 0) return ROL_PASAJERO;

    return ROL_INVALIDO;
}



void mostrarMenu(void) {
    mostrarCabecera("MENU ADMIN");

    printf("1. Cargar CSV en BD\n");
    printf("5. Ver aeropuertos\n");
    printf("9. Ver vuelos\n");
    printf("13. Ver pasajeros\n");
    printf("16. Ver equipaje\n");
    printf("20. Cambiar usuario\n");
    printf("21. Salir\n");
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

    printf("Cargados en BD:\n");
    printf("Aeropuertos: %d\n", a);
    printf("Vuelos: %d\n", v);
    printf("Usuarios: %d\n", u);
    printf("Equipajes: %d\n", e);
}




void ejecutarMenuEmpleado(void) {
    int op;
    do {
        mostrarCabecera("MENU EMPLEADO");
        printf("1. Ver vuelos (BD)\n2. Salir\n");

        if (leerOpcion(&op) != 0) continue;

        if (op == 1) db_vuelo_listar(db);

        if (op != 2) pausarPantalla();

    } while (op != 2);
}

void ejecutarMenuPasajero(void) {
    int op;
    do {
        mostrarCabecera("MENU PASAJERO");
        printf("1. Ver vuelos (BD)\n2. Salir\n");

        if (leerOpcion(&op) != 0) continue;

        if (op == 1) db_vuelo_listar(db);

        if (op != 2) pausarPantalla();

    } while (op != 2);
}



int main(void) {

    int rol;
    int opcion;

    if (cargarConfig("config.txt", &configSistema) != 0) {
        printf("Error cargando config\n");
        return 1;
    }

    printf("[DEBUG] admin_user='%s' admin_pass='%s'\n",
           configSistema.admin_user, configSistema.admin_pass);
    printf("[DEBUG] ruta_db='%s'\n", configSistema.ruta_db);

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

            do {
                mostrarMenu();

                if (leerOpcion(&opcion) != 0) continue;

                switch (opcion) {

                    case 1:
                        accionCargarCSV();
                        break;

                    case 5:
                        db_aeropuerto_listar(db);
                        break;

                    case 9:
                        db_vuelo_listar(db);
                        break;

                    case 13:
                        db_usuario_listar(db);
                        break;

                    case 16:
                    	db_equipaje_listar(db);
                        break;

                    case 20:
                        break;

                    case 21:
                        if (confirmarAccion("¿Salir del programa?")) {
                            db_cerrar(db);
                            return 0;
                        }
                        break;

                    default:
                        printf("Opcion no implementada\n");
                }

                if (opcion != 20) pausarPantalla();

            } while (opcion != 20);
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
