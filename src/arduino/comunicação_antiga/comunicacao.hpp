#ifndef COMUNICACAO_H
#define COMUNICACAO_H

#include "Arduino.h"

// Definição dos comandos
#define INFRA1 1      // Requisição de dados do sensor de obstáculo infravermelho 1
#define INFRA2 2      // Requisição de dados do sensor de obstáculo infravermelho 2
#define INFRA3 3      // Requisição de dados do sensor de obstáculo infravermelho 3
#define INFRA4 4      // Requisição de dados do sensor de obstáculo infravermelho 4
#define ENCODER1 5    // Requisição de dados do encoder 1
#define ENCODER2 6    // Requisição de dados do encoder 2
#define ENCODER3 7    // Requisição de dados do encoder 3
#define ENCODER4 8    // Requisição de dados do encoder 4
#define ENCODERM 9    // Requisição de dados do encoder do manipulador
#define FIMDECURSO 10 // Requisição de dados da chave de fim de curso no manipulador
#define RODAS 11      // Envio de dados às rodas
#define MANIPUL 12    // Envio de dados ao motor do manipulador
#define EFETUADOR 13  // Envio de dados ao efetuador da garra
#define POSICAO 14    // Envio de dados da posição

// Definição dos pinos
#define INFRA1_S 13
#define INFRA2_S 12
#define INFRA3_S 11
#define INFRA4_S 10
#define ENCODER1_S A11
#define ENCODER2_S A11 // Se consertarem o encoder da Roda 2, (LeftFrontWheel), muda isso para A12 aqui e em posicao.cpp
#define ENCODER3_S A13
#define ENCODER4_S A14
#define ENCODERM_S A10
#define FIMDECURSO_S 9
#define RODA1_STEP 40
#define RODA1_DIR 39
#define RODA2_STEP 52
#define RODA2_DIR 51
#define RODA3_STEP 52
#define RODA3_DIR 51
#define RODA4_STEP 44
#define RODA4_DIR 43
#define MANIPUL_STEP 24
#define MANIPUL_DIR 23
#define EFETUADOR_S 7

// Função responsável por fazer a comunicação com o Raspberry
void doComm();

// Lê os dados do sensor infravermelho ou fim de curso e envia ao Raspberry
void readInfraredOrFDC(const int pinNum);

// Lê os dados do sensor encoder e envia ao Raspberry
void readEncoder(const int pinNum);

// Envia os dados ao motor servo da garra
void actEfetuador();

// Envia a posição desejada do motor stepper do manipulador
void actManip();

// Ennvia a posição do robô
void sendPos();

#endif
