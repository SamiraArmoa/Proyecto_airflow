/*
 * Vuelo.h
 *
 *  Created on: 19 may 2026
 *      Author: oier.artabe
 */

#ifndef CLIENT_SRC_VUELO_VUELO_H_
#define CLIENT_SRC_VUELO_VUELO_H_




#include <string>

class Vuelo {
private:
    std::string id_vuelo;
    std::string aerolinea;
    std::string origen;
    std::string destino;
    std::string fecha;
    std::string hora_salida;
    std::string hora_llegada;
    int capacidad;
    int asientos_disponibles;

public:
    Vuelo();
    Vuelo(const std::string &id_vuelo, const std::string &aerolinea,
          const std::string &origen,   const std::string &destino,
          const std::string &fecha,    const std::string &hora_salida,
          const std::string &hora_llegada,
          int capacidad, int asientos_disponibles);

    std::string getIdVuelo()            const;
    std::string getAerolinea()          const;
    std::string getOrigen()             const;
    std::string getDestino()            const;
    std::string getFecha()              const;
    std::string getHoraSalida()         const;
    std::string getHoraLlegada()        const;
    int         getCapacidad()          const;
    int         getAsientosDisponibles() const;

    void setIdVuelo(const std::string &id);
    void setAerolinea(const std::string &a);
    void setOrigen(const std::string &o);
    void setDestino(const std::string &d);
    void setFecha(const std::string &f);
    void setHoraSalida(const std::string &hs);
    void setHoraLlegada(const std::string &hl);
    void setCapacidad(int c);
    void setAsientosDisponibles(int a);

    void mostrar() const;
};

#endif /* CLIENT_SRC_PASAJERO_VUELO_H_ */
