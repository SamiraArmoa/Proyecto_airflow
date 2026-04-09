
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "usuario.h"
#define MAX_LENGTH 100

// Crear un nuevo empleado
int crearUsuario() {
	char str[MAX_LENGTH];
	char *nombre;
	char *email;
	int telefono;
	char *contrasena;
	char nom[100];
	char em[100];
	char con[100];

	printf("CREAR USUARIO\n");
	printf("Nombre: ");
	fflush(stdin);
	fgets(nom, 100, stdin);
	nom[strcspn(nom, "\n")] = '\0';

	nombre = (char*)malloc((strlen(nom) + 1) * sizeof(char));
	if (nombre == NULL) {
		printf("Error al asignar memoria para el nombre.\n");
		return -1;
	}
	strcpy(nombre, nom);

	printf("Email: ");
	fflush(stdin);
	fgets(em, 100, stdin);
	nom[strcspn(em, "\n")] = '\0';

	email = (char *)malloc((strlen(em) + 1) * sizeof(char));
	if (email == NULL) {
		printf("Error al asignar memoria para el nombre.\n");
		return -1;
	}
	strcpy(email, em);

	printf("Telefono: ");
	fflush(stdin);
	fgets(str, MAX_LENGTH, stdin);
	sscanf(str, "%i", &telefono);

	printf("\nContrasena: ");
	fgets(con, 100, stdin);
	con[strcspn(con, "\n")] = '\0';

	contrasena = (char *)malloc((strlen(con) + 1) * sizeof(char));
	if (contrasena == NULL) {
		printf("Error al asignar memoria para el tipo.\n");
		return -1;
	}
	strcpy(contrasena, con);

	//insertarClientes(nombre, email, telefono, contrasena, CLIENT_DB_PATH);
	fflush(stdout);
	return 0;
}

// Eliminar empleado por nombre
int eliminarUsuario() {
	int id_cl = 0;
	printf("Inserta el id del usuario que quieres eliminar: ");
	scanf("%d", &id_cl);

	int confirmacion = 0;
	//	printf("\nELIMINAR EMPLEADO\n");
	printf("¿Quieres eliminar este usuario?\n");
	printf("1. SI\n");
	printf("2. NO\n");
	printf("Opcion: ");
	scanf("%d", &confirmacion);

	if (confirmacion != 1) {
		printf("Eliminacion cancelada.\n");
		return 0;
	}

	//deleteClientes(id_cl, CLIENT_DB_PATH);
	return 0;
}


// Actualizar por ID
int actualizarUsuario() {
	int id_us = 0;
	char *nombre;
	char *email;
	int telefono;
	char *contrasena;
	char nom[100];
	char em[100];
	char con[100];

	printf("Inserta el id del usuario que quieres actualizar: ");
	fflush(stdin);
	scanf("%i", &id_us);

	printf("Nombre: ");
	fflush(stdin);
	fgets(nom, 100, stdin);
	nom[strcspn(nom, "\n")] = '\0';

	nombre = (char *)malloc((strlen(nom) + 1) * sizeof(char));
	if (nombre == NULL) {
		printf("Error al asignar memoria para el nombre.\n");
		return -1;
	}
	strcpy(nombre, nom);
	printf("Email: ");
	fflush(stdin);
	fgets(em, 25, stdin);
	em[strcspn(em, "\n")] = '\0';

	email = (char *)malloc((strlen(em) + 1) * sizeof(char));
	if (email == NULL) {
		free(nombre);
		printf("Error al asignar memoria para el cargo.\n");
		return -1;
	}
	strcpy(email, em);

	printf("Telefono: ");
	fflush(stdin);
	scanf("%i", &telefono);

	printf("Contrasena: ");
	fflush(stdin);
	fgets(con, 75, stdin);
	con[strcspn(con, "\n")] = '\0';

	contrasena = (char *)malloc((strlen(con) + 1) * sizeof(char));
	if (contrasena == NULL) {
		free(nombre);
		free(email);
		printf("Error al asignar memoria para la cuenta bancaria.\n");
		return -1;
	}

	strcpy(contrasena, con);
	return 0;
}

