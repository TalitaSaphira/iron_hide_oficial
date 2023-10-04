#ifndef POSICAO_H
#define POSICAO_H

#include "Arduino.h"

// O código da totalização do movimento. O ideal é que seja chamado em um loop
void totalizacao();

// Checa os quadrantes das rodas
void checkQuadrant();

// Checa os quadrantes do eixo
void checkQuadrantEixo();

// Reseta as posições, menos a posição do eixo
void resetpos();

#endif
