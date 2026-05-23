#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <ctime>
#include <vector>
#include <sstream>
#include <cstdlib>

#include <winsock2.h>
#include <windows.h>

extern "C" {
    #include "config.h"
    #include "../../data/db/db.h"
}

#pragma comment(lib, "ws2_32.lib")

#define BUFFER_SIZE 4096

static Config configSistema;
static void *db = NULL;
static int idUsuarioActual = -1;

std::vector<std::string> dividir(const std::string &texto, char separador) {
    std::vector<std::string> partes;
    std::stringstream ss(texto);
    std::string item;

    while (std::getline(ss, item, separador)) {
        partes.push_back(item);
    }

    return partes;
}

void escribirLog(const std::string &mensaje) {
    std::ofstream log(configSistema.ruta_log, std::ios::app);

    if (!log.is_open()) {
        std::cout << "[LOG ERROR] No se pudo abrir el fichero de log\n";
        return;
    }

    time_t ahora = time(NULL);
    tm *tiempo = localtime(&ahora);

    char fecha[100];
    strftime(fecha, sizeof(fecha), "%Y-%m-%d %H:%M:%S", tiempo);

    log << "[" << fecha << "] " << mensaje << std::endl;
    log.close();
}

void enviarRespuesta(SOCKET cliente, const std::string &respuesta) {
    send(cliente, respuesta.c_str(), (int)respuesta.length(), 0);
}

std::string procesarLogin(const std::string &peticion) {
    std::vector<std::string> partes = dividir(peticion, '|');

    if (partes.size() != 3) {
        escribirLog("Intento de login con formato incorrecto");
        return "ERROR|Formato login incorrecto";
    }

    std::string usuario = partes[1];
    std::string password = partes[2];

    if (usuario == configSistema.admin_user && password == configSistema.admin_pass) {
        escribirLog("Login correcto: ADMIN");
        idUsuarioActual = -1;
        return "ROL|ADMIN";
    }

    if (usuario == configSistema.empleado_user && password == configSistema.empleado_pass) {
        escribirLog("Login correcto: EMPLEADO");
        idUsuarioActual = -1;
        return "ROL|EMPLEADO";
    }

    if (usuario == configSistema.pasajero_user &&
    password == configSistema.pasajero_pass) {

    escribirLog("Login correcto: PASAJERO");

    idUsuarioActual = 1;

    return "ROL|PASAJERO|1";
}

    escribirLog("Login incorrecto para usuario: " + usuario);
    return "ERROR|Login incorrecto";
}

std::string procesarPeticion(const std::string &peticion) {
    escribirLog("Peticion recibida: " + peticion);

    if (peticion.rfind("LOGIN|", 0) == 0) {
        return procesarLogin(peticion);
    }

    if (peticion == "LISTAR_AEROPUERTOS") {
        char *texto = db_aeropuerto_listar_texto(db);

        if (!texto) {
            return "ERROR|No se pudieron listar aeropuertos";
        }

        std::string respuesta = texto;
        free(texto);
        return "DATA|" + respuesta;
    }

    if (peticion == "LISTAR_VUELOS") {
        char *texto = db_vuelo_listar_texto(db);

        if (!texto) {
            return "ERROR|No se pudieron listar vuelos";
        }

        std::string respuesta = texto;
        free(texto);
        return "DATA|" + respuesta;
    }

    if (peticion == "LISTAR_USUARIOS") {
        char *texto = db_usuario_listar_texto(db);

        if (!texto) {
            return "ERROR|No se pudieron listar usuarios";
        }

        std::string respuesta = texto;
        free(texto);
        return "DATA|" + respuesta;
    }

    if (peticion == "LISTAR_EQUIPAJES") {
        char *texto = db_equipaje_listar_texto(db);

        if (!texto) {
            return "ERROR|No se pudieron listar equipajes";
        }

        std::string respuesta = texto;
        free(texto);
        return "DATA|" + respuesta;
    }

    if (peticion.rfind("BUSCAR_VUELO|", 0) == 0) {
        std::vector<std::string> partes = dividir(peticion, '|');

        if (partes.size() != 2) {
            return "ERROR|Formato incorrecto. Uso: BUSCAR_VUELO|codigo";
        }

        char *texto = db_vuelo_buscar_codigo_texto(db, partes[1].c_str());

        if (!texto) {
            return "ERROR|No se pudo buscar el vuelo";
        }

        std::string respuesta = texto;
        free(texto);

        if (respuesta.rfind("ERROR|", 0) == 0) {
            return respuesta;
        }

        return "DATA|" + respuesta;
    }

    if (peticion.rfind("BUSCAR_VUELOS_ORIGEN_DESTINO|", 0) == 0) {
        std::vector<std::string> partes = dividir(peticion, '|');

        if (partes.size() != 3) {
            return "ERROR|Formato incorrecto. Uso: BUSCAR_VUELOS_ORIGEN_DESTINO|origen|destino";
        }

        char *texto = db_vuelos_buscar_origen_destino_texto(
            db,
            partes[1].c_str(),
            partes[2].c_str()
        );

        if (!texto) {
            return "ERROR|No se pudieron buscar vuelos por origen y destino";
        }

        std::string respuesta = texto;
        free(texto);

        if (respuesta.rfind("ERROR|", 0) == 0) {
            return respuesta;
        }

        return "DATA|" + respuesta;
    }

    if (peticion == "CARGAR_CSV") {
        int a = db_cargar_aeropuertos_csv(db, configSistema.ruta_aeropuertos);
        int v = db_cargar_vuelos_csv(db, configSistema.ruta_vuelos);
        int u = db_cargar_usuarios_csv(db, configSistema.ruta_pasajeros);
        int e = db_cargar_equipajes_csv(db, configSistema.ruta_equipajes);

        std::string respuesta = "OK|CSV cargados en BD\n";
        respuesta += "Aeropuertos: " + std::to_string(a) + "\n";
        respuesta += "Vuelos: " + std::to_string(v) + "\n";
        respuesta += "Usuarios: " + std::to_string(u) + "\n";
        respuesta += "Equipajes: " + std::to_string(e) + "\n";

        return respuesta;
    }

    if (peticion.rfind("COMPRAR_BILLETE|", 0) == 0) {
        std::vector<std::string> partes = dividir(peticion, '|');

        if (partes.size() != 3) {
            return "ERROR|Formato incorrecto. Uso: COMPRAR_BILLETE|idUsuario|cod_vuelo";
        }

        int idUsuario = atoi(partes[1].c_str());

        if (idUsuario <= 0) {
            return "ERROR|ID de usuario invalido";
        }

        char *texto = db_comprar_billete_texto(
            db,
            idUsuario,
            partes[2].c_str()
        );

        if (!texto) {
            return "ERROR|Error interno al comprar billete";
        }

        std::string respuesta = texto;
        free(texto);

        return respuesta;
    }

    if (peticion == "SALIR") {
        idUsuarioActual = -1;
        return "OK|Cliente desconectado";
    }

    return "ERROR|Peticion no reconocida";
}

int main() {
    WSADATA wsaData;
    SOCKET socketServidor;
    SOCKET socketCliente;
    sockaddr_in servidorAddr;
    sockaddr_in clienteAddr;
    int clienteTam = sizeof(clienteAddr);

    char buffer[BUFFER_SIZE];

    if (cargarConfig("src/config.txt", &configSistema) != 0) {
        std::cout << "Error cargando config.txt\n";
        return 1;
    }

    db = db_abrir(configSistema.ruta_db);

    if (!db) {
        std::cout << "Error abriendo la base de datos\n";
        return 1;
    }

    db_activar_fk(db);
    db_inicializar(db);

    escribirLog("Servidor iniciado");
    escribirLog("Base de datos abierta correctamente");

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cout << "Error inicializando Winsock\n";
        db_cerrar(db);
        return 1;
    }

    socketServidor = socket(AF_INET, SOCK_STREAM, 0);

    if (socketServidor == INVALID_SOCKET) {
        std::cout << "Error creando socket servidor\n";
        WSACleanup();
        db_cerrar(db);
        return 1;
    }

    servidorAddr.sin_family = AF_INET;
    servidorAddr.sin_addr.s_addr = INADDR_ANY;
    servidorAddr.sin_port = htons(configSistema.puerto_servidor);

    if (bind(socketServidor, (sockaddr *)&servidorAddr, sizeof(servidorAddr)) == SOCKET_ERROR) {
        std::cout << "Error haciendo bind en el puerto " << configSistema.puerto_servidor << "\n";
        closesocket(socketServidor);
        WSACleanup();
        db_cerrar(db);
        return 1;
    }

    if (listen(socketServidor, 1) == SOCKET_ERROR) {
        std::cout << "Error escuchando conexiones\n";
        closesocket(socketServidor);
        WSACleanup();
        db_cerrar(db);
        return 1;
    }

    std::cout << "Servidor escuchando en puerto " << configSistema.puerto_servidor << "...\n";
    escribirLog("Servidor escuchando en puerto " + std::to_string(configSistema.puerto_servidor));

    while (true) {
        std::cout << "Esperando cliente...\n";
        escribirLog("Esperando cliente");

        clienteTam = sizeof(clienteAddr);
        socketCliente = accept(socketServidor, (sockaddr *)&clienteAddr, &clienteTam);

        if (socketCliente == INVALID_SOCKET) {
            std::cout << "Error aceptando cliente\n";
            escribirLog("Error aceptando cliente");
            continue;
        }

        std::cout << "Cliente conectado\n";
        escribirLog("Cliente conectado");

        bool salirCliente = false;

        while (!salirCliente) {
            memset(buffer, 0, sizeof(buffer));

            int bytesRecibidos = recv(socketCliente, buffer, sizeof(buffer) - 1, 0);

            if (bytesRecibidos <= 0) {
                std::cout << "Cliente desconectado\n";
                escribirLog("Cliente desconectado");
                break;
            }

            buffer[bytesRecibidos] = '\0';

            std::string peticion(buffer);

            if (peticion == "SALIR") {
                salirCliente = true;
            }

            std::string respuesta = procesarPeticion(peticion);
            enviarRespuesta(socketCliente, respuesta);
        }

        closesocket(socketCliente);
        idUsuarioActual = -1;

        std::cout << "Conexion con cliente cerrada\n";
        escribirLog("Conexion con cliente cerrada");
    }

    closesocket(socketServidor);
    WSACleanup();

    db_cerrar(db);

    escribirLog("Servidor cerrado");

    return 0;
}