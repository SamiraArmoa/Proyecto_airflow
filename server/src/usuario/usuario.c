/*
 * cliente.c
 *
 *  Created on: 1 abr 2026
 *      Author: s.armoa
 */
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
	// Asignar memoria din�mica para el nombre segun la longitud
	nombre = (char*)malloc((strlen(nom) + 1) * sizeof(char));
	if (nombre == NULL) {
		printf("Error al asignar memoria para el nombre.\n");
		return -1;  // Error si no se pudo asignar memoria
	}
	strcpy(nombre, nom);  // Copiar la cadena le�da en nombre

	printf("Email: ");
	fflush(stdin);
	fgets(em, 100, stdin);
	nom[strcspn(em, "\n")] = '\0';
	// Asignar memoria din�mica para el nombre seg�n la longitud
	email = (char *)malloc((strlen(em) + 1) * sizeof(char));
	if (email == NULL) {
		printf("Error al asignar memoria para el nombre.\n");
		return -1;  // Error si no se pudo asignar memoria
	}
	strcpy(email, em);  // Copiar la cadena leida en nombre

	printf("Telefono: ");
	fflush(stdin);
	fgets(str, MAX_LENGTH, stdin);
	sscanf(str, "%i", &telefono);

	printf("\nContrasena: ");
	fgets(con, 100, stdin);
	con[strcspn(con, "\n")] = '\0';
	// Asignar memoria dinamica para el nombre segun la longitud
	contrasena = (char *)malloc((strlen(con) + 1) * sizeof(char));
	if (contrasena == NULL) {
		printf("Error al asignar memoria para el tipo.\n");
		return -1;  // Error si no se pudo asignar memoria
	}
	strcpy(contrasena, con);  // Copiar la cadena leida en nombre

	//insertarClientes(nombre, email, telefono, contrasena, CLIENT_DB_PATH);
	fflush(stdout);
	return 0;
}

// Eliminar empleado por nombre
int eliminarUsuario() {
	int id_cl = 0;
//	imprimirUsuarios();
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
		return 0; // Salimos sin hacer nada
	}

	//deleteClientes(id_cl, CLIENT_DB_PATH);
	return 0;
}

// Mostrar todos los empleados
//int imprimirClientes() {
//	int rc = verClientes(CLIENT_DB_PATH);
//	return rc;
//}

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

//	imprimirUsuarios();
	printf("Inserta el id del usuario que quieres actualizar: ");
	fflush(stdin);
	scanf("%i", &id_us);

	printf("Nombre: ");
	fflush(stdin);
	fgets(nom, 100, stdin);
	nom[strcspn(nom, "\n")] = '\0';
	// Asignar memoria din�mica para el nombre seg�n la longitud
	nombre = (char *)malloc((strlen(nom) + 1) * sizeof(char));
	if (nombre == NULL) {
		printf("Error al asignar memoria para el nombre.\n");
		return -1;  // Error si no se pudo asignar memoria
	}
	strcpy(nombre, nom);  // Copiar la cadena le�da en nombre

	printf("Email: ");
	fflush(stdin);
	fgets(em, 25, stdin);
	em[strcspn(em, "\n")] = '\0';  // Fix: changed from cargo to car
	// Asignar memoria din�mica para el cargo seg�n la longitud
	email = (char *)malloc((strlen(em) + 1) * sizeof(char));
	if (email == NULL) {
		free(nombre);  // Free previously allocated memory
		printf("Error al asignar memoria para el cargo.\n");
		return -1;  // Error si no se pudo asignar memoria
	}
	strcpy(email, em);  // Copiar la cadena le�da en cargo

	printf("Telefono: ");
	fflush(stdin);
	scanf("%i", &telefono);

	printf("Contrasena: ");
	fflush(stdin);
	fgets(con, 75, stdin);
	con[strcspn(con, "\n")] = '\0'; // Fix: changed from cuentaBancaria to cuenBan
	// Asignar memoria din�mica para la cuenta bancaria seg�n la longitud
	contrasena = (char *)malloc((strlen(con) + 1) * sizeof(char));
	if (contrasena == NULL) {
		free(nombre);  // Free previously allocated memory
		free(email); // Free previously allocated memory
		printf("Error al asignar memoria para la cuenta bancaria.\n");
		return -1;  // Error si no se pudo asignar memoria
	}

	strcpy(contrasena, con); // Copiar la cadena le�da en cuenta bancaria
//	updateClientes(id_cl, nombre, email, telefono, contrasena, CLIENT_DB_PATH);
	return 0;
}

