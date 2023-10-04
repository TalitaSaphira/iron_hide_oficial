#include <AccelStepper.h>
#include <Servo.h>
#include <math.h>
#include "rodas.hpp"
#include "comunicacao.hpp"
#include "posicao.hpp"

// Define os motores stepper
AccelStepper LeftBackWheel(1, RODA1_STEP, RODA1_DIR);   // (Type:driver, STEP, DIR) - Stepper1
AccelStepper LeftFrontWheel(1, RODA2_STEP, RODA2_DIR);  // Stepper2
AccelStepper RightBackWheel(1, RODA3_STEP, RODA3_DIR);  // Stepper3
AccelStepper RightFrontWheel(1, RODA4_STEP, RODA4_DIR); // Stepper4
AccelStepper Manipulador(1, MANIPUL_STEP, MANIPUL_DIR); // StepperManip

// Define o servo da garra
Servo efet;

// Velocidade do arduino
int velocidade = 200;   // Velocidade do robô

// Posições do robô e a sua posição objetivo
float posx = 0;       // Em cm
float posy = 0;
float posz = 0;
float posrot = 0;     // Em radiano

// Que tipo de movimento o robô está fazendo
int comandoRoda = 10;

float lxly = 20.5; // Soma das distâncias (lx e ly) do centro do robô aos cantos (ou seja, lx + ly) -> aproximado
int r = 7; // Raio da roda (Distância do centro da roda até o centro do roller) em cm
int spr = 200;  // Passos por revolução do NEMA 17

int keepspin = 0;

void setup() {
  LeftFrontWheel.setMaxSpeed(1000);
  LeftBackWheel.setMaxSpeed(1000);
  RightFrontWheel.setMaxSpeed(1000);
  RightBackWheel.setMaxSpeed(1000);
  Manipulador.setMaxSpeed(1000);
  LeftFrontWheel.setSpeed(0);
  LeftBackWheel.setSpeed(0);
  RightFrontWheel.setSpeed(0);
  RightBackWheel.setSpeed(0);
  Manipulador.setSpeed(0);
  Manipulador.setAcceleration(150);

  efet.attach(EFETUADOR_S);
  
  pinMode(INFRA1_S, INPUT);
  pinMode(INFRA2_S, INPUT);
  pinMode(INFRA3_S, INPUT);
  pinMode(INFRA4_S, INPUT);
  pinMode(FIMDECURSO_S, INPUT);
  pinMode(RODA1_STEP, INPUT);
  pinMode(RODA1_DIR, INPUT);
  pinMode(RODA2_STEP, INPUT);
  pinMode(RODA2_DIR, INPUT);
  pinMode(RODA3_STEP, INPUT);
  pinMode(RODA3_DIR, INPUT);
  pinMode(RODA4_STEP, INPUT);
  pinMode(RODA4_DIR, INPUT);
  pinMode(MANIPUL_STEP, INPUT);
  pinMode(MANIPUL_DIR, INPUT);

  Serial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);

  //--------------Onde podemos fazer os testes-----------------
  //moveForward();
  //LeftFrontWheel.setSpeed(200);
  
  /*delay(3000);
  moveForward();
  delay(2000);
  rotateRight(180);
  moveForward();
  delay(2000);
  stopMoving();
  delay(100);
  moveBackward();
  delay(100);
  moveForward();
  delay(100);
  stopMoving();*/
//delay(10000);
  //rotateRight(180);
}

void loop() {
  // Checa se o Raspberry enviou uma mensagem e faz tudo
  doComm();
  
  // Executar os passos
  LeftFrontWheel.runSpeed();
  LeftBackWheel.runSpeed();
  RightFrontWheel.runSpeed();
  RightBackWheel.runSpeed();
  //Manipulador.run();

  // Faz a totalização do movimento
  totalizacao();
}
