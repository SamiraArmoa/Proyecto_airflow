#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "usuario/usuario.h"
#include "aeropuerto/aeropuerto.h"   /* Ajusta la ruta si tu .h está en otra carpeta */

#define OPCION_MIN 1
#define OPCION_SALIR 21

typedef enum {
    OPCION_CARGAR_CSV = 1,

    /* AEROPUERTOS */
    OPCION_CREAR_AEROPUERTO,
    OPCION_ELIMINAR_AEROPUERTO,
    OPCION_ACTUALIZAR_AEROPUERTO,
    OPCION_VER_AEROPUERTOS,

    /* VUELOS */
    OPCION_CREAR_VUELO,
    OPCION_ELIMINAR_VUELO,
    OPCION_ACTUALIZAR_VUELO,
    OPCION_VER_VUELOS,

    /* PASAJEROS */
    OPCION_CREAR_PASAJERO,
    OPCION_ELIMINAR_PASAJERO,
    OPCION_ACTUALIZAR_PASAJERO,
    OPCION_VER_PASAJEROS,

    /* EQUIPAJE */
    OPCION_REGISTRAR_EQUIPAJE,
    OPCION_ELIMINAR_EQUIPAJE,
    OPCION_VER_EQUIPAJE,

    /* OPERACIONES */
    OPCION_ASIGNAR_PASAJERO_A_VUELO,
    OPCION_REGISTRAR_EQUIPAJE_A_PASAJERO,
    OPCION_VER_PASAJEROS_POR_VUELO,
    OPCION_VER_EQUIPAJE_POR_PASAJERO,

    OPCION_MENU_SALIR
} OpcionMenu;

/* ================= DATOS GLOBALES ================= */

static Aeropuerto listaAeropuertos[MAX_AEROPUERTOS];
static int totalAeropuertos = 0;

/* ================= FUNCIONES AUXILIARES ================= */

void limpiarBufferEntrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        /* limpiar buffer */
    }
}

int leerEntero(int *valor) {
    int resultado;

    if (valor == NULL) {
        return -1;
    }

    resultado = scanf("%d", valor);

    if (resultado != 1) {
        printf("Error: introduce un numero valido.\n");
        limpiarBufferEntrada();
        return -1;
    }

    limpiarBufferEntrada();
    return 0;
}

int leerCaracter(char *caracter) {
    if (caracter == NULL) {
        return -1;
    }

    if (scanf(" %c", caracter) != 1) {
        printf("Error al leer el caracter.\n");
        limpiarBufferEntrada();
        return -1;
    }

    limpiarBufferEntrada();
    return 0;
}

int leerCadena(char *buffer, int tam) {
    if (buffer == NULL || tam <= 0) {
        return -1;
    }

    if (fgets(buffer, tam, stdin) == NULL) {
        return -1;
    }

    buffer[strcspn(buffer, "\n")] = '\0';
    return 0;
}

void pausarPantalla(void) {
    printf("\nPulsa ENTER para continuar...");
    getchar();
}

int confirmarAccion(const char *mensaje) {
    char respuesta;

    if (mensaje != NULL) {
        printf("%s (s/n): ", mensaje);
    } else {
        printf("¿Deseas continuar? (s/n): ");
    }

    if (leerCaracter(&respuesta) != 0) {
        return 0;
    }

    if (respuesta == 's' || respuesta == 'S') {
        return 1;
    }

    return 0;
}

int opcionValida(int opcion) {
    return (opcion >= OPCION_MIN && opcion <= OPCION_SALIR);
}

void mostrarCabecera(const char *titulo) {
    printf("\n========================================\n");
    if (titulo != NULL) {
        printf("%s\n", titulo);
    }
    printf("========================================\n");
}

void mostrarFuncionNoDisponible(const char *nombreFuncion) {
    if (nombreFuncion != NULL) {
        printf("%s en construccion.\n", nombreFuncion);
    } else {
        printf("Funcion en construccion.\n");
    }
}

/* ================= MENU ================= */

void mostrarMenu(void) {
    mostrarCabecera("MENU ADMINISTRADOR");

    printf("1. Cargar aeropuertos, vuelos y pasajeros desde .csv\n");

    printf("--- AEROPUERTOS ---\n");
    printf("2.  Crear aeropuertos\n");
    printf("3.  Eliminar aeropuerto\n");
    printf("4.  Actualizar aeropuerto\n");
    printf("5.  Ver aeropuertos\n");

    printf("--- VUELOS ---\n");
    printf("6.  Crear vuelo\n");
    printf("7.  Eliminar vuelo\n");
    printf("8.  Actualizar vuelo\n");
    printf("9.  Ver vuelos\n");

    printf("--- PASAJEROS ---\n");
    printf("10. Crear pasajero\n");
    printf("11. Eliminar pasajero\n");
    printf("12. Actualizar pasajero\n");
    printf("13. Ver pasajeros\n");

    printf("--- EQUIPAJE ---\n");
    printf("14. Registrar equipaje\n");
    printf("15. Eliminar equipaje\n");
    printf("16. Ver equipaje\n");

    printf("--- OPERACIONES ---\n");
    printf("17. Asignar pasajero a vuelo\n");
    printf("18. Registrar equipaje a pasajero\n");
    printf("19. Ver pasajeros por vuelo\n");
    printf("20. Ver equipaje por pasajero\n");

    printf("21. Salir\n");
}

int leerOpcionMenu(int *opcion) {
    if (opcion == NULL) {
        return -1;
    }

    mostrarMenu();
    printf("\nElige una opcion: ");

    if (leerEntero(opcion) != 0) {
        return -1;
    }

    if (!opcionValida(*opcion)) {
        printf("Error: la opcion debe estar entre %d y %d.\n", OPCION_MIN, OPCION_SALIR);
        return -1;
    }

    return 0;
}

/* ================= ACCIONES AEROPUERTOS ================= */

void accionCargarCSV(void) {
    char ruta[256];

    mostrarCabecera("CARGAR DATOS DESDE CSV");
    printf("Introduce la ruta del CSV de aeropuertos: ");

    if (leerCadena(ruta, sizeof(ruta)) != 0) {
        printf("Error al leer la ruta.\n");
        return;
    }

    if (aeropuerto_cargar_csv(listaAeropuertos, &totalAeropuertos, ruta) < 0) {
        printf("No se pudieron cargar los aeropuertos.\n");
    }
}

void accionCrearAeropuerto(void) {
    char codigo[MAX_CODIGO];
    char nombre[MAX_NOMBRE];
    char ciudad[MAX_CIUDAD];
    char pais[MAX_PAIS];
    int num_pistas;

    mostrarCabecera("CREAR AEROPUERTO");

    printf("Codigo: ");
    if (leerCadena(codigo, sizeof(codigo)) != 0) {
        printf("Error al leer el codigo.\n");
        return;
    }

    printf("Nombre: ");
    if (leerCadena(nombre, sizeof(nombre)) != 0) {
        printf("Error al leer el nombre.\n");
        return;
    }

    printf("Ciudad: ");
    if (leerCadena(ciudad, sizeof(ciudad)) != 0) {
        printf("Error al leer la ciudad.\n");
        return;
    }

    printf("Pais: ");
    if (leerCadena(pais, sizeof(pais)) != 0) {
        printf("Error al leer el pais.\n");
        return;
    }

    printf("Numero de pistas: ");
    if (leerEntero(&num_pistas) != 0) {
        return;
    }

    if (aeropuerto_crear(listaAeropuertos, &totalAeropuertos,
                         codigo, nombre, ciudad, pais, num_pistas) == 0) {
        printf("Aeropuerto creado correctamente.\n");
    } else {
        printf("No se pudo crear el aeropuerto.\n");
    }
}

void accionEliminarAeropuerto(void) {
    char codigo[MAX_CODIGO];

    mostrarCabecera("ELIMINAR AEROPUERTO");

    printf("Codigo del aeropuerto a eliminar: ");
    if (leerCadena(codigo, sizeof(codigo)) != 0) {
        printf("Error al leer el codigo.\n");
        return;
    }

    if (!confirmarAccion("¿Seguro que deseas eliminar este aeropuerto?")) {
        printf("Operacion cancelada.\n");
        return;
    }

    if (aeropuerto_eliminar(listaAeropuertos, &totalAeropuertos, codigo) == 0) {
        printf("Aeropuerto eliminado correctamente.\n");
    } else {
        printf("No se pudo eliminar el aeropuerto.\n");
    }
}

void accionActualizarAeropuerto(void) {
    char codigo[MAX_CODIGO];
    char nuevo_nombre[MAX_NOMBRE];
    char nueva_ciudad[MAX_CIUDAD];
    char nuevo_pais[MAX_PAIS];
    int nuevo_num_pistas;

    mostrarCabecera("ACTUALIZAR AEROPUERTO");

    printf("Codigo del aeropuerto a actualizar: ");
    if (leerCadena(codigo, sizeof(codigo)) != 0) {
        printf("Error al leer el codigo.\n");
        return;
    }

    printf("Nuevo nombre (ENTER para no cambiar): ");
    if (leerCadena(nuevo_nombre, sizeof(nuevo_nombre)) != 0) {
        printf("Error al leer el nombre.\n");
        return;
    }

    printf("Nueva ciudad (ENTER para no cambiar): ");
    if (leerCadena(nueva_ciudad, sizeof(nueva_ciudad)) != 0) {
        printf("Error al leer la ciudad.\n");
        return;
    }

    printf("Nuevo pais (ENTER para no cambiar): ");
    if (leerCadena(nuevo_pais, sizeof(nuevo_pais)) != 0) {
        printf("Error al leer el pais.\n");
        return;
    }

    printf("Nuevo numero de pistas (0 para no cambiar): ");
    if (leerEntero(&nuevo_num_pistas) != 0) {
        return;
    }

    if (aeropuerto_actualizar(listaAeropuertos, totalAeropuertos,
                              codigo, nuevo_nombre, nueva_ciudad,
                              nuevo_pais, nuevo_num_pistas) == 0) {
        printf("Aeropuerto actualizado correctamente.\n");
    } else {
        printf("No se pudo actualizar el aeropuerto.\n");
    }
}

void accionVerAeropuertos(void) {
    mostrarCabecera("VER AEROPUERTOS");
    aeropuerto_ver(listaAeropuertos, totalAeropuertos);
}

/* ================= RESTO DE ACCIONES ================= */

void accionCrearVuelo(void) {
    mostrarCabecera("CREAR VUELO");
    mostrarFuncionNoDisponible("Crear vuelo");
}

void accionEliminarVuelo(void) {
    mostrarCabecera("ELIMINAR VUELO");
    mostrarFuncionNoDisponible("Eliminar vuelo");
}

void accionActualizarVuelo(void) {
    mostrarCabecera("ACTUALIZAR VUELO");
    mostrarFuncionNoDisponible("Actualizar vuelo");
}

void accionVerVuelos(void) {
    mostrarCabecera("VER VUELOS");
    mostrarFuncionNoDisponible("Ver vuelos");
}

void accionCrearPasajero(void) {
    mostrarCabecera("CREAR PASAJERO");
    mostrarFuncionNoDisponible("Crear pasajero");
}

void accionEliminarPasajero(void) {
    mostrarCabecera("ELIMINAR PASAJERO");
    mostrarFuncionNoDisponible("Eliminar pasajero");
}

void accionActualizarPasajero(void) {
    mostrarCabecera("ACTUALIZAR PASAJERO");
    mostrarFuncionNoDisponible("Actualizar pasajero");
}

void accionVerPasajeros(void) {
    mostrarCabecera("VER PASAJEROS");
    mostrarFuncionNoDisponible("Ver pasajeros");
}

void accionRegistrarEquipaje(void) {
    mostrarCabecera("REGISTRAR EQUIPAJE");
    mostrarFuncionNoDisponible("Registrar equipaje");
}

void accionEliminarEquipaje(void) {
    mostrarCabecera("ELIMINAR EQUIPAJE");
    mostrarFuncionNoDisponible("Eliminar equipaje");
}

void accionVerEquipaje(void) {
    mostrarCabecera("VER EQUIPAJE");
    mostrarFuncionNoDisponible("Ver equipaje");
}

void accionAsignarPasajeroAVuelo(void) {
    mostrarCabecera("ASIGNAR PASAJERO A VUELO");
    mostrarFuncionNoDisponible("Asignar pasajero a vuelo");
}

void accionRegistrarEquipajeAPasajero(void) {
    mostrarCabecera("REGISTRAR EQUIPAJE A PASAJERO");
    mostrarFuncionNoDisponible("Registrar equipaje a pasajero");
}

void accionVerPasajerosPorVuelo(void) {
    mostrarCabecera("VER PASAJEROS POR VUELO");
    mostrarFuncionNoDisponible("Ver pasajeros por vuelo");
}

void accionVerEquipajePorPasajero(void) {
    mostrarCabecera("VER EQUIPAJE POR PASAJERO");
    mostrarFuncionNoDisponible("Ver equipaje por pasajero");
}

void accionCrearCliente(void) {
    mostrarCabecera("CREAR CLIENTE");
    printf("Iniciando creacion de cliente...\n");
    crearUsuario();
}

void accionEliminarCliente(void) {
    mostrarCabecera("ELIMINAR CLIENTE");
    printf("Iniciando eliminacion de cliente...\n");
    eliminarUsuario();
}

void accionActualizarCliente(void) {
    mostrarCabecera("ACTUALIZAR CLIENTE");
    mostrarFuncionNoDisponible("Actualizar cliente");
}

int accionSalir(void) {
    mostrarCabecera("SALIR");

    if (confirmarAccion("¿Seguro que deseas salir del sistema?")) {
        printf("Saliendo del sistema...\n");
        return 1;
    }

    printf("Salida cancelada.\n");
    return 0;
}

/* ================= CONTROLADOR ================= */

int escogerOpcion(int *opcion) {
    if (opcion == NULL) {
        return -1;
    }

    switch (*opcion) {
        case OPCION_CARGAR_CSV:
            accionCargarCSV();
            break;

        case OPCION_CREAR_AEROPUERTO:
            accionCrearAeropuerto();
            break;

        case OPCION_ELIMINAR_AEROPUERTO:
            accionEliminarAeropuerto();
            break;

        case OPCION_ACTUALIZAR_AEROPUERTO:
            accionActualizarAeropuerto();
            break;

        case OPCION_VER_AEROPUERTOS:
            accionVerAeropuertos();
            break;

        case OPCION_CREAR_VUELO:
            accionCrearVuelo();
            break;

        case OPCION_ELIMINAR_VUELO:
            accionEliminarVuelo();
            break;

        case OPCION_ACTUALIZAR_VUELO:
            accionActualizarVuelo();
            break;

        case OPCION_VER_VUELOS:
            accionVerVuelos();
            break;

        case OPCION_CREAR_PASAJERO:
            accionCrearPasajero();
            break;

        case OPCION_ELIMINAR_PASAJERO:
            accionEliminarPasajero();
            break;

        case OPCION_ACTUALIZAR_PASAJERO:
            accionActualizarPasajero();
            break;

        case OPCION_VER_PASAJEROS:
            accionVerPasajeros();
            break;

        case OPCION_REGISTRAR_EQUIPAJE:
            accionRegistrarEquipaje();
            break;

        case OPCION_ELIMINAR_EQUIPAJE:
            accionEliminarEquipaje();
            break;

        case OPCION_VER_EQUIPAJE:
            accionVerEquipaje();
            break;

        case OPCION_ASIGNAR_PASAJERO_A_VUELO:
            accionAsignarPasajeroAVuelo();
            break;

        case OPCION_REGISTRAR_EQUIPAJE_A_PASAJERO:
            accionRegistrarEquipajeAPasajero();
            break;

        case OPCION_VER_PASAJEROS_POR_VUELO:
            accionVerPasajerosPorVuelo();
            break;

        case OPCION_VER_EQUIPAJE_POR_PASAJERO:
            accionVerEquipajePorPasajero();
            break;

        case OPCION_MENU_SALIR:
            if (!accionSalir()) {
                *opcion = 0;
            }
            break;

        default:
            printf("Opcion no valida.\n");
            return -1;
    }

    return 0;
}

/* ================= MAIN ================= */

int main(void) {
    int opcion = 0;

    do {
        if (leerOpcionMenu(&opcion) != 0) {
            pausarPantalla();
            continue;
        }

        escogerOpcion(&opcion);

        if (opcion != OPCION_SALIR) {
            pausarPantalla();
        }

    } while (opcion != OPCION_SALIR);

    return 0;
}