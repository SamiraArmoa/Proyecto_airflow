/*
 * Equipaje.cpp
 *
 *  Created on: 19 may 2026
 *      Author: oier.artabe
 */


#include "Equipaje.h"
#include <iostream>
#include <iomanip>

Equipaje::Equipaje()
    : id_equipaje(""), id_pasajero(""), tipo(""),
      peso_kg(0.0f), facturado(false) {}

Equipaje::Equipaje(const std::string &id_equipaje, const std::string &id_pasajero,
                   const std::string &tipo, float peso_kg, bool facturado)
    : id_equipaje(id_equipaje), id_pasajero(id_pasajero), tipo(tipo),
      peso_kg(peso_kg), facturado(facturado) {}

std::string Equipaje::getIdEquipaje()  const { return id_equipaje; }
std::string Equipaje::getIdPasajero()  const { return id_pasajero; }
std::string Equipaje::getTipo()        const { return tipo; }
float       Equipaje::getPesoKg()      const { return peso_kg; }
bool        Equipaje::getFacturado()   const { return facturado; }

void Equipaje::setIdEquipaje(const std::string &id) { id_equipaje = id; }
void Equipaje::setIdPasajero(const std::string &id) { id_pasajero = id; }
void Equipaje::setTipo(const std::string &t)        { tipo        = t; }
void Equipaje::setPesoKg(float p)                   { peso_kg     = p; }
void Equipaje::setFacturado(bool f)                 { facturado   = f; }

void Equipaje::mostrar() const {
    std::cout << std::left
              << std::setw(12) << id_equipaje
              << std::setw(12) << id_pasajero
              << std::setw(12) << tipo
              << std::setw(8)  << std::fixed << std::setprecision(2) << peso_kg
              << (facturado ? "Si" : "No") << std::endl;
}
