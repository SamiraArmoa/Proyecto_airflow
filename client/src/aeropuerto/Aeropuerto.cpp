/*
 * Aeropuerto.cpp
 *
 *  Created on: 19 may 2026
 *      Author: oier.artabe
 */


#include "Aeropuerto.h"
#include <iostream>
#include <iomanip>

Aeropuerto::Aeropuerto()
    : codigo(""), nombre(""), ciudad(""), pais(""), num_pistas(0) {}

Aeropuerto::Aeropuerto(const std::string &codigo, const std::string &nombre,
                       const std::string &ciudad, const std::string &pais,
                       int num_pistas)
    : codigo(codigo), nombre(nombre), ciudad(ciudad),
      pais(pais), num_pistas(num_pistas) {}

std::string Aeropuerto::getCodigo()    const { return codigo; }
std::string Aeropuerto::getNombre()    const { return nombre; }
std::string Aeropuerto::getCiudad()    const { return ciudad; }
std::string Aeropuerto::getPais()      const { return pais; }
int         Aeropuerto::getNumPistas() const { return num_pistas; }

void Aeropuerto::setCodigo(const std::string &c)    { codigo     = c; }
void Aeropuerto::setNombre(const std::string &n)    { nombre     = n; }
void Aeropuerto::setCiudad(const std::string &ci)   { ciudad     = ci; }
void Aeropuerto::setPais(const std::string &p)      { pais       = p; }
void Aeropuerto::setNumPistas(int np)               { num_pistas = np; }

void Aeropuerto::mostrar() const {
    std::cout << std::left
              << std::setw(8)  << codigo
              << std::setw(40) << nombre
              << std::setw(25) << ciudad
              << std::setw(20) << pais
              << num_pistas    << std::endl;
}
