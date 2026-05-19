/*
 * controlador.cpp
 *
 *  Created on: 19 may 2026
 *      Author: oier.artabe
 */


#include "clientSocket.h"
#include <iostream>
#include <cstring>

#define BUFFER_SIZE 65536

ClientSocket::ClientSocket() {
    socketCliente = INVALID_SOCKET;
}

ClientSocket::~ClientSocket() {
    cerrar();
}

bool ClientSocket::conectar(const std::string &ip, int puerto) {
    WSADATA wsaData;
    sockaddr_in servidorAddr;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cout << "Error inicializando Winsock\n";
        return false;
    }

    socketCliente = socket(AF_INET, SOCK_STREAM, 0);

    if (socketCliente == INVALID_SOCKET) {
        std::cout << "Error creando socket cliente\n";
        WSACleanup();
        return false;
    }

    servidorAddr.sin_family = AF_INET;
    servidorAddr.sin_addr.s_addr = inet_addr(ip.c_str());
    servidorAddr.sin_port = htons(puerto);

    if (connect(socketCliente, (sockaddr *)&servidorAddr, sizeof(servidorAddr)) == SOCKET_ERROR) {
        std::cout << "No se pudo conectar con el servidor\n";
        std::cout << "Asegurate de ejecutar primero servidor.exe\n";
        cerrar();
        return false;
    }

    return true;
}

std::string ClientSocket::enviarPeticion(const std::string &peticion) {
    char buffer[BUFFER_SIZE];

    send(socketCliente, peticion.c_str(), (int)peticion.length(), 0);

    memset(buffer, 0, sizeof(buffer));

    int bytesRecibidos = recv(socketCliente, buffer, sizeof(buffer) - 1, 0);

    if (bytesRecibidos <= 0) {
        return "ERROR|No se recibio respuesta del servidor";
    }

    buffer[bytesRecibidos] = '\0';

    return std::string(buffer);
}

void ClientSocket::cerrar() {
    if (socketCliente != INVALID_SOCKET) {
        closesocket(socketCliente);
        socketCliente = INVALID_SOCKET;
    }

    WSACleanup();
}

SOCKET ClientSocket::getSocket() const {
    return socketCliente;
}