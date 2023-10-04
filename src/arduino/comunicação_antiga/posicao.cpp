/*
 * Esse código irá calcular a odometria. Ele irá ler pelos encoders o quanto que as rodas moveram.
 * É importante que não haja deslizamento nas rodas, se não vai cagar com tudo
 */

#include "posicao.hpp"

// Definição dos comandos de rodas
#define FORWARD 0
#define BACKWARD 1
#define SIDEWAYSRIGHT 2
#define SIDEWAYSLEFT 3
#define ROTATELEFT 4
#define ROTATERIGHT 5
#define RIGHTFORWARD 6
#define RIGHTBACKWARD 7
#define LEFTFORWARD 8
#define LEFTBACKWARD 9
#define STOPMOVE 10

// Definição dos pinos dos encoders
#define ENCODER1_S A11
#define ENCODER2_S A11 // Se consertarem o encoder da Roda 2, (LeftFrontWheel), muda isso para A12 aqui e em comunicacao.hpp
#define ENCODER3_S A13
#define ENCODER4_S A14
#define ENCODERM_S A10

extern float posx;
extern float posy;
extern float posz;
extern float posrot;

extern int comandoRoda;

extern int velocidade;

extern float lxly;
extern int r;
extern int spr;

// Definem o quadrante atual e o útlimo quadrante em que estava o encoder
/*
  //Quadrantes:
  4  |  1
  ---|---
  3  |  2
*/
int nowQuadrant1 = 1;
int lastQuadrant1 = 1;
int nowQuadrant2 = 1;
int lastQuadrant2 = 1;
int nowQuadrant3 = 1;
int lastQuadrant3 = 1;
int nowQuadrant4 = 1;
int lastQuadrant4 = 1;
int nowQuadrantEixo = 1;
int lastQuadrantEixo = 1;

// Definem o último tempo medido e o tempo medido agora
unsigned long lastTime = millis();
unsigned long nowTime;

// Definem o ângulo medido agora e o último ângulo medido
float lastang1 = analogRead(ENCODER1_S)*360/1024;
float nowang1;
float lastang2 = analogRead(ENCODER2_S)*360/1024;
float nowang2;
float lastang3 = analogRead(ENCODER3_S)*360/1024;
float nowang3;
float lastang4 = analogRead(ENCODER4_S)*360/1024;
float nowang4;
float lastangeixo = analogRead(ENCODERM_S)*360/1024;
float nowangeixo;

// Define 90º em radianos
const float rad90 = 90*M_PI/180;

// O código da totalização do movimento. O ideal é que seja chamado em um loop
void totalizacao() {
  // Veja as posições atuais do encoder e transforme em graus
  nowang1 = analogRead(ENCODER1_S)*360/1024;
  nowang2 = analogRead(ENCODER2_S)*360/1024;
  nowang3 = analogRead(ENCODER3_S)*360/1024;
  nowang4 = analogRead(ENCODER4_S)*360/1024;

  // Cheque o quadrante das rodas
  checkQuadrant();

  // Se ele mudou de quadrante entre 1 <-> 4, ele fez um giro, tem que levar isso em compensação
  int compensador = 0;
  if (nowQuadrant1 == 1 && lastQuadrant1 == 4) {
    compensador = 360;
  }
  if (nowQuadrant1 == 4 && lastQuadrant1 == 1){
    compensador = -360;
  }
  // A diferença de ângulo é o anterior menor o atual mais o compensador de giro. Em valores absolutos
  float difang1 = abs(nowang1 - lastang1 + compensador);

  // Agora faz isso para os outros 3 encoders
  compensador = 0;
  if (nowQuadrant2 == 1 && lastQuadrant2 == 4) {
    compensador = 360;
  }
  if (nowQuadrant2 == 4 && lastQuadrant2 == 1){
    compensador = -360;
  }
  
  float difang2 = abs(nowang2 - lastang2 + compensador);
  
  compensador = 0;
  if (nowQuadrant3 == 1 && lastQuadrant3 == 4) {
    compensador = 360;
  }
  if (nowQuadrant3 == 4 && lastQuadrant3 == 1){
    compensador = -360;
  }
  
  float difang3 = abs(nowang3 - lastang3 + compensador);
  
  compensador = 0;
  if (nowQuadrant4 == 1 && lastQuadrant4 == 4) {
    compensador = 360;
  }
  if (nowQuadrant4 == 4 && lastQuadrant4 == 1){
    compensador = -360;
  }
  
  float difang4 = abs(nowang4 - lastang4 + compensador);

  // A diferença de ângulo total é uma média simples entre as diferenças dos ângulos
  float difang = (difang1+difang2+difang3+difang4)/4;
  
  // Quantidade movida é difang vezes raio das rodas vezes pi/180
  float amountMoved = difang*r*M_PI/180;
  compensador = 0;

  // Agora vamos calcular as velocidades
  // Veja a diferença de tempo
  nowTime = millis();
  unsigned long difTime = nowTime - lastTime;

  // A velocidade angular da roda é isso
  float velocRoda = difang/difTime * M_PI/180;
  // A velocidade angular do robô inteiro é isso
  float velocAngular = velocRoda*r/lxly;
  
  // Agora, sabendo o quanto ele rodou, calculamos o quanto que ele se moveu
  // Eu tenho um diagramazinho na sede
  // Note que só usamos a velocidade ângular do robô quando ele roda
  switch (comandoRoda) {
    case FORWARD:
      posx = posx + cos(posrot) * amountMoved;
      posx = posx + sin(posrot) * amountMoved;
    break;
    case BACKWARD:
      posx = posx + cos(rad90*2+posrot) * amountMoved;
      posx = posx + sin(rad90*2+posrot) * amountMoved;
    break;
    case SIDEWAYSRIGHT:
      posx = posx + cos(rad90+posrot) * amountMoved;
      posx = posx + sin(rad90+posrot) * amountMoved;
    break;
    case SIDEWAYSLEFT:
      posx = posx + cos(rad90-posrot) * amountMoved;
      posx = posx + sin(rad90-posrot) * amountMoved;
    break;
    case ROTATELEFT:
      posrot = posrot - velocAngular*difTime;
    break;
    case ROTATERIGHT:
      posrot = posrot + velocAngular*difTime;
    break;
  }

  // Faz tudo aquilo que fizemos, porém com o motor do eixo vertical
  nowangeixo = analogRead(ENCODERM_S)*360/1024;
  checkQuadrantEixo();
  
  if (nowQuadrantEixo == 1 && lastQuadrantEixo == 4) {
    compensador = 360;
  }
  if (nowQuadrantEixo == 4 && lastQuadrantEixo == 1){
    compensador = -360;
  }
  
  difang = nowangeixo - lastangeixo + compensador;
  amountMoved = difang*r*M_PI/180;
  compensador = 0;
  posz = posz + amountMoved;

  // Agora, no final do código, os últimos valores são os atuais
  lastTime = nowTime;
  lastang1 = nowang1;
  lastang2 = nowang2;
  lastang3 = nowang3;
  lastang4 = nowang4;
  lastangeixo = nowangeixo;
}

// Checa os quadrantes das rodas
/*
  //Quadrantes:
  4  |  1
  ---|---
  3  |  2
*/
void checkQuadrant() {
  if (nowang1 >= 0 && nowang1 < 90) {
    nowQuadrant1 = 1;
  }
  if (nowang1 >= 90 && nowang1 < 180) {
    nowQuadrant1 = 2;
  }
  if (nowang1 >= 180 && nowang1 < 270) {
    nowQuadrant1 = 3;
  }
  if (nowang1 >= 270 && nowang1 < 360) {
    nowQuadrant1 = 4;
  }
  lastQuadrant1 = nowQuadrant1;
  
  if (nowang2 >= 0 && nowang2 < 90) {
    nowQuadrant2 = 1;
  }
  if (nowang2 >= 90 && nowang2 < 180) {
    nowQuadrant2 = 2;
  }
  if (nowang2 >= 180 && nowang2 < 270) {
    nowQuadrant2 = 3;
  }
  if (nowang2 >= 270 && nowang2 < 360) {
    nowQuadrant2 = 4;
  }
  lastQuadrant2 = nowQuadrant2;
  
  if (nowang3 >= 0 && nowang3 < 90) {
    nowQuadrant3 = 1;
  }
  if (nowang3 >= 90 && nowang3 < 180) {
    nowQuadrant3 = 2;
  }
  if (nowang3 >= 180 && nowang3 < 270) {
    nowQuadrant3 = 3;
  }
  if (nowang3 >= 270 && nowang3 < 360) {
    nowQuadrant3 = 4;
  }
  lastQuadrant3 = nowQuadrant3;
  
  if (nowang4 >= 0 && nowang4 < 90) {
    nowQuadrant4 = 1;
  }
  if (nowang4 >= 90 && nowang4 < 180) {
    nowQuadrant4 = 2;
  }
  if (nowang4 >= 180 && nowang4 < 270) {
    nowQuadrant4 = 3;
  }
  if (nowang4 >= 270 && nowang4 < 360) {
    nowQuadrant4 = 4;
  }
  lastQuadrant4 = nowQuadrant4;
}

// Checa os quadrantes do eixo
void checkQuadrantEixo() {
  if (nowangeixo >= 0 && nowangeixo < 90) {
    nowQuadrantEixo = 1;
  }
  if (nowangeixo >= 90 && nowangeixo < 180) {
    nowQuadrantEixo = 2;
  }
  if (nowangeixo >= 180 && nowangeixo < 270) {
    nowQuadrantEixo = 3;
  }
  if (nowangeixo >= 270 && nowangeixo < 360) {
    nowQuadrantEixo = 4;
  }
  lastQuadrantEixo = nowQuadrantEixo;
}

// Reseta as posições, menos a posição do eixo
void resetpos() {
  posx = 0;
  posy = 0;
  posrot = 0;
}
