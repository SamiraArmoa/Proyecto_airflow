/*
 * Pasajero.h
 *
 *  Created on: 19 may 2026
 *      Author: oier.artabe
 */

#ifndef CLIENT_SRC_PASAJERO_PASAJERO_H_
#define CLIENT_SRC_PASAJERO_PASAJERO_H_

#include <string>
#include <vector>

class Pasajero {
private:
    std::string dni;
    std::string nombre;
    std::string apellido;
    std::string email;
    std::string telefono;
    std::vector<std::string> vuelos;

public:
    Pasajero();

    Pasajero(const std::string &dni,
             const std::string &nombre,
             const std::string &apellido,
             const std::string &email,
             const std::string &telefono);

    std::string getDni() const;
    std::string getNombre() const;
    std::string getApellido() const;
    std::string getEmail() const;
    std::string getTelefono() const;
    std::vector<std::string> getVuelos() const;

    void setDni(const std::string &dni);
    void setNombre(const std::string &nombre);
    void setApellido(const std::string &apellido);
    void setEmail(const std::string &email);
    void setTelefono(const std::string &telefono);

    void agregarVuelo(const std::string &idVuelo);
    void quitarVuelo(const std::string &idVuelo);

    void mostrar() const;
};

#endif /* CLIENT_SRC_PASAJERO_PASAJERO_H_ */
