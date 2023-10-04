/*
   === Arduino Mecanum Wheels Robot ===
     Radio control with NRF24L01 
  by Dejan, www.HowToMechatronics.com
  Libraries:
  RF24, https://github.com/tmrh20/RF24/
  AccelStepper by Mike McCauley: http://www.airspayce.com/mikem/arduino/AccelStepper/index.html

*/

/*Adaptei do código original daqui de cima ^^^^*/

/* O que esse código irá fazer: Receber a velocidade total ou ângular do raspberry (ímplicito, fora do código), receber o comando de qual tipo de movimento ele precisa fazer,
calcular a velocidade das rodas necessárias para alcançar essa velocidade e rodas as rodas*/

#include <AccelStepper.h>
#include "rodas.hpp"

extern void totalizacao();

extern AccelStepper LeftBackWheel;
extern AccelStepper LeftFrontWheel;
extern AccelStepper RightBackWheel;
extern AccelStepper RightFrontWheel;
extern int velocidade;
extern int comandoRoda;

extern float posrot;

extern float lxly;
extern int r;
extern int spr;

int wheelSpeed = velocidade; // A velocidade da roda vai ser a velocidade definida em embarcados.ino

float lastrot;

float angulo;

// Atua nas rodas. Como fazer a movimentação
void actMotor() {
  // Manda um ack
  Serial.write(1);
  comandoRoda = Serial.read();
  Serial.write(1);
  angulo = Serial.read();
  angulo = angulo*M_PI/180;

  switch (comandoRoda) {
    case FORWARD:
      moveForward();
    break;
    case BACKWARD:
      moveBackward();
    break;
    case SIDEWAYSRIGHT:
      moveSidewaysRight();
    break;
    case SIDEWAYSLEFT:
      moveSidewaysLeft();
    break;
    case ROTATELEFT:
      rotateLeft();
    break;
    case ROTATERIGHT:
      rotateRight(0);
    break;
    case RIGHTFORWARD:
      moveRightForward();
    break;
    case RIGHTBACKWARD:
      moveRightBackward();
    break;
    case LEFTFORWARD:
      moveLeftForward();
    break;
    case LEFTBACKWARD:
      moveLeftBackward();
    break;
    case STOPMOVE:
      stopMoving();
    break;
    default:
      stopMoving();
    break;
  }
}

// Agora todos os movimentos possíveis
void moveForward() {
  //float wheelSpeed = velocidade/r;        // Cálculo da velocidade que a roda precisa ter para o robô andar na velocidade desejada
  //wheelSpeed = wheelSpeed*spr/(2*M_PI);   // rad/s para passos por segundo
  
  LeftFrontWheel.setSpeed(wheelSpeed);
  LeftBackWheel.setSpeed(wheelSpeed);
  RightFrontWheel.setSpeed(wheelSpeed);
  RightBackWheel.setSpeed(wheelSpeed);
}
void moveBackward() {
  //float wheelSpeed = velocidade/r;
  //wheelSpeed = wheelSpeed*spr/(2*M_PI);
  
  LeftFrontWheel.setSpeed(-wheelSpeed);
  LeftBackWheel.setSpeed(-wheelSpeed);
  RightFrontWheel.setSpeed(-wheelSpeed);
  RightBackWheel.setSpeed(-wheelSpeed);
}
void moveSidewaysRight() {
  //float wheelSpeed = velocidade/r;
  //wheelSpeed = wheelSpeed*spr/(2*M_PI);
  
  LeftFrontWheel.setSpeed(wheelSpeed);
  LeftBackWheel.setSpeed(-wheelSpeed);
  RightFrontWheel.setSpeed(-wheelSpeed);
  RightBackWheel.setSpeed(wheelSpeed);
}
void moveSidewaysLeft() {
  //float wheelSpeed = velocidade/r;
  //wheelSpeed = wheelSpeed*spr/(2*M_PI);
  
  LeftFrontWheel.setSpeed(-wheelSpeed);
  LeftBackWheel.setSpeed(wheelSpeed);
  RightFrontWheel.setSpeed(wheelSpeed);
  RightBackWheel.setSpeed(-wheelSpeed);
}
void rotateLeft() {
  //float wheelSpeed = velocidade*lxly/r;
  //wheelSpeed = wheelSpeed*spr/60;         // RPM para passos por segundo
  
  LeftFrontWheel.setSpeed(-wheelSpeed);
  LeftBackWheel.setSpeed(-wheelSpeed);
  RightFrontWheel.setSpeed(wheelSpeed);
  RightBackWheel.setSpeed(wheelSpeed);

  rotateUntil(angulo);
}
void rotateRight(int angulo2) {
  //float wheelSpeed = velocidade*lxly/r;
  //wheelSpeed = wheelSpeed*spr/60;

  if (angulo2 != 0) {
    angulo = angulo2*180/M_PI;
  }
  
  LeftFrontWheel.setSpeed(wheelSpeed);
  LeftBackWheel.setSpeed(wheelSpeed);
  RightFrontWheel.setSpeed(-wheelSpeed);
  RightBackWheel.setSpeed(-wheelSpeed);

  rotateUntil(angulo);
}
void moveRightForward() {
  //float velocidadeDecompos = velocidade/sqrt(2);
  //float wheelSpeed = velocidadeDecompos*2/r;
  //wheelSpeed = wheelSpeed*spr/(2*M_PI);
  
  LeftFrontWheel.setSpeed(wheelSpeed);
  LeftBackWheel.setSpeed(0);
  RightFrontWheel.setSpeed(0);
  RightBackWheel.setSpeed(wheelSpeed);
}
void moveRightBackward() {
  //float velocidadeDecompos = velocidade/sqrt(2);
  //float wheelSpeed = velocidadeDecompos*2/r;
  //wheelSpeed = wheelSpeed*spr/(2*M_PI);
  
  LeftFrontWheel.setSpeed(0);
  LeftBackWheel.setSpeed(-wheelSpeed);
  RightFrontWheel.setSpeed(-wheelSpeed);
  RightBackWheel.setSpeed(0);
}
void moveLeftForward() {
  //float velocidadeDecompos = velocidade/sqrt(2);
  //float wheelSpeed = velocidadeDecompos*2/r;
  //wheelSpeed = wheelSpeed*spr/(2*M_PI);
  
  LeftFrontWheel.setSpeed(0);
  LeftBackWheel.setSpeed(wheelSpeed);
  RightFrontWheel.setSpeed(wheelSpeed);
  RightBackWheel.setSpeed(0);
}
void moveLeftBackward() {
  //float velocidadeDecompos = velocidade/sqrt(2);
  //float wheelSpeed = velocidadeDecompos*2/r;
  //wheelSpeed = wheelSpeed*spr/(2*M_PI);
  
  LeftFrontWheel.setSpeed(-wheelSpeed);
  LeftBackWheel.setSpeed(0);
  RightFrontWheel.setSpeed(0);
  RightBackWheel.setSpeed(-wheelSpeed);
}
void stopMoving() {
  LeftFrontWheel.setSpeed(0);
  LeftBackWheel.setSpeed(0);
  RightFrontWheel.setSpeed(0);
  RightBackWheel.setSpeed(0);
}

// Dá delay porém deixa as rodas rodando e a totalização contando
void delayWithWheels(int tempo) {
  int timeRef = millis();
  int timeNow = timeRef;
  while (timeRef - timeNow < tempo) {
    LeftFrontWheel.runSpeed();
    LeftBackWheel.runSpeed();
    RightFrontWheel.runSpeed();
    RightBackWheel.runSpeed();
    totalizacao();
    timeNow = millis();
  }
}

// Rode até um certo ângulo
void rotateUntil(float angulo) {
  lastrot = posrot;
  while (angulo < abs(posrot - lastrot)) {
    LeftFrontWheel.runSpeed();
    LeftBackWheel.runSpeed();
    RightFrontWheel.runSpeed();
    RightBackWheel.runSpeed();
    totalizacao();
  }
}
