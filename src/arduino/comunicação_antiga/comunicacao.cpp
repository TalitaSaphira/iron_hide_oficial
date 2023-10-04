  /*
 * Este código é responsável pela comunicação entre Raspberry Pi e Arduino por protocolo USB
 * O protocolo de comunicação em si é algo próprio, USB é só usado para transporte de informação
 */

#include <AccelStepper.h>
#include <Servo.h>
#include "comunicacao.hpp"

// Avisa ao compilador que essas funções e variáveis foram definidas por outro código
extern void actMotor();
extern AccelStepper Manipulador;
extern Servo efet;

extern float posx;
extern float posy;
extern float posz;
extern float posrot;
extern float comandoRoda;

// Função principal da comunicação. Ideal que seja chamado em um loop
void doComm() {
  /*if (Serial.available() > 0) {
    digitalWrite(LED_BUILTIN, HIGH);
    uint8_t data = Serial.read();
    Serial.print("You sent me: ");
    Serial.write(data);
  }*/
  
  // Checa se recebeu algum comando da Raspberry
  if (Serial.available()) {
    uint8_t comando = Serial.read();

    // Decodifica o comando e atua de acordo
    switch (comando) {
      case INFRA1:
        readInfraredOrFDC(INFRA1_S);
        break;
      case INFRA2:
        readInfraredOrFDC(INFRA2_S);
        break;
      case INFRA3:
        readInfraredOrFDC(INFRA3_S);
        break;
      case INFRA4:
        readInfraredOrFDC(INFRA4_S);
        break;
      case ENCODER1:
        readEncoder(ENCODER1_S);
        break;
      case ENCODER2:
        readEncoder(ENCODER2_S);
        break;
      case ENCODER3:
        readEncoder(ENCODER3_S);
        break;
      case ENCODER4:
        readEncoder(ENCODER4_S);
        break;
      case ENCODERM:
        readEncoder(ENCODERM_S);
        break;
      case FIMDECURSO:
        readInfraredOrFDC(FIMDECURSO_S);
        break;
      case RODAS:
        actMotor();
        break;
      case MANIPUL:
        actManip();
        break;
      case EFETUADOR:
        actEfetuador();
        break;
      case POSICAO:
        sendPos();
        break;
    }
  }
}

// Recebe os dados do sensor e envia ao Rasperry... NÃO UTILIZADO
/*void readInfraredOrFDC(const int pinNum) {
  int valor = digitalRead(pinNum);
  Serial.write(valor);
}

// Recebe os dados do encoder e envia ao Rasperry... NÃO UTILIZADO
void readEncoder(const int pinNum) {
  // A leitura pode ser feita com https://www.arduino.cc/reference/en/libraries/as5600/
  int valor; //DEPENDE DA BIBLIOTECA
  Serial.write(valor);
}*/

// Atua no efetuador em si
void actEfetuador() {
  // Envia confirmação pro raspberry
  Serial.write(1);
  
  // Abertura: 0 / Fechamento: 35
  // Lê a posição e atua
  uint8_t pos = Serial.read();
  efet.write(pos);
}

// Atua no motor do eixo vertical
void actManip() {
  // Envia confirmação pro raspberry
  Serial.write(1);

  // Lê a posição e atua
  uint8_t pos = Serial.read();
  int passos = 9450*(pos/255)/1.8;         // Converte valor 0-255 em graus (0º-9450º)passos
  Manipulador.moveTo(passos);
}

// Envia dados da odometria para a Raspberry (posx, posy, posz e posrot)
void sendPos() {
  uint8_t tosend;
  // Manda reconhecimento para o raspberry
  Serial.write(1);
  // Mandar a posição x. Primeiro manda a parte inteira
  tosend = posx;
  Serial.write(tosend);
  // Agora manda a parte decimal
  tosend = (posx-tosend)*100;
  Serial.write(tosend);
  
  // Mandar a posição y
  tosend = posy;
  Serial.write(tosend);
  tosend = (posy-tosend)*100;
  Serial.write(tosend);
  
  // Mandar a posição z
  tosend = posz;
  Serial.write(tosend);
  tosend = (posz-tosend)*100;
  Serial.write(tosend);
  
  // Mandar a posição de rotação em radianos
  tosend = posrot;
  Serial.write(tosend);
  tosend = (posrot-tosend)*100;
  Serial.write(tosend);
}
