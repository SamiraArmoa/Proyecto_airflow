/*
 * main.cpp
 *
 *  Created on: 19 may 2026
 *      Author: oier.artabe
 */


#include "controlador.h"

int main() {
    Controlador controlador;

    if (!controlador.iniciar()) {
        return 1;
    }

    controlador.ejecutar();

    return 0;
}