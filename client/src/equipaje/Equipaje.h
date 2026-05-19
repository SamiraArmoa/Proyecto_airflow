/*
 * Equipaje.h
 *
 *  Created on: 19 may 2026
 *      Author: oier.artabe
 */

#ifndef CLIENT_SRC_EQUIPAJE_EQUIPAJE_H_
#define CLIENT_SRC_EQUIPAJE_EQUIPAJE_H_

#include <string>

class Equipaje {
private:
    std::string id_equipaje;
    std::string id_pasajero;
    std::string tipo;
    float peso_kg;
    bool facturado;

public:
    Equipaje();
    Equipaje(const std::string &id_equipaje, const std::string &id_pasajero,
             const std::string &tipo, float peso_kg, bool facturado);

    std::string getIdEquipaje()  const;
    std::string getIdPasajero()  const;
    std::string getTipo()        const;
    float       getPesoKg()      const;
    bool        getFacturado()   const;

    void setIdEquipaje(const std::string &id);
    void setIdPasajero(const std::string &id);
    void setTipo(const std::string &tipo);
    void setPesoKg(float peso);
    void setFacturado(bool facturado);

    void mostrar() const;
};


#endif /* CLIENT_SRC_EQUIPAJE_EQUIPAJE_H_ */
