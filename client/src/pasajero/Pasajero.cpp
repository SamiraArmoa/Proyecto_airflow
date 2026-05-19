/*
 * Pasajero.cpp
 *
 *  Created on: 19 may 2026
 *      Author: oier.artabe
 */


#include "Pasajero.h"
#include <iostream>
#include <iomanip>
#include <algorithm>


Pasajero::Pasajero()
    : dni(""), nombre(""), apellido(""), email(""), telefono("") {}

Pasajero::Pasajero(const std::string &dni,
                   const std::string &nombre,
                   const std::string &apellido,
                   const std::string &email,
                   const std::string &telefono)
    : dni(dni), nombre(nombre), apellido(apellido),
      email(email), telefono(telefono) {}

std::string Pasajero::getDni() const { return dni; }
std::string Pasajero::getNombre() const { return nombre; }
std::string Pasajero::getApellido() const { return apellido; }
std::string Pasajero::getEmail() const { return email; }
std::string Pasajero::getTelefono() const { return telefono; }
std::vector<std::string> Pasajero::getVuelos() const { return vuelos; }

void Pasajero::setDni(const std::string &d) { dni = d; }
void Pasajero::setNombre(const std::string &n) { nombre = n; }
void Pasajero::setApellido(const std::string &a) { apellido = a; }
void Pasajero::setEmail(const std::string &e) { email = e; }
void Pasajero::setTelefono(const std::string &t) { telefono = t; }

void Pasajero::agregarVuelo(const std::string &idVuelo) {
    if (std::find(vuelos.begin(), vuelos.end(), idVuelo) == vuelos.end()) {
        vuelos.push_back(idVuelo);
    }
}

void Pasajero::quitarVuelo(const std::string &idVuelo) {
    vuelos.erase(
        std::remove(vuelos.begin(), vuelos.end(), idVuelo),
        vuelos.end()
    );
}

void Pasajero::mostrar() const {
    std::cout << std::left
              << std::setw(12) << dni
              << std::setw(15) << nombre
              << std::setw(20) << apellido
              << std::setw(30) << email
              << std::setw(15) << telefono;

    if (vuelos.empty()) {
        std::cout << "-";
    } else {
        for (size_t i = 0; i < vuelos.size(); i++) {
            if (i > 0) std::cout << "|";
            std::cout << vuelos[i];
        }
    }

    std::cout << std::endl;
}