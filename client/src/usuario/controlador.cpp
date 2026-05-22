/*
 * controlador.cpp
 *
 *  Created on: 19 may 2026
 *      Author: oier.artabe
 */

#include "controlador.h"
#include <iostream>

Controlador::Controlador()
    : cacheAeropuertosValida(false),
      cacheVuelosValida(false),
      cacheUsuariosValida(false),
      cacheEquipajesValida(false),
      salirPrograma(false) {}

bool Controlador::iniciar() {
    if (!cliente.conectar("127.0.0.1", 5000)) {
        return false;
    }

    std::cout << "Conectado al servidor correctamente\n";
    return true;
}

std::string Controlador::extraerContenido(const std::string &respuesta) {
    if (respuesta.rfind("DATA|", 0) == 0) {
        return respuesta.substr(5);
    }

    if (respuesta.rfind("OK|", 0) == 0) {
        return respuesta.substr(3);
    }

    return respuesta;
}

void Controlador::pausar() {
    std::cout << "\nPulsa ENTER para continuar...";
    std::cin.ignore();
    std::cin.get();
}

void Controlador::buscarVueloPorCodigo() {
    std::string codigo;

    std::cout << "\nCodigo del vuelo: ";
    std::getline(std::cin, codigo);

    if (codigo.empty()) {
        std::cout << "Codigo vacio\n";
        pausar();
        return;
    }

    std::string respuesta = cliente.enviarPeticion("BUSCAR_VUELO|" + codigo);
    std::cout << "\n" << extraerContenido(respuesta) << "\n";
    pausar();
}

void Controlador::buscarVuelosOrigenDestino() {
    std::string origen;
    std::string destino;

    std::cout << "\nCodigo aeropuerto origen: ";
    std::getline(std::cin, origen);

    std::cout << "Codigo aeropuerto destino: ";
    std::getline(std::cin, destino);

    if (origen.empty() || destino.empty()) {
        std::cout << "Origen o destino vacio\n";
        pausar();
        return;
    }

    std::string peticion = "BUSCAR_VUELOS_ORIGEN_DESTINO|" + origen + "|" + destino;
    std::string respuesta = cliente.enviarPeticion(peticion);

    std::cout << "\n" << extraerContenido(respuesta) << "\n";
    pausar();
}

void Controlador::comprarBillete() {
    std::string cod_vuelo;

    std::cout << "\nCodigo del vuelo que deseas comprar: ";
    std::getline(std::cin, cod_vuelo);

    if (cod_vuelo.empty()) {
        std::cout << "Codigo vacio\n";
        pausar();
        return;
    }

    std::string info = cliente.enviarPeticion("BUSCAR_VUELO|" + cod_vuelo);
    std::cout << "\n" << extraerContenido(info) << "\n";

    std::cout << "Confirmas la compra? (s/n): ";
    std::string confirmacion;
    std::getline(std::cin, confirmacion);

    if (confirmacion != "s" && confirmacion != "S") {
        std::cout << "Compra cancelada.\n";
        pausar();
        return;
    }

    std::string respuesta = cliente.enviarPeticion("COMPRAR_BILLETE|" + cod_vuelo);
    std::cout << "\n" << extraerContenido(respuesta) << "\n";
    pausar();
}

std::string Controlador::hacerLogin() {
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

        std::string respuesta = cliente.enviarPeticion("LOGIN|" + usuario + "|" + password);

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

void Controlador::mostrarMenuAdmin() {
    std::cout << "\n==============================\n";
    std::cout << "MENU ADMIN\n";
    std::cout << "==============================\n";
    std::cout << "1. Cargar CSV en BD\n";
    std::cout << "2. Ver aeropuertos\n";
    std::cout << "3. Ver vuelos\n";
    std::cout << "4. Ver pasajeros\n";
    std::cout << "5. Ver equipajes\n";
    std::cout << "6. Buscar vuelo por codigo\n";
    std::cout << "7. Buscar vuelos por origen y destino\n";
    std::cout << "8. Cambiar usuario\n";
    std::cout << "9. Salir\n";
    std::cout << "\nOpcion: ";
}

void Controlador::mostrarMenuEmpleado() {
    std::cout << "\n==============================\n";
    std::cout << "MENU EMPLEADO\n";
    std::cout << "==============================\n";
    std::cout << "1. Ver aeropuertos\n";
    std::cout << "2. Ver vuelos\n";
    std::cout << "3. Ver pasajeros\n";
    std::cout << "4. Ver equipajes\n";
    std::cout << "5. Buscar vuelo por codigo\n";
    std::cout << "6. Buscar vuelos por origen y destino\n";
    std::cout << "7. Cambiar usuario\n";
    std::cout << "8. Salir\n";
    std::cout << "\nOpcion: ";
}

void Controlador::mostrarMenuPasajero() {
    std::cout << "\n==============================\n";
    std::cout << "MENU PASAJERO\n";
    std::cout << "==============================\n";
    std::cout << "1. Ver vuelos\n";
    std::cout << "2. Ver equipajes\n";
    std::cout << "3. Buscar vuelo por codigo\n";
    std::cout << "4. Buscar vuelos por origen y destino\n";
    std::cout << "5. Comprar billete\n";
    std::cout << "6. Cambiar usuario\n";
    std::cout << "7. Salir\n";
    std::cout << "\nOpcion: ";
}

void Controlador::invalidarCache() {
    cacheAeropuertosValida = false;
    cacheVuelosValida = false;
    cacheUsuariosValida = false;
    cacheEquipajesValida = false;
}

void Controlador::ejecutarMenuAdmin() {
    int opcion = 0;

    do {
        mostrarMenuAdmin();
        if (!(std::cin >> opcion)) {
            std::cout << "\n[ERROR] Por favor, introduce un numero valido.\n";
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            pausar();
            continue;
        }
        std::cin.ignore(10000, '\n');

        switch (opcion) {
            case 1: {
                std::string respuesta = cliente.enviarPeticion("CARGAR_CSV");
                std::cout << "\n" << extraerContenido(respuesta) << "\n";
                invalidarCache();
                pausar();
                break;
            }

            case 2:
                if (!cacheAeropuertosValida) {
                    cacheAeropuertos = extraerContenido(cliente.enviarPeticion("LISTAR_AEROPUERTOS"));
                    cacheAeropuertosValida = true;
                }
                std::cout << cacheAeropuertos << "\n";
                pausar();
                break;

            case 3:
                if (!cacheVuelosValida) {
                    cacheVuelos = extraerContenido(cliente.enviarPeticion("LISTAR_VUELOS"));
                    cacheVuelosValida = true;
                }
                std::cout << cacheVuelos << "\n";
                pausar();
                break;

            case 4:
                if (!cacheUsuariosValida) {
                    cacheUsuarios = extraerContenido(cliente.enviarPeticion("LISTAR_USUARIOS"));
                    cacheUsuariosValida = true;
                }
                std::cout << cacheUsuarios << "\n";
                pausar();
                break;

            case 5:
                if (!cacheEquipajesValida) {
                    cacheEquipajes = extraerContenido(cliente.enviarPeticion("LISTAR_EQUIPAJES"));
                    cacheEquipajesValida = true;
                }
                std::cout << cacheEquipajes << "\n";
                pausar();
                break;

            case 6:
                buscarVueloPorCodigo();
                break;

            case 7:
                buscarVuelosOrigenDestino();
                break;

            case 8:
                std::cout << "Cambiando usuario...\n";
                break;

            case 9:
                std::cout << extraerContenido(cliente.enviarPeticion("SALIR")) << "\n";
                salirPrograma = true;
                break;

            default:
                std::cout << "Opcion no valida\n";
                pausar();
                break;
        }

    } while (opcion != 8 && opcion != 9);
}

void Controlador::ejecutarMenuEmpleado() {
    int opcion = 0;

    do {
        mostrarMenuEmpleado();
        if (!(std::cin >> opcion)) {
            std::cout << "\n[ERROR] Por favor, introduce un numero valido.\n";
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            pausar();
            continue;
        }
        std::cin.ignore(10000, '\n');

        switch (opcion) {
            case 1:
                if (!cacheAeropuertosValida) {
                    cacheAeropuertos = extraerContenido(cliente.enviarPeticion("LISTAR_AEROPUERTOS"));
                    cacheAeropuertosValida = true;
                }
                std::cout << cacheAeropuertos << "\n";
                pausar();
                break;

            case 2:
                if (!cacheVuelosValida) {
                    cacheVuelos = extraerContenido(cliente.enviarPeticion("LISTAR_VUELOS"));
                    cacheVuelosValida = true;
                }
                std::cout << cacheVuelos << "\n";
                pausar();
                break;

            case 3:
                if (!cacheUsuariosValida) {
                    cacheUsuarios = extraerContenido(cliente.enviarPeticion("LISTAR_USUARIOS"));
                    cacheUsuariosValida = true;
                }
                std::cout << cacheUsuarios << "\n";
                pausar();
                break;

            case 4:
                if (!cacheEquipajesValida) {
                    cacheEquipajes = extraerContenido(cliente.enviarPeticion("LISTAR_EQUIPAJES"));
                    cacheEquipajesValida = true;
                }
                std::cout << cacheEquipajes << "\n";
                pausar();
                break;

            case 5:
                buscarVueloPorCodigo();
                break;

            case 6:
                buscarVuelosOrigenDestino();
                break;

            case 7:
                std::cout << "Cambiando usuario...\n";
                break;

            case 8:
                std::cout << extraerContenido(cliente.enviarPeticion("SALIR")) << "\n";
                salirPrograma = true;
                break;

            default:
                std::cout << "Opcion no valida\n";
                pausar();
                break;
        }

    } while (opcion != 7 && opcion != 8);
}

void Controlador::ejecutarMenuPasajero() {
    int opcion = 0;

    do {
        mostrarMenuPasajero();
        
        if (!(std::cin >> opcion)) {
            std::cout << "\n[ERROR] Por favor, introduce un numero valido.\n";
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            pausar();
            continue;
        }
        std::cin.ignore(10000, '\n');

        switch (opcion) {
                    case 1:
                        if (!cacheVuelosValida) {
                            cacheVuelos = extraerContenido(cliente.enviarPeticion("LISTAR_VUELOS"));
                            cacheVuelosValida = true;
                        }
                        std::cout << cacheVuelos << "\n";
                        pausar();
                        break;

                    case 2:
                        if (!cacheEquipajesValida) {
                            cacheEquipajes = extraerContenido(cliente.enviarPeticion("LISTAR_EQUIPAJES"));
                            cacheEquipajesValida = true;
                        }
                        std::cout << cacheEquipajes << "\n";
                        pausar();
                        break;

                    case 3:
                        buscarVueloPorCodigo();
                        break;

                    case 4:
                        buscarVuelosOrigenDestino();
                        break;

                    case 5:
                        comprarBillete();
                        break;

                    case 6:
                        std::cout << "Cambiando usuario...\n";
                        break;

                    case 7:
                        std::cout << extraerContenido(cliente.enviarPeticion("SALIR")) << "\n";
                        salirPrograma = true;
                        break;

                    default:
                        std::cout << "Opcion no valida\n";
                        pausar();
                        break;
                }


    } while (opcion != 6 && opcion != 7);
}

void Controlador::ejecutar() {
    while (!salirPrograma) {
        std::string rol = hacerLogin();

        if (rol == "ADMIN") {
            ejecutarMenuAdmin();
        }
        else if (rol == "EMPLEADO") {
            ejecutarMenuEmpleado();
        }
        else if (rol == "PASAJERO") {
            ejecutarMenuPasajero();
        }
    }
}
