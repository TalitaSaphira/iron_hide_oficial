#ifndef RODAS_H
#define RODAS_H

#include "Arduino.h"

// Definição dos comandos
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

// Principal
void actMotor();

// Funções de movimento dos motores
void moveForward();
void moveBackward();
void moveSidewaysRight();
void moveSidewaysLeft();
void rotateLeft();
void rotateRight(int angulo2);
void moveRightForward();
void moveRightBackward();
void moveLeftForward();
void moveLeftBackward();
void stopMoving();
void delayWithWheels(int tempo);
void rotateUntil(float angulo);

#endif
