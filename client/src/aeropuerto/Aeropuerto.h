/*
 * Aeropuerto.h
 *
 *  Created on: 19 may 2026
 *      Author: oier.artabe
 */

#ifndef CLIENT_SRC_AEROPUERTO_AEROPUERTO_H_
#define CLIENT_SRC_AEROPUERTO_AEROPUERTO_H_

#include <string>

class Aeropuerto {
private:
    std::string codigo;
    std::string nombre;
    std::string ciudad;
    std::string pais;
    int num_pistas;

public:
    Aeropuerto();
    Aeropuerto(const std::string &codigo, const std::string &nombre,
               const std::string &ciudad, const std::string &pais,
               int num_pistas);

    std::string getCodigo()    const;
    std::string getNombre()    const;
    std::string getCiudad()    const;
    std::string getPais()      const;
    int         getNumPistas() const;

    void setCodigo(const std::string &codigo);
    void setNombre(const std::string &nombre);
    void setCiudad(const std::string &ciudad);
    void setPais(const std::string &pais);
    void setNumPistas(int num_pistas);

    void mostrar() const;
};

#endif /* CLIENT_SRC_AEROPUERTO_AEROPUERTO_H_ */
