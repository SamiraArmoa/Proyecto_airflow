#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "usuario/usuario.h"
#include "aeropuerto/aeropuerto.h"
#include "vuelo/vuelo.h"
#include "pasajero/pasajero.h"

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

static Vuelo listaVuelos[MAX_VUELOS];
static int totalVuelos = 0;

static Pasajero listaPasajeros[MAX_PASAJEROS];
static int totalPasajeros = 0;

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

/* ================= ACCIONES CARGA CSV ================= */

void accionCargarCSV(void) {
    char rutaAeropuertos[256];
    char rutaVuelos[256];
    char rutaPasajeros[256];

    mostrarCabecera("CARGAR DATOS DESDE CSV");

    printf("Introduce la ruta del CSV de aeropuertos: ");
    if (leerCadena(rutaAeropuertos, sizeof(rutaAeropuertos)) != 0) {
        printf("Error al leer la ruta de aeropuertos.\n");
        return;
    }

    if (aeropuerto_cargar_csv(listaAeropuertos, &totalAeropuertos, rutaAeropuertos) < 0) {
        printf("No se pudieron cargar los aeropuertos.\n");
    } else {
        printf("Aeropuertos cargados correctamente.\n");
    }

    printf("Introduce la ruta del CSV de vuelos: ");
    if (leerCadena(rutaVuelos, sizeof(rutaVuelos)) != 0) {
        printf("Error al leer la ruta de vuelos.\n");
        return;
    }

    if (vuelo_cargar_csv(listaVuelos, &totalVuelos, rutaVuelos) < 0) {
        printf("No se pudieron cargar los vuelos.\n");
    } else {
        printf("Vuelos cargados correctamente.\n");
    }

    printf("Introduce la ruta del CSV de pasajeros: ");
    if (leerCadena(rutaPasajeros, sizeof(rutaPasajeros)) != 0) {
        printf("Error al leer la ruta de pasajeros.\n");
        return;
    }

    if (pasajero_cargar_csv(listaPasajeros, &totalPasajeros, rutaPasajeros) < 0) {
        printf("No se pudieron cargar los pasajeros.\n");
    } else {
        printf("Pasajeros cargados correctamente.\n");
    }
}

/* ================= ACCIONES AEROPUERTOS ================= */

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

/* ================= ACCIONES VUELOS ================= */

void accionCrearVuelo(void) {
    char id[MAX_ID_VUELO];
    char aerolinea[MAX_AEROLINEA];
    char origen[MAX_CODIGO];
    char destino[MAX_CODIGO];
    char fecha[MAX_FECHA];
    char hora_salida[MAX_HORA];
    char hora_llegada[MAX_HORA];
    int capacidad;

    mostrarCabecera("CREAR VUELO");

    printf("ID del vuelo: ");
    if (leerCadena(id, sizeof(id)) != 0) {
        printf("Error al leer el ID del vuelo.\n");
        return;
    }

    printf("Aerolínea: ");
    if (leerCadena(aerolinea, sizeof(aerolinea)) != 0) {
        printf("Error al leer la aerolinea.\n");
        return;
    }

    printf("Origen: ");
    if (leerCadena(origen, sizeof(origen)) != 0) {
        printf("Error al leer el origen.\n");
        return;
    }

    printf("Destino: ");
    if (leerCadena(destino, sizeof(destino)) != 0) {
        printf("Error al leer el destino.\n");
        return;
    }

    printf("Fecha: ");
    if (leerCadena(fecha, sizeof(fecha)) != 0) {
        printf("Error al leer la fecha.\n");
        return;
    }

    printf("Hora de salida: ");
    if (leerCadena(hora_salida, sizeof(hora_salida)) != 0) {
        printf("Error al leer la hora de salida.\n");
        return;
    }

    printf("Hora de llegada: ");
    if (leerCadena(hora_llegada, sizeof(hora_llegada)) != 0) {
        printf("Error al leer la hora de llegada.\n");
        return;
    }

    printf("Capacidad: ");
    if (leerEntero(&capacidad) != 0) {
        printf("Capacidad no valida.\n");
        return;
    }

    if (vuelo_crear(listaVuelos, &totalVuelos, id, aerolinea, origen, destino,
                    fecha, hora_salida, hora_llegada, capacidad) == 0) {
        printf("Vuelo creado correctamente.\n");
    } else {
        printf("No se pudo crear el vuelo.\n");
    }
}

void accionEliminarVuelo(void) {
    char id[MAX_ID_VUELO];

    mostrarCabecera("ELIMINAR VUELO");

    printf("ID del vuelo a eliminar: ");
    if (leerCadena(id, sizeof(id)) != 0) {
        printf("Error al leer el ID.\n");
        return;
    }

    if (!confirmarAccion("¿Seguro que deseas eliminar este vuelo?")) {
        printf("Operacion cancelada.\n");
        return;
    }

    if (vuelo_eliminar(listaVuelos, &totalVuelos, id) == 0) {
        printf("Vuelo eliminado correctamente.\n");
    } else {
        printf("No se pudo eliminar el vuelo.\n");
    }
}

void accionActualizarVuelo(void) {
    char id[MAX_ID_VUELO];
    char nueva_hora_salida[MAX_HORA];
    char nueva_hora_llegada[MAX_HORA];
    int nueva_capacidad;

    mostrarCabecera("ACTUALIZAR VUELO");

    printf("ID del vuelo a actualizar: ");
    if (leerCadena(id, sizeof(id)) != 0) {
        printf("Error al leer el ID.\n");
        return;
    }

    printf("Nueva hora de salida (ENTER para no cambiar): ");
    if (leerCadena(nueva_hora_salida, sizeof(nueva_hora_salida)) != 0) {
        printf("Error al leer la hora de salida.\n");
        return;
    }

    printf("Nueva hora de llegada (ENTER para no cambiar): ");
    if (leerCadena(nueva_hora_llegada, sizeof(nueva_hora_llegada)) != 0) {
        printf("Error al leer la hora de llegada.\n");
        return;
    }

    printf("Nueva capacidad (0 para no cambiar): ");
    if (leerEntero(&nueva_capacidad) != 0) {
        printf("Capacidad no valida.\n");
        return;
    }

    if (vuelo_actualizar(listaVuelos, totalVuelos, id,
                         nueva_hora_salida, nueva_hora_llegada, nueva_capacidad) == 0) {
        printf("Vuelo actualizado correctamente.\n");
    } else {
        printf("No se pudo actualizar el vuelo.\n");
    }
}

void accionVerVuelos(void) {
    mostrarCabecera("VER VUELOS");
    vuelo_ver(listaVuelos, totalVuelos);
}

/* ================= ACCIONES PASAJEROS ================= */

void accionCrearPasajero(void) {
    char dni[MAX_DNI];
    char nombre[MAX_NOMBRE_P];
    char apellido[MAX_APELLIDO_P];
    char email[MAX_EMAIL];
    char telefono[MAX_TELEFONO];

    mostrarCabecera("CREAR PASAJERO");

    printf("DNI: ");
    if (leerCadena(dni, sizeof(dni)) != 0) {
        printf("Error al leer el DNI.\n");
        return;
    }

    printf("Nombre: ");
    if (leerCadena(nombre, sizeof(nombre)) != 0) {
        printf("Error al leer el nombre.\n");
        return;
    }

    printf("Apellido: ");
    if (leerCadena(apellido, sizeof(apellido)) != 0) {
        printf("Error al leer el apellido.\n");
        return;
    }

    printf("Email: ");
    if (leerCadena(email, sizeof(email)) != 0) {
        printf("Error al leer el email.\n");
        return;
    }

    printf("Telefono: ");
    if (leerCadena(telefono, sizeof(telefono)) != 0) {
        printf("Error al leer el telefono.\n");
        return;
    }

    if (pasajero_crear(listaPasajeros, &totalPasajeros, dni, nombre, apellido, email, telefono) == 0) {
        printf("Pasajero creado correctamente.\n");
    } else {
        printf("No se pudo crear el pasajero.\n");
    }
}

void accionEliminarPasajero(void) {
    char dni[MAX_DNI];

    mostrarCabecera("ELIMINAR PASAJERO");

    printf("DNI del pasajero a eliminar: ");
    if (leerCadena(dni, sizeof(dni)) != 0) {
        printf("Error al leer el DNI.\n");
        return;
    }

    if (!confirmarAccion("¿Seguro que deseas eliminar este pasajero?")) {
        printf("Operacion cancelada.\n");
        return;
    }

    if (pasajero_eliminar(listaPasajeros, &totalPasajeros, dni) == 0) {
        printf("Pasajero eliminado correctamente.\n");
    } else {
        printf("No se pudo eliminar el pasajero.\n");
    }
}

void accionActualizarPasajero(void) {
    char dni[MAX_DNI];
    char nuevo_email[MAX_EMAIL];
    char nuevo_telefono[MAX_TELEFONO];

    mostrarCabecera("ACTUALIZAR PASAJERO");

    printf("DNI del pasajero a actualizar: ");
    if (leerCadena(dni, sizeof(dni)) != 0) {
        printf("Error al leer el DNI.\n");
        return;
    }

    printf("Nuevo email (ENTER para no cambiar): ");
    if (leerCadena(nuevo_email, sizeof(nuevo_email)) != 0) {
        printf("Error al leer el email.\n");
        return;
    }

    printf("Nuevo telefono (ENTER para no cambiar): ");
    if (leerCadena(nuevo_telefono, sizeof(nuevo_telefono)) != 0) {
        printf("Error al leer el telefono.\n");
        return;
    }

    if (pasajero_actualizar(listaPasajeros, totalPasajeros, dni, nuevo_email, nuevo_telefono) == 0) {
        printf("Pasajero actualizado correctamente.\n");
    } else {
        printf("No se pudo actualizar el pasajero.\n");
    }
}

void accionVerPasajeros(void) {
    mostrarCabecera("VER PASAJEROS");
    pasajero_ver(listaPasajeros, totalPasajeros);
}

/* ================= OPERACIONES ================= */

void accionAsignarPasajeroAVuelo(void) {
    char dni[MAX_DNI];
    char id_vuelo[MAX_ID_VUELO];

    mostrarCabecera("ASIGNAR PASAJERO A VUELO");

    printf("DNI del pasajero: ");
    if (leerCadena(dni, sizeof(dni)) != 0) {
        printf("Error al leer el DNI.\n");
        return;
    }

    printf("ID del vuelo: ");
    if (leerCadena(id_vuelo, sizeof(id_vuelo)) != 0) {
        printf("Error al leer el ID del vuelo.\n");
        return;
    }

    if (vuelo_buscar(listaVuelos, totalVuelos, id_vuelo) == -1) {
        printf("El vuelo no existe.\n");
        return;
    }

    if (pasajero_buscar(listaPasajeros, totalPasajeros, dni) == -1) {
        printf("El pasajero no existe.\n");
        return;
    }

    if (vuelo_reservar_asiento(listaVuelos, totalVuelos, id_vuelo) != 0) {
        printf("No se pudo reservar asiento en el vuelo.\n");
        return;
    }

    if (pasajero_asignar_vuelo(listaPasajeros, totalPasajeros, dni, id_vuelo) != 0) {
        vuelo_liberar_asiento(listaVuelos, totalVuelos, id_vuelo);
        printf("No se pudo asignar el vuelo al pasajero.\n");
        return;
    }

    printf("Pasajero asignado al vuelo correctamente.\n");
}

void accionRegistrarEquipajeAPasajero(void) {
    mostrarCabecera("REGISTRAR EQUIPAJE A PASAJERO");
    mostrarFuncionNoDisponible("Registrar equipaje a pasajero");
}

void accionVerPasajerosPorVuelo(void) {
    char id_vuelo[MAX_ID_VUELO];
    int encontrados = 0;
    int i, j;

    mostrarCabecera("VER PASAJEROS POR VUELO");

    printf("ID del vuelo: ");
    if (leerCadena(id_vuelo, sizeof(id_vuelo)) != 0) {
        printf("Error al leer el ID del vuelo.\n");
        return;
    }

    if (vuelo_buscar(listaVuelos, totalVuelos, id_vuelo) == -1) {
        printf("El vuelo no existe.\n");
        return;
    }

    printf("\nPasajeros asignados al vuelo %s:\n\n", id_vuelo);

    for (i = 0; i < totalPasajeros; i++) {
        for (j = 0; j < listaPasajeros[i].num_vuelos; j++) {
            if (strcmp(listaPasajeros[i].vuelos[j], id_vuelo) == 0) {
                printf("DNI: %s\n", listaPasajeros[i].dni);
                printf("Nombre: %s\n", listaPasajeros[i].nombre);
                printf("Apellido: %s\n", listaPasajeros[i].apellido);
                printf("Email: %s\n", listaPasajeros[i].email);
                printf("Telefono: %s\n", listaPasajeros[i].telefono);
                printf("----------------------------------------\n");
                encontrados = 1;
                break;
            }
        }
    }

    if (!encontrados) {
        printf("No hay pasajeros asignados a este vuelo.\n");
    }
}

void accionVerEquipajePorPasajero(void) {
    mostrarCabecera("VER EQUIPAJE POR PASAJERO");
    mostrarFuncionNoDisponible("Ver equipaje por pasajero");
}

/* ================= EQUIPAJE Y OTROS ================= */

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