/*
 * Vuelo.cpp
 *
 *  Created on: 19 may 2026
 *      Author: oier.artabe
 */


#include "Vuelo.h"
#include <iostream>
#include <iomanip>

Vuelo::Vuelo()
    : id_vuelo(""), aerolinea(""), origen(""), destino(""),
      fecha(""), hora_salida(""), hora_llegada(""),
      capacidad(0), asientos_disponibles(0) {}

Vuelo::Vuelo(const std::string &id_vuelo, const std::string &aerolinea,
             const std::string &origen,   const std::string &destino,
             const std::string &fecha,    const std::string &hora_salida,
             const std::string &hora_llegada,
             int capacidad, int asientos_disponibles)
    : id_vuelo(id_vuelo), aerolinea(aerolinea), origen(origen),
      destino(destino), fecha(fecha), hora_salida(hora_salida),
      hora_llegada(hora_llegada), capacidad(capacidad),
      asientos_disponibles(asientos_disponibles) {}

std::string Vuelo::getIdVuelo()             const { return id_vuelo; }
std::string Vuelo::getAerolinea()           const { return aerolinea; }
std::string Vuelo::getOrigen()              const { return origen; }
std::string Vuelo::getDestino()             const { return destino; }
std::string Vuelo::getFecha()               const { return fecha; }
std::string Vuelo::getHoraSalida()          const { return hora_salida; }
std::string Vuelo::getHoraLlegada()         const { return hora_llegada; }
int         Vuelo::getCapacidad()           const { return capacidad; }
int         Vuelo::getAsientosDisponibles() const { return asientos_disponibles; }

void Vuelo::setIdVuelo(const std::string &id)   { id_vuelo  = id; }
void Vuelo::setAerolinea(const std::string &a)  { aerolinea = a; }
void Vuelo::setOrigen(const std::string &o)     { origen    = o; }
void Vuelo::setDestino(const std::string &d)    { destino   = d; }
void Vuelo::setFecha(const std::string &f)      { fecha     = f; }
void Vuelo::setHoraSalida(const std::string &hs)  { hora_salida  = hs; }
void Vuelo::setHoraLlegada(const std::string &hl) { hora_llegada = hl; }
void Vuelo::setCapacidad(int c)                 { capacidad = c; }
void Vuelo::setAsientosDisponibles(int a)       { asientos_disponibles = a; }

void Vuelo::mostrar() const {
    std::cout << std::left
              << std::setw(12) << id_vuelo
              << std::setw(20) << aerolinea
              << std::setw(6)  << origen
              << std::setw(6)  << destino
              << std::setw(12) << fecha
              << std::setw(7)  << hora_salida
              << std::setw(7)  << hora_llegada
              << capacidad << "/" << asientos_disponibles
              << std::endl;
}
