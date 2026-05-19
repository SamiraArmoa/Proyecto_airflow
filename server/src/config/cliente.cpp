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

std::string extraerContenido(const std::string &respuesta) {
    if (respuesta.rfind("DATA|", 0) == 0) {
        return respuesta.substr(5);
    }

    if (respuesta.rfind("OK|", 0) == 0) {
        return respuesta.substr(3);
    }

    return respuesta;
}

void pausar() {
    std::cout << "\nPulsa ENTER para continuar...";
    std::cin.ignore();
    std::cin.get();
}

void buscarVueloPorCodigo(SOCKET socketCliente) {
    std::string codigo;

    std::cout << "\nCodigo del vuelo: ";
    std::getline(std::cin, codigo);

    if (codigo.empty()) {
        std::cout << "Codigo vacio\n";
        pausar();
        return;
    }

    std::string respuesta = enviarPeticion(socketCliente, "BUSCAR_VUELO|" + codigo);
    std::cout << "\n" << extraerContenido(respuesta) << "\n";
    pausar();
}

std::string hacerLogin(SOCKET socketCliente) {
    std::string usuario;
    std::string password;

    while (true) {
        std::cout << "\n==============================\n";
        std::cout << "LOGIN\n";
        std::cout << "==============================\n";

        std::cout << "Usuario: ";
        std::getline(std::cin, usuario);

        std::cout << "Password: ";
        std::getline(std::cin, password);

        std::string peticion = "LOGIN|" + usuario + "|" + password;
        std::string respuesta = enviarPeticion(socketCliente, peticion);

        if (respuesta == "ROL|ADMIN") {
            std::cout << "\nLogin correcto. Rol: ADMIN\n";
            return "ADMIN";
        }

        if (respuesta == "ROL|EMPLEADO") {
            std::cout << "\nLogin correcto. Rol: EMPLEADO\n";
            return "EMPLEADO";
        }

        if (respuesta == "ROL|PASAJERO") {
            std::cout << "\nLogin correcto. Rol: PASAJERO\n";
            return "PASAJERO";
        }

        std::cout << "\n" << respuesta << "\n";
        std::cout << "Intentalo de nuevo.\n";
    }
}

void mostrarMenuAdmin() {
    std::cout << "\n==============================\n";
    std::cout << "MENU ADMIN\n";
    std::cout << "==============================\n";
    std::cout << "1. Cargar CSV en BD\n";
    std::cout << "2. Ver aeropuertos\n";
    std::cout << "3. Ver vuelos\n";
    std::cout << "4. Ver pasajeros\n";
    std::cout << "5. Ver equipajes\n";
    std::cout << "6. Buscar vuelo por codigo\n";
    std::cout << "7. Cambiar usuario\n";
    std::cout << "8. Salir\n";
    std::cout << "\nOpcion: ";
}

void mostrarMenuEmpleado() {
    std::cout << "\n==============================\n";
    std::cout << "MENU EMPLEADO\n";
    std::cout << "==============================\n";
    std::cout << "1. Ver aeropuertos\n";
    std::cout << "2. Ver vuelos\n";
    std::cout << "3. Ver pasajeros\n";
    std::cout << "4. Ver equipajes\n";
    std::cout << "5. Buscar vuelo por codigo\n";
    std::cout << "6. Cambiar usuario\n";
    std::cout << "7. Salir\n";
    std::cout << "\nOpcion: ";
}

void mostrarMenuPasajero() {
    std::cout << "\n==============================\n";
    std::cout << "MENU PASAJERO\n";
    std::cout << "==============================\n";
    std::cout << "1. Ver vuelos\n";
    std::cout << "2. Ver equipajes\n";
    std::cout << "3. Buscar vuelo por codigo\n";
    std::cout << "4. Cambiar usuario\n";
    std::cout << "5. Salir\n";
    std::cout << "\nOpcion: ";
}

void invalidarCache(bool &cacheAeropuertosValida,
                    bool &cacheVuelosValida,
                    bool &cacheUsuariosValida,
                    bool &cacheEquipajesValida) {
    cacheAeropuertosValida = false;
    cacheVuelosValida = false;
    cacheUsuariosValida = false;
    cacheEquipajesValida = false;
}

void ejecutarMenuAdmin(SOCKET socketCliente,
                       std::string &cacheAeropuertos,
                       std::string &cacheVuelos,
                       std::string &cacheUsuarios,
                       std::string &cacheEquipajes,
                       bool &cacheAeropuertosValida,
                       bool &cacheVuelosValida,
                       bool &cacheUsuariosValida,
                       bool &cacheEquipajesValida,
                       bool &salirPrograma) {
    int opcion = 0;

    do {
        mostrarMenuAdmin();

        std::cin >> opcion;
        std::cin.ignore();

        switch (opcion) {
            case 1: {
                std::string respuesta = enviarPeticion(socketCliente, "CARGAR_CSV");
                std::cout << "\n" << extraerContenido(respuesta) << "\n";

                invalidarCache(cacheAeropuertosValida,
                               cacheVuelosValida,
                               cacheUsuariosValida,
                               cacheEquipajesValida);

                pausar();
                break;
            }

            case 2: {
                if (!cacheAeropuertosValida) {
                    cacheAeropuertos = extraerContenido(enviarPeticion(socketCliente, "LISTAR_AEROPUERTOS"));
                    cacheAeropuertosValida = true;
                }

                std::cout << cacheAeropuertos << "\n";
                pausar();
                break;
            }

            case 3: {
                if (!cacheVuelosValida) {
                    cacheVuelos = extraerContenido(enviarPeticion(socketCliente, "LISTAR_VUELOS"));
                    cacheVuelosValida = true;
                }

                std::cout << cacheVuelos << "\n";
                pausar();
                break;
            }

            case 4: {
                if (!cacheUsuariosValida) {
                    cacheUsuarios = extraerContenido(enviarPeticion(socketCliente, "LISTAR_USUARIOS"));
                    cacheUsuariosValida = true;
                }

                std::cout << cacheUsuarios << "\n";
                pausar();
                break;
            }

            case 5: {
                if (!cacheEquipajesValida) {
                    cacheEquipajes = extraerContenido(enviarPeticion(socketCliente, "LISTAR_EQUIPAJES"));
                    cacheEquipajesValida = true;
                }

                std::cout << cacheEquipajes << "\n";
                pausar();
                break;
            }

            case 6:
                buscarVueloPorCodigo(socketCliente);
                break;

            case 7:
                std::cout << "Cambiando usuario...\n";
                break;

            case 8: {
                std::string respuesta = enviarPeticion(socketCliente, "SALIR");
                std::cout << extraerContenido(respuesta) << "\n";
                salirPrograma = true;
                break;
            }

            default:
                std::cout << "Opcion no valida\n";
                pausar();
                break;
        }

    } while (opcion != 7 && opcion != 8);
}

void ejecutarMenuEmpleado(SOCKET socketCliente,
                          std::string &cacheAeropuertos,
                          std::string &cacheVuelos,
                          std::string &cacheUsuarios,
                          std::string &cacheEquipajes,
                          bool &cacheAeropuertosValida,
                          bool &cacheVuelosValida,
                          bool &cacheUsuariosValida,
                          bool &cacheEquipajesValida,
                          bool &salirPrograma) {
    int opcion = 0;

    do {
        mostrarMenuEmpleado();

        std::cin >> opcion;
        std::cin.ignore();

        switch (opcion) {
            case 1: {
                if (!cacheAeropuertosValida) {
                    cacheAeropuertos = extraerContenido(enviarPeticion(socketCliente, "LISTAR_AEROPUERTOS"));
                    cacheAeropuertosValida = true;
                }

                std::cout << cacheAeropuertos << "\n";
                pausar();
                break;
            }

            case 2: {
                if (!cacheVuelosValida) {
                    cacheVuelos = extraerContenido(enviarPeticion(socketCliente, "LISTAR_VUELOS"));
                    cacheVuelosValida = true;
                }

                std::cout << cacheVuelos << "\n";
                pausar();
                break;
            }

            case 3: {
                if (!cacheUsuariosValida) {
                    cacheUsuarios = extraerContenido(enviarPeticion(socketCliente, "LISTAR_USUARIOS"));
                    cacheUsuariosValida = true;
                }

                std::cout << cacheUsuarios << "\n";
                pausar();
                break;
            }

            case 4: {
                if (!cacheEquipajesValida) {
                    cacheEquipajes = extraerContenido(enviarPeticion(socketCliente, "LISTAR_EQUIPAJES"));
                    cacheEquipajesValida = true;
                }

                std::cout << cacheEquipajes << "\n";
                pausar();
                break;
            }

            case 5:
                buscarVueloPorCodigo(socketCliente);
                break;

            case 6:
                std::cout << "Cambiando usuario...\n";
                break;

            case 7: {
                std::string respuesta = enviarPeticion(socketCliente, "SALIR");
                std::cout << extraerContenido(respuesta) << "\n";
                salirPrograma = true;
                break;
            }

            default:
                std::cout << "Opcion no valida\n";
                pausar();
                break;
        }

    } while (opcion != 6 && opcion != 7);
}

void ejecutarMenuPasajero(SOCKET socketCliente,
                          std::string &cacheVuelos,
                          std::string &cacheEquipajes,
                          bool &cacheVuelosValida,
                          bool &cacheEquipajesValida,
                          bool &salirPrograma) {
    int opcion = 0;

    do {
        mostrarMenuPasajero();

        std::cin >> opcion;
        std::cin.ignore();

        switch (opcion) {
            case 1: {
                if (!cacheVuelosValida) {
                    cacheVuelos = extraerContenido(enviarPeticion(socketCliente, "LISTAR_VUELOS"));
                    cacheVuelosValida = true;
                }

                std::cout << cacheVuelos << "\n";
                pausar();
                break;
            }

            case 2: {
                if (!cacheEquipajesValida) {
                    cacheEquipajes = extraerContenido(enviarPeticion(socketCliente, "LISTAR_EQUIPAJES"));
                    cacheEquipajesValida = true;
                }

                std::cout << cacheEquipajes << "\n";
                pausar();
                break;
            }

            case 3:
                buscarVueloPorCodigo(socketCliente);
                break;

            case 4:
                std::cout << "Cambiando usuario...\n";
                break;

            case 5: {
                std::string respuesta = enviarPeticion(socketCliente, "SALIR");
                std::cout << extraerContenido(respuesta) << "\n";
                salirPrograma = true;
                break;
            }

            default:
                std::cout << "Opcion no valida\n";
                pausar();
                break;
        }

    } while (opcion != 4 && opcion != 5);
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

    bool salirPrograma = false;

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

    while (!salirPrograma) {
        std::string rol = hacerLogin(socketCliente);

        if (rol == "ADMIN") {
            ejecutarMenuAdmin(socketCliente,
                              cacheAeropuertos,
                              cacheVuelos,
                              cacheUsuarios,
                              cacheEquipajes,
                              cacheAeropuertosValida,
                              cacheVuelosValida,
                              cacheUsuariosValida,
                              cacheEquipajesValida,
                              salirPrograma);
        }
        else if (rol == "EMPLEADO") {
            ejecutarMenuEmpleado(socketCliente,
                                 cacheAeropuertos,
                                 cacheVuelos,
                                 cacheUsuarios,
                                 cacheEquipajes,
                                 cacheAeropuertosValida,
                                 cacheVuelosValida,
                                 cacheUsuariosValida,
                                 cacheEquipajesValida,
                                 salirPrograma);
        }
        else if (rol == "PASAJERO") {
            ejecutarMenuPasajero(socketCliente,
                                 cacheVuelos,
                                 cacheEquipajes,
                                 cacheVuelosValida,
                                 cacheEquipajesValida,
                                 salirPrograma);
        }
    }

    closesocket(socketCliente);
    WSACleanup();

    return 0;
}