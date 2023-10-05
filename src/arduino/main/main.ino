//Primeiro deve-se enviar os dados para a rasp:
//   - imu mpu 6050
//   - encoders AS5600
//Depois vamos receber informações para o controle das rodas:
//   - drivers A4988

#include <Wire.h>
//#include <AS5600.h>
#include <MPU6050.h>

//===drivers===
const int stepLB = 13;
const int stepRB = 12;
const int stepLF = 11;
const int stepRF = 10;

const int dirLB = 33;
const int dirRB = 32;
const int dirLF = 31;
const int dirRF = 30;

#define ENABLE_PIN 4

//===encoders===
//AS5600 encoder1(0x36);  // Endereço I2C do primeiro encoder
//AS5600 encoder2(0x37);  // Endereço I2C do segundo encoder
//AS5600 encoder3(0x38);  // Endereço I2C do terceiro encoder
//AS5600 encoder4(0x39);  // Endereço I2C do quarto encoder

// #define encoderLB A0;
// #define encoderRB A1;
// #define encoderLF A2;
// #define encoderRF A3;

// Variáveis para contagem de pulsos
// const int encoder1PinA = 2;
// const int encoder1PinB = 3;
// const int encoder2PinA = 4;
// const int encoder2PinB = 5;
// const int encoder3PinA = 6;
// const int encoder3PinB = 7;
// const int encoder4PinA = 8;
// const int encoder4PinB = 9;

// volatile long encoder1Pos = 0;
// volatile long encoder2Pos = 0;
// volatile long encoder3Pos = 0;
// volatile long encoder4Pos = 0;

//===imu===
MPU6050 mpu;

void setup() {
  //===Início da comunicação serial===
  Wire.begin();
  Serial.begin(9600);
  while (!Serial) {}

  //===Verifica se a imu está conectada corretamente e a calibra===
  if (!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_16G)) {
    Serial.println("Não foi possível encontrar a MPU-6050. Por favor, verifique suas conexões.");
    while (1);
  }
  
  mpu.calibrateGyro();
  mpu.setThreshold(3);

  //===Verifica se todos os encoders estão conectados===
  //if (!encoder1.begin() || !encoder2.begin() || !encoder3.begin() || !encoder4.begin()) {
  //  Serial.println("Não foi possível encontrar um ou mais encoders AS5600. Por favor, verifique suas conexões.");
  //  while (1);
  //}
  // pinMode(encoderLB, INPUT);
  // pinMode(encoderRB, INPUT);
  // pinMode(encoderLF, INPUT);
  // pinMode(encoderRF, INPUT);

  // Configuração de interrupção para detectar pulsos do encoder
  // attachInterrupt(digitalPinToInterrupt(encoderLB), contabilizarPulso, RISING);

  //===Configura a informação dos drivers===
  // pinMode(ENABLE_PIN, OUTPUT);
  // digitalWrite(ENABLE_PIN, LOW);  // Ativar o driver

  // stepper.setMaxSpeed(1000);       // Velocidade máxima em passos por segundo
  // stepper.setAcceleration(500);    // Aceleração em passos por segundo ao quadrado

  pinMode(stepLB, OUTPUT);
  pinMode(stepRB, OUTPUT);
  pinMode(stepLF, OUTPUT);
  pinMode(stepRF, OUTPUT);

  pinMode(dirLB, OUTPUT);
  pinMode(dirRB, OUTPUT);
  pinMode(dirLF, OUTPUT);
  pinMode(dirRF, OUTPUT);
}

void loop() {
  //Envia os quaternions e a covariância da imu
  Vector normQuat = mpu.readNormalizeQuat();
  
  // Formata os dados do quaternion como uma string
  String quaternionData = String(normQuat.w, 4) + "," +
                          String(normQuat.x, 4) + "," +
                          String(normQuat.y, 4) + "," +
                          String(normQuat.z, 4);
  
  // Envia os dados do quaternion para o Raspberry Pi via comunicação serial
  Serial.println(quaternionData);

  //Serial.println(normQuat);

  int16_t accelX, accelY, accelZ;
  mpu.getAcceleration(&accelX, &accelY, &accelZ);
  
  // Enviar os dados do acelerômetro para a Raspberry Pi via comunicação serial
  Serial.print("Acelerômetro X: "); Serial.print(accelX);
  Serial.print(", Y: "); Serial.print(accelY);
  Serial.print(", Z: "); Serial.println(accelZ);

  //envia a velocidade angular medida nos encoders
  
  // Verificar se há dados disponíveis na porta serial
  //if (Serial.available() > 0) {
  //  int steps = Serial.parseInt();  // Ler o número de passos do Raspberry Pi
  //  stepper.moveTo(steps);          // Mover o motor para a posição especificada
  //}

  //Rotate motors forward
  digitalWrite(dirLB, HIGH); // Set direction
  digitalWrite(dirRB, HIGH);
  digitalWrite(dirLF, HIGH);
  digitalWrite(dirRF, HIGH);

  if(Serial.available() > 0){
    String dados = Serial.readStringUntil('\n');

    // Separe os valores usando a vírgula como delimitador
    float velLB = dados.substring(0, dados.indexOf(',')).toFloat();
    dados.remove(0, dados.indexOf(',') + 1);
    float velRB = dados.substring(0, dados.indexOf(',')).toFloat();
    dados.remove(0, dados.indexOf(',') + 1);
    float velLF = dados.substring(0, dados.indexOf(',')).toFloat();
    dados.remove(0, dados.indexOf(',') + 1);
    float velRF = dados.toFloat();

    // Use os valores float como desejado
    Serial.print("VelLB: "); Serial.println(velLB, 2);
    Serial.print("VelRB: "); Serial.println(velRB, 2);
    Serial.print("VelLF: "); Serial.println(velLF, 2);
    Serial.print("VelLF: "); Serial.println(velRF, 2);

    // Generate PWM signal for motor steps (adjust the delay for speed control)
    for (int i = 0; i < 1000; i++) {
      digitalWrite(stepLB, velLB);
      digitalWrite(stepRB, velLF);
      digitalWrite(stepLF, velRB);
      digitalWrite(stepRF, velRF);
      delayMicroseconds(500); // Adjust the delay for speed control
      digitalWrite(stepLB, LOW);
      digitalWrite(stepRB, LOW);
      digitalWrite(stepLF, LOW);
      digitalWrite(stepRF, LOW);
      delayMicroseconds(500); // Adjust the delay for speed control
    }  
        //Use isso apenas ver se o código funciona e depois exclua
        //Serial.println(message);
  }

  
  
  delay(1000);  // Aguarda 1 segundo antes de ler os dados novamente
}