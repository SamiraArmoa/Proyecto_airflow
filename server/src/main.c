#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "usuario/usuario.h"
#include "aeropuerto/aeropuerto.h"
#include "vuelo/vuelo.h"
#include "pasajero/pasajero.h"
#include "equipaje/equipaje.h"
#include "config/config.h"

#define OPCION_MIN 1
#define OPCION_SALIR 21

typedef enum {
    ROL_INVALIDO = -1,
    ROL_ADMINISTRADOR = 0,
    ROL_EMPLEADO = 1,
    ROL_PASAJERO = 2
} RolUsuario;

static Config configSistema;

static Aeropuerto listaAeropuertos[MAX_AEROPUERTOS];
static int totalAeropuertos = 0;

static Vuelo listaVuelos[MAX_VUELOS];
static int totalVuelos = 0;

static Pasajero listaPasajeros[MAX_PASAJEROS];
static int totalPasajeros = 0;

static Equipaje listaEquipaje[MAX_EQUIPAJES];
static int totalEquipaje = 0;



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

int leerFlotante(float *valor) {
    char buffer[64];
    if (!valor) return -1;
    if (!fgets(buffer, sizeof(buffer), stdin)) return -1;
    return (sscanf(buffer, "%f", valor) == 1) ? 0 : -1;
}

int confirmarAccion(const char *msg) {
    char r[4];
    printf("%s (s/n): ", msg);
    if (leerCadena(r, sizeof(r)) != 0) return 0;
    return (r[0] == 's' || r[0] == 'S');
}

void mostrarCabecera(const char *t) {
    printf("\n===\n");
    printf("%s\n", t);
    printf("===\n");
}


int loginSistema(void) {
    char usuario[100];
    char password[100];

    mostrarCabecera("LOGIN");

    printf("Usuario: ");
    if (leerCadena(usuario, sizeof(usuario)) != 0) return ROL_INVALIDO;

    printf("Password: ");
    if (leerCadena(password, sizeof(password)) != 0) return ROL_INVALIDO;

    if (strcmp(usuario, "admin") == 0 && strcmp(password, "admin") == 0)
        return ROL_ADMINISTRADOR;

    if (strcmp(usuario, "empleado") == 0 && strcmp(password, "empleado") == 0)
        return ROL_EMPLEADO;

    if (strcmp(usuario, "pasajero") == 0 && strcmp(password, "pasajero") == 0)
        return ROL_PASAJERO;

    return ROL_INVALIDO;
}


void mostrarMenu(void) {
    mostrarCabecera("MENU ADMIN");

    printf("1. Cargar CSV\n");
    printf("2. Crear aeropuerto\n");
    printf("3. Eliminar aeropuerto\n");
    printf("4. Actualizar aeropuerto\n");
    printf("5. Ver aeropuertos\n");
    printf("6. Crear vuelo\n");
    printf("7. Eliminar vuelo\n");
    printf("8. Actualizar vuelo\n");
    printf("9. Ver vuelos\n");
    printf("10. Crear pasajero\n");
    printf("11. Eliminar pasajero\n");
    printf("12. Actualizar pasajero\n");
    printf("13. Ver pasajeros\n");
    printf("14. Registrar equipaje\n");
    printf("15. Eliminar equipaje\n");
    printf("16. Ver equipaje\n");
    printf("17. Asignar pasajero a vuelo\n");
    printf("18. Ver pasajeros por vuelo\n");
    printf("19. Ver equipaje por pasajero\n");
    printf("20. Cambiar usuario\n");
    printf("21. Salir\n");
}

int leerOpcion(int *op) {
    printf("\nOpcion: ");
    return leerEntero(op);
}



void accionCargarCSV(void) {
    aeropuerto_cargar_csv(listaAeropuertos, &totalAeropuertos, configSistema.ruta_aeropuertos);
    vuelo_cargar_csv(listaVuelos, &totalVuelos, configSistema.ruta_vuelos);
    pasajero_cargar_csv(listaPasajeros, &totalPasajeros, configSistema.ruta_pasajeros);
    equipaje_cargar_csv(listaEquipaje, &totalEquipaje, configSistema.ruta_equipajes);

    printf("Datos cargados correctamente.\n");
}


void guardarTodo(void) {
    aeropuerto_guardar_csv(listaAeropuertos, totalAeropuertos, configSistema.ruta_aeropuertos);
    vuelo_guardar_csv(listaVuelos, totalVuelos, configSistema.ruta_vuelos);
    pasajero_guardar_csv(listaPasajeros, totalPasajeros, configSistema.ruta_pasajeros);
    equipaje_guardar_csv(listaEquipaje, totalEquipaje, configSistema.ruta_equipajes);

    printf("Datos guardados correctamente.\n");
}



void ejecutarMenuEmpleado(void) {
    int op;
    do {
        mostrarCabecera("MENU EMPLEADO");
        printf("1. Ver vuelos\n2. Ver pasajeros\n3. Salir\n");

        if (leerOpcion(&op) != 0) continue;

        if (op == 1) vuelo_ver(listaVuelos, totalVuelos);
        if (op == 2) pasajero_ver(listaPasajeros, totalPasajeros);

        if (op != 3) pausarPantalla();

    } while (op != 3);
}



void ejecutarMenuPasajero(void) {
    int op;
    do {
        mostrarCabecera("MENU PASAJERO");
        printf("1. Ver vuelos\n2. Salir\n");

        if (leerOpcion(&op) != 0) continue;

        if (op == 1) vuelo_ver(listaVuelos, totalVuelos);

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

    while (1) {

        rol = loginSistema();

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

                    case 1: accionCargarCSV(); break;

                    case 5: aeropuerto_ver(listaAeropuertos, totalAeropuertos); break;
                    case 9: vuelo_ver(listaVuelos, totalVuelos); break;
                    case 13: pasajero_ver(listaPasajeros, totalPasajeros); break;
                    case 16: equipaje_ver(listaEquipaje, totalEquipaje); break;

                    case 20:
                        guardarTodo();
                        break;

                    case 21:
                        if (confirmarAccion("¿Salir del programa?")) {
                            guardarTodo();
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

    return 0;
}
