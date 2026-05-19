/*
 * controlador.h
 *
 *  Created on: 19 may 2026
 *      Author: oier.artabe
 */

#ifndef CLIENT_SRC_USUARIO_CONTROLADOR_H_
#define CLIENT_SRC_USUARIO_CONTROLADOR_H_

#include <string>
#include "clientSocket.h"

class Controlador {
private:
    ClientSocket cliente;

    std::string cacheAeropuertos;
    std::string cacheVuelos;
    std::string cacheUsuarios;
    std::string cacheEquipajes;

    bool cacheAeropuertosValida;
    bool cacheVuelosValida;
    bool cacheUsuariosValida;
    bool cacheEquipajesValida;

    bool salirPrograma;

    std::string extraerContenido(const std::string &respuesta);
    void pausar();

    std::string hacerLogin();

    void mostrarMenuAdmin();
    void mostrarMenuEmpleado();
    void mostrarMenuPasajero();

    void invalidarCache();

    void buscarVueloPorCodigo();

    void ejecutarMenuAdmin();
    void ejecutarMenuEmpleado();
    void ejecutarMenuPasajero();

public:
    Controlador();

    bool iniciar();
    void ejecutar();
};

#endif /* CLIENT_SRC_USUARIO_CONTROLADOR_H_ */
