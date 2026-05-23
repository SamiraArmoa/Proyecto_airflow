#ifndef CONTROLADOR_H
#define CONTROLADOR_H

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

    int idUsuarioAutenticado;

    std::string extraerContenido(const std::string &respuesta);
    void pausar();

    std::string hacerLogin();

    void mostrarMenuAdmin();
    void mostrarMenuEmpleado();
    void mostrarMenuPasajero();

    void invalidarCache();

    void buscarVueloPorCodigo();
    void buscarVuelosOrigenDestino();
    void comprarBillete();

    void ejecutarMenuAdmin();
    void ejecutarMenuEmpleado();
    void ejecutarMenuPasajero();

public:
    Controlador();

    bool iniciar();
    void ejecutar();
};

#endif /* CONTROLADOR_H */