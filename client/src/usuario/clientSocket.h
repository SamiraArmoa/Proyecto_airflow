/*
 * clientSocket.h
 *
 *  Created on: 19 may 2026
 *      Author: oier.artabe
 */

#ifndef CLIENT_SRC_USUARIO_CLIENTSOCKET_H_
#define CLIENT_SRC_USUARIO_CLIENTSOCKET_H_

#include <string>
#include <winsock2.h>
#include <windows.h>

class ClientSocket {
private:
    SOCKET socketCliente;

public:
    ClientSocket();
    ~ClientSocket();

    bool conectar(const std::string &ip, int puerto);
    std::string enviarPeticion(const std::string &peticion);
    void cerrar();

    SOCKET getSocket() const;
};

#endif /* CLIENT_SRC_USUARIO_CLIENTSOCKET_H_ */
