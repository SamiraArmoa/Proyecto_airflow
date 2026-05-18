#include <iostream>
#include <string>
#include <cstring>

#include <winsock2.h>
#include <windows.h>

#pragma comment(lib, "ws2_32.lib")

#define BUFFER_SIZE 65536

std::string enviarPeticion(SOCKET socketCliente, const std::string &peticion) {
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

void mostrarMenu() {
    std::cout << "\n==============================\n";
    std::cout << "MENU CLIENTE\n";
    std::cout << "==============================\n";
    std::cout << "1. Cargar CSV en BD\n";
    std::cout << "2. Ver aeropuertos\n";
    std::cout << "3. Ver vuelos\n";
    std::cout << "4. Ver pasajeros\n";
    std::cout << "5. Ver equipajes\n";
    std::cout << "6. Salir\n";
    std::cout << "\nOpcion: ";
}

void pausar() {
    std::cout << "\nPulsa ENTER para continuar...";
    std::cin.ignore();
    std::cin.get();
}

int main() {
    WSADATA wsaData;
    SOCKET socketCliente;
    sockaddr_in servidorAddr;

    std::string cacheAeropuertos;
    std::string cacheVuelos;
    std::string cacheUsuarios;
    std::string cacheEquipajes;

    bool cacheAeropuertosValida = false;
    bool cacheVuelosValida = false;
    bool cacheUsuariosValida = false;
    bool cacheEquipajesValida = false;

    int opcion = 0;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cout << "Error inicializando Winsock\n";
        return 1;
    }

    socketCliente = socket(AF_INET, SOCK_STREAM, 0);

    if (socketCliente == INVALID_SOCKET) {
        std::cout << "Error creando socket cliente\n";
        WSACleanup();
        return 1;
    }

    servidorAddr.sin_family = AF_INET;
    servidorAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servidorAddr.sin_port = htons(5000);

    if (connect(socketCliente, (sockaddr *)&servidorAddr, sizeof(servidorAddr)) == SOCKET_ERROR) {
        std::cout << "No se pudo conectar con el servidor\n";
        std::cout << "Asegurate de ejecutar primero servidor.exe\n";
        closesocket(socketCliente);
        WSACleanup();
        return 1;
    }

    std::cout << "Conectado al servidor correctamente\n";

    do {
        mostrarMenu();

        std::cin >> opcion;
        std::cin.ignore();

        switch (opcion) {
            case 1: {
                std::string respuesta = enviarPeticion(socketCliente, "CARGAR_CSV");
                std::cout << "\n" << respuesta << "\n";

                cacheAeropuertosValida = false;
                cacheVuelosValida = false;
                cacheUsuariosValida = false;
                cacheEquipajesValida = false;

                pausar();
                break;
            }

            case 2: {
                if (!cacheAeropuertosValida) {
                    cacheAeropuertos = enviarPeticion(socketCliente, "LISTAR_AEROPUERTOS");
                    cacheAeropuertosValida = true;
                }

                std::cout << cacheAeropuertos << "\n";
                pausar();
                break;
            }

            case 3: {
                if (!cacheVuelosValida) {
                    cacheVuelos = enviarPeticion(socketCliente, "LISTAR_VUELOS");
                    cacheVuelosValida = true;
                }

                std::cout << cacheVuelos << "\n";
                pausar();
                break;
            }

            case 4: {
                if (!cacheUsuariosValida) {
                    cacheUsuarios = enviarPeticion(socketCliente, "LISTAR_USUARIOS");
                    cacheUsuariosValida = true;
                }

                std::cout << cacheUsuarios << "\n";
                pausar();
                break;
            }

            case 5: {
                if (!cacheEquipajesValida) {
                    cacheEquipajes = enviarPeticion(socketCliente, "LISTAR_EQUIPAJES");
                    cacheEquipajesValida = true;
                }

                std::cout << cacheEquipajes << "\n";
                pausar();
                break;
            }

            case 6: {
                std::string respuesta = enviarPeticion(socketCliente, "SALIR");
                std::cout << respuesta << "\n";
                break;
            }

            default:
                std::cout << "Opcion no valida\n";
                pausar();
                break;
        }

    } while (opcion != 6);

    closesocket(socketCliente);
    WSACleanup();

    return 0;
}