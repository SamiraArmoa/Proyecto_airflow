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
#define OPCION_SALIR 22

// PROTOTIPOS

void limpiarBufferEntrada(void);
int leerEntero(int *valor);
int leerFlotante(float *valor);
int leerCaracter(char *caracter);
int leerCadena(char *buffer, int tam);
void pausarPantalla(void);
int confirmarAccion(const char *mensaje);
int opcionValida(int opcion);
void mostrarCabecera(const char *titulo);
void mostrarFuncionNoDisponible(const char *nombreFuncion);

void mostrarMenu(void);
int leerOpcionMenu(int *opcion);
int escogerOpcion(int *opcion);

void mostrarMenuEmpleado(void);
int leerOpcionMenuEmpleado(int *opcion);

void mostrarMenuPasajero(void);
int leerOpcionMenuPasajero(int *opcion);

void accionCargarCSV(void);
void accionCrearAeropuerto(void);
void accionEliminarAeropuerto(void);
void accionActualizarAeropuerto(void);
void accionVerAeropuertos(void);

void accionCrearVuelo(void);
void accionEliminarVuelo(void);
void accionActualizarVuelo(void);
void accionVerVuelos(void);

void accionCrearPasajero(void);
void accionEliminarPasajero(void);
void accionActualizarPasajero(void);
void accionVerPasajeros(void);

void accionRegistrarEquipaje(void);
void accionEliminarEquipaje(void);
void accionVerEquipaje(void);

void accionAsignarPasajeroAVuelo(void);
void accionRegistrarEquipajeAPasajero(void);
void accionVerPasajerosPorVuelo(void);
void accionVerEquipajePorPasajero(void);

int accionSalir(void);

void ejecutarMenuEmpleado(void);
void ejecutarMenuPasajero(void);
void mostrarDatosPasajero(const char *dni);
void mostrarVuelosPasajero(const char *dni);

int loginSistema(void);
void inicializarConfigPorDefecto(void);

// ROLES

typedef enum {
    ROL_INVALIDO = 0,
    ROL_ADMINISTRADOR = 1,
    ROL_EMPLEADO = 2,
    ROL_PASAJERO = 3
} RolUsuario;

// CONFIG GLOBAL

static Config configSistema;

// DATOS GLOBALES

static Aeropuerto listaAeropuertos[MAX_AEROPUERTOS];
static int totalAeropuertos = 0;

static Vuelo listaVuelos[MAX_VUELOS];
static int totalVuelos = 0;

static Pasajero listaPasajeros[MAX_PASAJEROS];
static int totalPasajeros = 0;

static Equipaje listaEquipaje[MAX_EQUIPAJES];
static int totalEquipaje = 0;

// FUNCIONES AUXILIARES

void limpiarBufferEntrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
    }
}

int leerEntero(int *valor) {
    char buffer[64];
    long numero;
    char extra;

    if (valor == NULL) {
        return -1;
    }

    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return -1;
    }

    if (sscanf(buffer, " %ld %c", &numero, &extra) != 1) {
        printf("Error: introduce un numero entero valido.\n");
        return -1;
    }

    *valor = (int)numero;
    return 0;
}

int leerFlotante(float *valor) {
    char buffer[64];
    float numero;
    char extra;

    if (valor == NULL) {
        return -1;
    }

    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return -1;
    }

    if (sscanf(buffer, " %f %c", &numero, &extra) != 1) {
        printf("Error: introduce un numero decimal valido.\n");
        return -1;
    }

    *valor = numero;
    return 0;
}

int leerCaracter(char *caracter) {
    char buffer[16];
    char c;
    char extra;

    if (caracter == NULL) {
        return -1;
    }

    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return -1;
    }

    if (sscanf(buffer, " %c %c", &c, &extra) != 1) {
        printf("Error al leer el caracter.\n");
        return -1;
    }

    *caracter = c;
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

    return (respuesta == 's' || respuesta == 'S');
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

void inicializarConfigPorDefecto(void) {
    memset(&configSistema, 0, sizeof(configSistema));
}

/* ================= LOGIN ================= */

int loginSistema(void) {
    char usuario[100];
    char password[100];

    mostrarCabecera("LOGIN");

    printf("Usuario: ");
    if (leerCadena(usuario, sizeof(usuario)) != 0) {
        return ROL_INVALIDO;
    }

    printf("Password: ");
    if (leerCadena(password, sizeof(password)) != 0) {
        return ROL_INVALIDO;
    }

    if (strcmp(usuario, configSistema.admin_user) == 0 &&
        strcmp(password, configSistema.admin_pass) == 0) {
        return ROL_ADMINISTRADOR;
    }

    if (strcmp(usuario, configSistema.empleado_user) == 0 &&
        strcmp(password, configSistema.empleado_pass) == 0) {
        return ROL_EMPLEADO;
    }

    if (strcmp(usuario, configSistema.pasajero_user) == 0 &&
        strcmp(password, configSistema.pasajero_pass) == 0) {
        return ROL_PASAJERO;
    }

    return ROL_INVALIDO;
}

// OPCIONES ADMIN

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

    OPCION_CAMBIAR_USUARIO,
    OPCION_MENU_SALIR
} OpcionMenu;

// MENU ADMIN

void mostrarMenu(void) {
    mostrarCabecera("MENU ADMINISTRADOR");

    printf("1. Cargar aeropuertos, vuelos, pasajeros y equipajes desde .csv\n");

    printf("--- AEROPUERTOS ---\n");
    printf("2.  Crear aeropuerto\n");
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

    printf("21. Cambiar usuario\n");
    printf("22. Salir\n");
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

// MENU EMPLEADO

void mostrarMenuEmpleado(void) {
    mostrarCabecera("MENU EMPLEADO");

    printf("1. Ver aeropuertos\n");
    printf("2. Ver vuelos\n");
    printf("3. Ver pasajeros\n");
    printf("4. Ver equipaje\n");
    printf("5. Asignar pasajero a vuelo\n");
    printf("6. Ver pasajeros por vuelo\n");
    printf("7. Ver equipaje por pasajero\n");
    printf("8. Salir\n");
}

int leerOpcionMenuEmpleado(int *opcion) {
    if (opcion == NULL) {
        return -1;
    }

    mostrarMenuEmpleado();
    printf("\nElige una opcion: ");

    if (leerEntero(opcion) != 0) {
        return -1;
    }

    if (*opcion < 1 || *opcion > 8) {
        printf("Error: opcion no valida.\n");
        return -1;
    }

    return 0;
}

// MENU PASAJERO

void mostrarMenuPasajero(void) {
    mostrarCabecera("MENU PASAJERO");

    printf("1. Ver mis datos\n");
    printf("2. Ver vuelos disponibles\n");
    printf("3. Ver mis vuelos\n");
    printf("4. Ver mi equipaje\n");
    printf("5. Salir\n");
}

int leerOpcionMenuPasajero(int *opcion) {
    if (opcion == NULL) {
        return -1;
    }

    mostrarMenuPasajero();
    printf("\nElige una opcion: ");

    if (leerEntero(opcion) != 0) {
        return -1;
    }

    if (*opcion < 1 || *opcion > 5) {
        printf("Error: opcion no valida.\n");
        return -1;
    }

    return 0;
}

// ACCIONES CARGA CSV

void accionCargarCSV(void) {
    mostrarCabecera("CARGAR DATOS DESDE CSV");

    if (strlen(configSistema.ruta_aeropuertos) == 0 ||
        strlen(configSistema.ruta_vuelos) == 0 ||
        strlen(configSistema.ruta_pasajeros) == 0 ||
        strlen(configSistema.ruta_equipajes) == 0) {
        printf("Error: faltan rutas en config.txt.\n");
        return;
    }

    totalAeropuertos = 0;
    totalVuelos = 0;
    totalPasajeros = 0;
    totalEquipaje = 0;

    aeropuerto_cargar_csv(listaAeropuertos, &totalAeropuertos, configSistema.ruta_aeropuertos);
    vuelo_cargar_csv(listaVuelos, &totalVuelos, configSistema.ruta_vuelos);
    pasajero_cargar_csv(listaPasajeros, &totalPasajeros, configSistema.ruta_pasajeros);
    equipaje_cargar_csv(listaEquipaje, &totalEquipaje, configSistema.ruta_equipajes);

    printf("\nDATOS CARGADOS:\n");
    printf("Aeropuertos: %d\n", totalAeropuertos);
    printf("Vuelos: %d\n", totalVuelos);
    printf("Pasajeros: %d\n", totalPasajeros);
    printf("Equipajes: %d\n", totalEquipaje);
}

// ACCIONES AEROPUERTOS

void accionCrearAeropuerto(void) {
    char codigo[MAX_CODIGO];
    char nombre[MAX_NOMBRE];
    char ciudad[MAX_CIUDAD];
    char pais[MAX_PAIS];
    int num_pistas;

    mostrarCabecera("CREAR AEROPUERTO");

    printf("Codigo: ");
    if (leerCadena(codigo, sizeof(codigo)) != 0) return;

    printf("Nombre: ");
    if (leerCadena(nombre, sizeof(nombre)) != 0) return;

    printf("Ciudad: ");
    if (leerCadena(ciudad, sizeof(ciudad)) != 0) return;

    printf("Pais: ");
    if (leerCadena(pais, sizeof(pais)) != 0) return;

    printf("Numero de pistas: ");
    if (leerEntero(&num_pistas) != 0) return;

    if (num_pistas < 0) {
        printf("El numero de pistas no puede ser negativo.\n");
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
    if (leerCadena(codigo, sizeof(codigo)) != 0) return;

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
    if (leerCadena(codigo, sizeof(codigo)) != 0) return;

    printf("Nuevo nombre (ENTER para no cambiar): ");
    if (leerCadena(nuevo_nombre, sizeof(nuevo_nombre)) != 0) return;

    printf("Nueva ciudad (ENTER para no cambiar): ");
    if (leerCadena(nueva_ciudad, sizeof(nueva_ciudad)) != 0) return;

    printf("Nuevo pais (ENTER para no cambiar): ");
    if (leerCadena(nuevo_pais, sizeof(nuevo_pais)) != 0) return;

    printf("Nuevo numero de pistas (0 para no cambiar): ");
    if (leerEntero(&nuevo_num_pistas) != 0) return;

    if (nuevo_num_pistas < 0) {
        printf("El numero de pistas no puede ser negativo.\n");
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

// ACCIONES VUELOS

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
    if (leerCadena(id, sizeof(id)) != 0) return;

    printf("Aerolínea: ");
    if (leerCadena(aerolinea, sizeof(aerolinea)) != 0) return;

    printf("Origen: ");
    if (leerCadena(origen, sizeof(origen)) != 0) return;

    printf("Destino: ");
    if (leerCadena(destino, sizeof(destino)) != 0) return;

    printf("Fecha: ");
    if (leerCadena(fecha, sizeof(fecha)) != 0) return;

    printf("Hora de salida: ");
    if (leerCadena(hora_salida, sizeof(hora_salida)) != 0) return;

    printf("Hora de llegada: ");
    if (leerCadena(hora_llegada, sizeof(hora_llegada)) != 0) return;

    printf("Capacidad: ");
    if (leerEntero(&capacidad) != 0) return;

    if (capacidad <= 0) {
        printf("La capacidad debe ser mayor que 0.\n");
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
    if (leerCadena(id, sizeof(id)) != 0) return;

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
    if (leerCadena(id, sizeof(id)) != 0) return;

    printf("Nueva hora de salida (ENTER para no cambiar): ");
    if (leerCadena(nueva_hora_salida, sizeof(nueva_hora_salida)) != 0) return;

    printf("Nueva hora de llegada (ENTER para no cambiar): ");
    if (leerCadena(nueva_hora_llegada, sizeof(nueva_hora_llegada)) != 0) return;

    printf("Nueva capacidad (0 para no cambiar): ");
    if (leerEntero(&nueva_capacidad) != 0) return;

    if (nueva_capacidad < 0) {
        printf("La capacidad no puede ser negativa.\n");
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

// ACCIONES PASAJEROS

void accionCrearPasajero(void) {
    char dni[MAX_DNI];
    char nombre[MAX_NOMBRE_P];
    char apellido[MAX_APELLIDO_P];
    char email[MAX_EMAIL];
    char telefono[MAX_TELEFONO];

    mostrarCabecera("CREAR PASAJERO");

    printf("DNI: ");
    if (leerCadena(dni, sizeof(dni)) != 0) return;

    printf("Nombre: ");
    if (leerCadena(nombre, sizeof(nombre)) != 0) return;

    printf("Apellido: ");
    if (leerCadena(apellido, sizeof(apellido)) != 0) return;

    printf("Email: ");
    if (leerCadena(email, sizeof(email)) != 0) return;

    printf("Telefono: ");
    if (leerCadena(telefono, sizeof(telefono)) != 0) return;

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
    if (leerCadena(dni, sizeof(dni)) != 0) return;

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
    if (leerCadena(dni, sizeof(dni)) != 0) return;

    printf("Nuevo email (ENTER para no cambiar): ");
    if (leerCadena(nuevo_email, sizeof(nuevo_email)) != 0) return;

    printf("Nuevo telefono (ENTER para no cambiar): ");
    if (leerCadena(nuevo_telefono, sizeof(nuevo_telefono)) != 0) return;

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

// OPERACIONES

void accionAsignarPasajeroAVuelo(void) {
    char dni[MAX_DNI];
    char id[MAX_ID_VUELO];

    mostrarCabecera("ASIGNAR PASAJERO A VUELO");

    printf("DNI pasajero: ");
    if (leerCadena(dni, sizeof(dni)) != 0) return;

    printf("ID vuelo: ");
    if (leerCadena(id, sizeof(id)) != 0) return;

    if (vuelo_buscar(listaVuelos, totalVuelos, id) == -1) {
        printf("El vuelo no existe.\n");
        return;
    }

    if (pasajero_buscar(listaPasajeros, totalPasajeros, dni) == -1) {
        printf("El pasajero no existe.\n");
        return;
    }

    if (vuelo_reservar_asiento(listaVuelos, totalVuelos, id) != 0) {
        printf("No hay asientos disponibles.\n");
        return;
    }

    if (pasajero_asignar_vuelo(listaPasajeros, totalPasajeros, dni, id) == 0) {
        printf("Pasajero asignado correctamente.\n");
    } else {
        vuelo_liberar_asiento(listaVuelos, totalVuelos, id);
        printf("No se pudo asignar el pasajero al vuelo.\n");
    }
}

void accionRegistrarEquipajeAPasajero(void) {
    char dni[MAX_DNI];
    char id_eq[MAX_ID_EQ];

    mostrarCabecera("REGISTRAR EQUIPAJE A PASAJERO");

    printf("DNI pasajero: ");
    if (leerCadena(dni, sizeof(dni)) != 0) return;

    if (pasajero_buscar(listaPasajeros, totalPasajeros, dni) == -1) {
        printf("El pasajero no existe.\n");
        return;
    }

    printf("ID equipaje a asignar: ");
    if (leerCadena(id_eq, sizeof(id_eq)) != 0) return;

    if (equipaje_buscar(listaEquipaje, totalEquipaje, id_eq) == -1) {
        printf("El equipaje no existe. Registralo primero (opcion 14).\n");
        return;
    }

    printf("Equipaje '%s' asociado al pasajero '%s'.\n", id_eq, dni);
}

void accionVerPasajerosPorVuelo(void) {
    char id_vuelo[MAX_ID_VUELO];
    int encontrados = 0;
    int i, j;

    mostrarCabecera("VER PASAJEROS POR VUELO");

    printf("ID del vuelo: ");
    if (leerCadena(id_vuelo, sizeof(id_vuelo)) != 0) return;

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
    char dni[MAX_DNI];

    mostrarCabecera("VER EQUIPAJE POR PASAJERO");

    printf("DNI del pasajero: ");
    if (leerCadena(dni, sizeof(dni)) != 0) return;

    if (pasajero_buscar(listaPasajeros, totalPasajeros, dni) == -1) {
        printf("El pasajero no existe.\n");
        return;
    }

    equipaje_ver_por_pasajero(listaEquipaje, totalEquipaje, dni);
}

// EQUIPAJE

void accionRegistrarEquipaje(void) {
    char id_eq[MAX_ID_EQ];
    char dni[MAX_DNI];
    char tipo[MAX_TIPO_EQ];
    float peso;
    int facturado;

    mostrarCabecera("REGISTRAR EQUIPAJE");

    printf("ID equipaje: ");
    if (leerCadena(id_eq, sizeof(id_eq)) != 0) return;

    printf("DNI pasajero: ");
    if (leerCadena(dni, sizeof(dni)) != 0) return;

    if (pasajero_buscar(listaPasajeros, totalPasajeros, dni) == -1) {
        printf("El pasajero no existe.\n");
        return;
    }

    printf("Tipo (maleta/mochila/bulto): ");
    if (leerCadena(tipo, sizeof(tipo)) != 0) return;

    printf("Peso en kg: ");
    if (leerFlotante(&peso) != 0) return;

    if (peso < 0.0f) {
        printf("El peso no puede ser negativo.\n");
        return;
    }

    printf("Facturado (1=si, 0=no): ");
    if (leerEntero(&facturado) != 0) return;

    if (facturado != 0 && facturado != 1) {
        printf("El valor de facturado debe ser 0 o 1.\n");
        return;
    }

    if (equipaje_registrar(listaEquipaje, &totalEquipaje,
                           id_eq, dni, tipo, peso, facturado) == 0) {
        printf("Equipaje registrado correctamente.\n");
    } else {
        printf("No se pudo registrar el equipaje.\n");
    }
}

void accionEliminarEquipaje(void) {
    char id_eq[MAX_ID_EQ];

    mostrarCabecera("ELIMINAR EQUIPAJE");

    printf("ID del equipaje a eliminar: ");
    if (leerCadena(id_eq, sizeof(id_eq)) != 0) return;

    if (!confirmarAccion("¿Seguro que deseas eliminar este equipaje?")) {
        printf("Operacion cancelada.\n");
        return;
    }

    if (equipaje_eliminar(listaEquipaje, &totalEquipaje, id_eq) == 0) {
        printf("Equipaje eliminado correctamente.\n");
    } else {
        printf("No se pudo eliminar el equipaje.\n");
    }
}

void accionVerEquipaje(void) {
    mostrarCabecera("VER EQUIPAJE");
    equipaje_ver(listaEquipaje, totalEquipaje);
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

// CONTROLADOR ADMIN

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
        case OPCION_CAMBIAR_USUARIO:
            printf("Cerrando sesion de administrador...\n");
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

// MENU EMPLEADO

void ejecutarMenuEmpleado(void) {
    int opcion = 0;

    do {
        if (leerOpcionMenuEmpleado(&opcion) != 0) {
            pausarPantalla();
            continue;
        }

        switch (opcion) {
            case 1:
                accionVerAeropuertos();
                break;
            case 2:
                accionVerVuelos();
                break;
            case 3:
                accionVerPasajeros();
                break;
            case 4:
                accionVerEquipaje();
                break;
            case 5:
                accionAsignarPasajeroAVuelo();
                break;
            case 6:
                accionVerPasajerosPorVuelo();
                break;
            case 7:
                accionVerEquipajePorPasajero();
                break;
            case 8:
                printf("Saliendo del menu empleado...\n");
                break;
            default:
                printf("Opcion no valida.\n");
                break;
        }

        if (opcion != 8) {
            pausarPantalla();
        }

    } while (opcion != 8);
}

// MENU PASAJERO

void mostrarDatosPasajero(const char *dni) {
    int idx = pasajero_buscar(listaPasajeros, totalPasajeros, dni);

    if (idx == -1) {
        printf("Pasajero no encontrado.\n");
        return;
    }

    printf("DNI: %s\n", listaPasajeros[idx].dni);
    printf("Nombre: %s\n", listaPasajeros[idx].nombre);
    printf("Apellido: %s\n", listaPasajeros[idx].apellido);
    printf("Email: %s\n", listaPasajeros[idx].email);
    printf("Telefono: %s\n", listaPasajeros[idx].telefono);
}

void mostrarVuelosPasajero(const char *dni) {
    int idx = pasajero_buscar(listaPasajeros, totalPasajeros, dni);
    int j;

    if (idx == -1) {
        printf("Pasajero no encontrado.\n");
        return;
    }

    if (listaPasajeros[idx].num_vuelos == 0) {
        printf("No tienes vuelos asignados.\n");
        return;
    }

    printf("Vuelos del pasajero %s:\n", dni);
    for (j = 0; j < listaPasajeros[idx].num_vuelos; j++) {
        printf("- %s\n", listaPasajeros[idx].vuelos[j]);
    }
}

void ejecutarMenuPasajero(void) {
    int opcion = 0;
    char dni[MAX_DNI];

    printf("\nIntroduce tu DNI: ");
    if (leerCadena(dni, sizeof(dni)) != 0) {
        printf("Error al leer el DNI.\n");
        return;
    }

    if (pasajero_buscar(listaPasajeros, totalPasajeros, dni) == -1) {
        printf("No existe un pasajero con ese DNI. Carga primero los CSV o crea el pasajero.\n");
        return;
    }

    do {
        if (leerOpcionMenuPasajero(&opcion) != 0) {
            pausarPantalla();
            continue;
        }

        switch (opcion) {
            case 1:
                mostrarCabecera("MIS DATOS");
                mostrarDatosPasajero(dni);
                break;
            case 2:
                accionVerVuelos();
                break;
            case 3:
                mostrarCabecera("MIS VUELOS");
                mostrarVuelosPasajero(dni);
                break;
            case 4:
                mostrarCabecera("MI EQUIPAJE");
                equipaje_ver_por_pasajero(listaEquipaje, totalEquipaje, dni);
                break;
            case 5:
                printf("Saliendo del menu pasajero...\n");
                break;
            default:
                printf("Opcion no valida.\n");
                break;
        }

        if (opcion != 5) {
            pausarPantalla();
        }

    } while (opcion != 5);
}

// MAIN

int main(void) {
    int rol = 0;
    int opcion = 0;

    inicializarConfigPorDefecto();

    if (cargarConfig("config.txt", &configSistema) != 0) {
        printf("Error: no se pudo cargar config.txt\n");
        return 1;
    }

    do {
        rol = loginSistema();

        if (rol == ROL_INVALIDO) {
            printf("Credenciales incorrectas.\n");
            pausarPantalla();
            continue;
        }

        switch (rol) {
            case ROL_ADMINISTRADOR:
                opcion = 0;

                do {
                    if (leerOpcionMenu(&opcion) != 0) {
                        pausarPantalla();
                        continue;
                    }

                    escogerOpcion(&opcion);

                    if (opcion != OPCION_SALIR && opcion != OPCION_CAMBIAR_USUARIO) {
                        pausarPantalla();
                    }

                } while (opcion != OPCION_SALIR && opcion != OPCION_CAMBIAR_USUARIO);

                if (opcion == OPCION_CAMBIAR_USUARIO) {
                    opcion = 0;
                }

                break;

            case ROL_PASAJERO:
                ejecutarMenuPasajero();
                break;

            case ROL_EMPLEADO:
                ejecutarMenuEmpleado();
                break;

            default:
                printf("Rol no valido.\n");
                pausarPantalla();
                break;
        }

    } while (!confirmarAccion("¿Deseas cerrar completamente la aplicacion?"));

    printf("Saliendo del sistema...\n");
    return 0;
}