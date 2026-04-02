#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "usuario.h"

#define MAX_NOMBRE 100
#define MAX_EMAIL 100
#define MAX_CONTRASENA 100
#define MAX_BUFFER 256

/* ================= FUNCIONES AUXILIARES ================= */

static void limpiarBufferEntrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        /* vaciar buffer */
    }
}

static void leerCadena(const char *mensaje, char *destino, size_t tam) {
    if (mensaje != NULL) {
        printf("%s", mensaje);
    }

    if (fgets(destino, (int)tam, stdin) != NULL) {
        destino[strcspn(destino, "\n")] = '\0';
    } else {
        destino[0] = '\0';
    }
}

static int leerEntero(const char *mensaje, int *valor) {
    char buffer[MAX_BUFFER];
    char *fin;
    long num;

    if (mensaje != NULL) {
        printf("%s", mensaje);
    }

    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return -1;
    }

    buffer[strcspn(buffer, "\n")] = '\0';

    if (buffer[0] == '\0') {
        return -1;
    }

    num = strtol(buffer, &fin, 10);

    if (*fin != '\0') {
        return -1;
    }

    *valor = (int)num;
    return 0;
}

static int cadenaVacia(const char *texto) {
    return (texto == NULL || texto[0] == '\0');
}

static int validarEmail(const char *email) {
    const char *arroba;
    const char *punto;

    if (cadenaVacia(email)) {
        return 0;
    }

    arroba = strchr(email, '@');
    punto = strrchr(email, '.');

    if (arroba == NULL || punto == NULL) {
        return 0;
    }

    if (arroba == email) {
        return 0;
    }

    if (punto < arroba) {
        return 0;
    }

    if (*(punto + 1) == '\0') {
        return 0;
    }

    return 1;
}

static int validarTelefono(int telefono) {
    return telefono > 0;
}

static int confirmarAccion(const char *mensaje) {
    int opcion;

    printf("%s\n", mensaje);
    printf("1. SI\n");
    printf("2. NO\n");

    if (leerEntero("Opcion: ", &opcion) != 0) {
        return 0;
    }

    return opcion == 1;
}

static void pedirDatosUsuario(Usuario *u) {
    if (u == NULL) {
        return;
    }

    do {
        leerCadena("Nombre: ", u->nombre, sizeof(u->nombre));
        if (cadenaVacia(u->nombre)) {
            printf("Error: el nombre no puede estar vacio.\n");
        }
    } while (cadenaVacia(u->nombre));

    do {
        leerCadena("Email: ", u->email, sizeof(u->email));
        if (!validarEmail(u->email)) {
            printf("Error: email no valido.\n");
        }
    } while (!validarEmail(u->email));

    do {
        if (leerEntero("Telefono: ", &u->telefono) != 0 || !validarTelefono(u->telefono)) {
            printf("Error: telefono no valido.\n");
            u->telefono = 0;
        }
    } while (!validarTelefono(u->telefono));

    do {
        leerCadena("Contrasena: ", u->contrasena, sizeof(u->contrasena));
        if (cadenaVacia(u->contrasena)) {
            printf("Error: la contrasena no puede estar vacia.\n");
        }
    } while (cadenaVacia(u->contrasena));
}

static void mostrarUsuario(const Usuario *u) {
    if (u == NULL) {
        return;
    }

    printf("\n===== DATOS DEL USUARIO =====\n");
    printf("ID: %d\n", u->id);
    printf("Nombre: %s\n", u->nombre);
    printf("Email: %s\n", u->email);
    printf("Telefono: %d\n", u->telefono);
    printf("Contrasena: %s\n", u->contrasena);
    printf("=============================\n");
}

/* ================= FUNCIONES PRINCIPALES ================= */

int crearUsuario(void) {
    Usuario nuevoUsuario;

    printf("\n===== CREAR USUARIO =====\n");

    nuevoUsuario.id = 0;
    pedirDatosUsuario(&nuevoUsuario);

    mostrarUsuario(&nuevoUsuario);

    /*
    insertarClientes(
        nuevoUsuario.nombre,
        nuevoUsuario.email,
        nuevoUsuario.telefono,
        nuevoUsuario.contrasena,
        CLIENT_DB_PATH
    );
    */

    printf("Usuario creado correctamente.\n");
    return 0;
}

int eliminarUsuario(void) {
    int idUsuario;

    printf("\n===== ELIMINAR USUARIO =====\n");

    /*
    imprimirUsuarios();
    */

    if (leerEntero("Inserta el id del usuario que quieres eliminar: ", &idUsuario) != 0) {
        printf("Error: ID no valido.\n");
        return -1;
    }

    if (!confirmarAccion("¿Quieres eliminar este usuario?")) {
        printf("Eliminacion cancelada.\n");
        return 0;
    }

    /*
    deleteClientes(idUsuario, CLIENT_DB_PATH);
    */

    printf("Usuario eliminado correctamente.\n");
    return 0;
}

int actualizarUsuario(void) {
    Usuario usuarioActualizado;

    printf("\n===== ACTUALIZAR USUARIO =====\n");

    /*
    imprimirUsuarios();
    */

    if (leerEntero("Inserta el id del usuario que quieres actualizar: ", &usuarioActualizado.id) != 0) {
        printf("Error: ID no valido.\n");
        return -1;
    }

    pedirDatosUsuario(&usuarioActualizado);

    mostrarUsuario(&usuarioActualizado);

    /*
    updateClientes(
        usuarioActualizado.id,
        usuarioActualizado.nombre,
        usuarioActualizado.email,
        usuarioActualizado.telefono,
        usuarioActualizado.contrasena,
        CLIENT_DB_PATH
    );
    */

    printf("Usuario actualizado correctamente.\n");
    return 0;
}