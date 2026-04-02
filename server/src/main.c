#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "usuario/usuario.h"

#define OPCION_MIN 1
#define OPCION_SALIR 21

typedef enum {
    OPCION_CARGAR_CSV = 1,
    OPCION_CREAR_EMPLEADO,
    OPCION_ELIMINAR_EMPLEADO,
    OPCION_ACTUALIZAR_EMPLEADO,
    OPCION_VER_EMPLEADOS,
    OPCION_ACTUALIZAR_RESTAURANTE,
    OPCION_CREAR_RESTAURANTE,
    OPCION_VER_CLIENTES,
    OPCION_VER_PEDIDOS,
    OPCION_CREAR_PRODUCTO,
    OPCION_ELIMINAR_PRODUCTO,
    OPCION_ACTUALIZAR_PRODUCTO,
    OPCION_VER_PRODUCTOS,
    OPCION_CREAR_INGREDIENTE,
    OPCION_ELIMINAR_INGREDIENTE,
    OPCION_VER_INGREDIENTES,
    OPCION_CREAR_PEDIDO,
    OPCION_CREAR_CLIENTE,
    OPCION_ELIMINAR_CLIENTE,
    OPCION_ACTUALIZAR_CLIENTE,
    OPCION_MENU_SALIR
} OpcionMenu;

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

    printf("1.  Cargar datos desde CSV\n");
    printf("2.  Crear empleados\n");
    printf("3.  Eliminar empleados\n");
    printf("4.  Actualizar empleados\n");
    printf("5.  Ver empleados\n");
    printf("6.  Actualizar restaurantes\n");
    printf("7.  Crear restaurantes\n");
    printf("8.  Ver clientes\n");
    printf("9.  Ver pedidos\n");
    printf("10. Crear productos\n");
    printf("11. Eliminar productos\n");
    printf("12. Actualizar productos\n");
    printf("13. Ver productos\n");
    printf("14. Crear ingredientes\n");
    printf("15. Eliminar ingredientes\n");
    printf("16. Ver ingredientes\n");
    printf("17. Crear pedidos\n");
    printf("18. Crear cliente\n");
    printf("19. Eliminar cliente\n");
    printf("20. Actualizar cliente\n");
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

/* ================= ACCIONES ================= */

void accionCargarCSV(void) {
    mostrarCabecera("CARGAR DATOS DESDE CSV");
    printf("Cargando datos desde CSV...\n");
    /* cargar_csvs(); */
}

void accionCrearEmpleado(void) {
    mostrarCabecera("CREAR EMPLEADO");
    mostrarFuncionNoDisponible("Crear empleado");
    /* crearEmpleado(); */
}

void accionEliminarEmpleado(void) {
    mostrarCabecera("ELIMINAR EMPLEADO");
    mostrarFuncionNoDisponible("Eliminar empleado");
    /* eliminarEmpleado(); */
}

void accionActualizarEmpleado(void) {
    mostrarCabecera("ACTUALIZAR EMPLEADO");
    mostrarFuncionNoDisponible("Actualizar empleado");
    /* actualizarEmpleado(); */
}

void accionVerEmpleados(void) {
    mostrarCabecera("VER EMPLEADOS");
    mostrarFuncionNoDisponible("Ver empleados");
    /* verEmpleados(); */
}

void accionActualizarRestaurante(void) {
    mostrarCabecera("ACTUALIZAR RESTAURANTE");
    mostrarFuncionNoDisponible("Actualizar restaurante");
}

void accionCrearRestaurante(void) {
    mostrarCabecera("CREAR RESTAURANTE");
    mostrarFuncionNoDisponible("Crear restaurante");
}

void accionVerClientes(void) {
    mostrarCabecera("VER CLIENTES");
    mostrarFuncionNoDisponible("Ver clientes");
    /* imprimirUsuarios(); */
}

void accionVerPedidos(void) {
    mostrarCabecera("VER PEDIDOS");
    mostrarFuncionNoDisponible("Ver pedidos");
    /* imprimirPedidos(); */
}

void accionCrearProducto(void) {
    mostrarCabecera("CREAR PRODUCTO");
    mostrarFuncionNoDisponible("Crear producto");
}

void accionEliminarProducto(void) {
    mostrarCabecera("ELIMINAR PRODUCTO");
    mostrarFuncionNoDisponible("Eliminar producto");
}

void accionActualizarProducto(void) {
    mostrarCabecera("ACTUALIZAR PRODUCTO");
    mostrarFuncionNoDisponible("Actualizar producto");
}

void accionVerProductos(void) {
    mostrarCabecera("VER PRODUCTOS");
    mostrarFuncionNoDisponible("Ver productos");
}

void accionCrearIngrediente(void) {
    mostrarCabecera("CREAR INGREDIENTE");
    mostrarFuncionNoDisponible("Crear ingrediente");
}

void accionEliminarIngrediente(void) {
    mostrarCabecera("ELIMINAR INGREDIENTE");
    mostrarFuncionNoDisponible("Eliminar ingrediente");
}

void accionVerIngredientes(void) {
    mostrarCabecera("VER INGREDIENTES");
    mostrarFuncionNoDisponible("Ver ingredientes");
}

void accionCrearPedido(void) {
    mostrarCabecera("CREAR PEDIDO");
    mostrarFuncionNoDisponible("Crear pedido");
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
    /* actualizarUsuario(); */
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

        case OPCION_CREAR_EMPLEADO:
            accionCrearEmpleado();
            break;

        case OPCION_ELIMINAR_EMPLEADO:
            accionEliminarEmpleado();
            break;

        case OPCION_ACTUALIZAR_EMPLEADO:
            accionActualizarEmpleado();
            break;

        case OPCION_VER_EMPLEADOS:
            accionVerEmpleados();
            break;

        case OPCION_ACTUALIZAR_RESTAURANTE:
            accionActualizarRestaurante();
            break;

        case OPCION_CREAR_RESTAURANTE:
            accionCrearRestaurante();
            break;

        case OPCION_VER_CLIENTES:
            accionVerClientes();
            break;

        case OPCION_VER_PEDIDOS:
            accionVerPedidos();
            break;

        case OPCION_CREAR_PRODUCTO:
            accionCrearProducto();
            break;

        case OPCION_ELIMINAR_PRODUCTO:
            accionEliminarProducto();
            break;

        case OPCION_ACTUALIZAR_PRODUCTO:
            accionActualizarProducto();
            break;

        case OPCION_VER_PRODUCTOS:
            accionVerProductos();
            break;

        case OPCION_CREAR_INGREDIENTE:
            accionCrearIngrediente();
            break;

        case OPCION_ELIMINAR_INGREDIENTE:
            accionEliminarIngrediente();
            break;

        case OPCION_VER_INGREDIENTES:
            accionVerIngredientes();
            break;

        case OPCION_CREAR_PEDIDO:
            accionCrearPedido();
            break;

        case OPCION_CREAR_CLIENTE:
            accionCrearCliente();
            break;

        case OPCION_ELIMINAR_CLIENTE:
            accionEliminarCliente();
            break;

        case OPCION_ACTUALIZAR_CLIENTE:
            accionActualizarCliente();
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