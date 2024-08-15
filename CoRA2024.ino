#include "motores.h"

// define os sensores
// SENSOR_0 == ESQUERDA :: SENSOR_4 == DIREITA
const int SENSOR_0 = 14;
const int SENSOR_1 = 15;
const int SENSOR_2 = 16;
const int SENSOR_3 = 17;
const int SENSOR_4 = 18;

// variaveis para ler a saida do sensor
int SENSOR[5];

#define BRANCO 0 
#define PRETO 1
#define TEMPO_CURVA 200

int velocidadeBaseDireita = 255;
int velocidadeBaseEsquerda = 255;

// variaveis para o calculo do PID
int erro = 0; int PID = 0; int erroAnterior = 0;
int I = 0; int P = erro; int D = 0; 
int Ki = 1; int Kd = 0.1; int Kp = 0.01; 

void setup() {
  pinMode(SENSOR_0, INPUT);
  pinMode(SENSOR_1, INPUT);
  pinMode(SENSOR_2, INPUT);
  pinMode(SENSOR_3, INPUT);
  pinMode(SENSOR_4, INPUT);

  Serial.begin(9600);
  delay(1000);
}

void loop() {
  SENSOR[0] = digitalRead(SENSOR_0);
  SENSOR[1] = digitalRead(SENSOR_1);
  SENSOR[2] = digitalRead(SENSOR_2);
  SENSOR[3] = digitalRead(SENSOR_3);
  SENSOR[4] = digitalRead(SENSOR_4);
  /*
  if (SENSOR[0] == BRANCO && SENSOR[1] == PRETO && SENSOR[2] == BRANCO && SENSOR[3] == PRETO && SENSOR[4] == PRETO) {
    curva_esquerda(255, 255);
    delay(TEMPO_CURVA);
  } else if (SENSOR[0] == PRETO && SENSOR[1] == PRETO && SENSOR[2] == BRANCO && SENSOR[3] == PRETO && SENSOR[4] == BRANCO) {
    curva_direita(255, 255);
    delay(TEMPO_CURVA);
  } else if (SENSOR[0] == BRANCO && SENSOR[1] == PRETO && SENSOR[2] == BRANCO && SENSOR[3] == PRETO && SENSOR[4] == BRANCO) {
    // vai decidir no dia qual trajeto é menor
    //curva_direita(255, 255);
    //curva_esquerda(255, 255);
    delay(TEMPO_CURVA);
  }
  */

  calcula_erro();
  calcula_PID();

  if (erro == -10) {
    parar();
  } else {
    ajusta_movimento();
  }

  delay(10);
}

void ajusta_movimento() {
  int velocidadeDireita = constrain(velocidadeBaseDireita + PID, 1, 190);
  int velocidadeEsquerda = constrain(velocidadeBaseEsquerda + PID, 1, 190);

  andar(velocidadeDireita, velocidadeEsquerda);
}

void calcula_erro() {
  if (SENSOR[1] == PRETO && SENSOR[2] == BRANCO && SENSOR[3] == PRETO) {
    erro = 0; // Anda reto
  } else if (SENSOR[1] == PRETO && SENSOR[2] == BRANCO && SENSOR[3] == BRANCO) {
    erro = -1; // carro pouco a esquerda
  } else if (SENSOR[1] == BRANCO && SENSOR[2] == BRANCO && SENSOR[3] == PRETO) {
    erro = 1; // carro muito a direita
  } else if (SENSOR[1] == PRETO && SENSOR[2] == PRETO && SENSOR[3] == BRANCO) {
    erro = -2; // carro muito a esquerda
  } else if (SENSOR[1] == BRANCO && SENSOR[2] == PRETO && SENSOR[3] == PRETO) {
    erro = 2; // carro muito a direita
  } else if (SENSOR[1] == PRETO && SENSOR[2] == PRETO && SENSOR[3] == PRETO) {
    erro = -10;
  }
}

void calcula_PID() {
  P = erro;
  I = constrain(I+P, 1, 255);
  D = erro - erroAnterior;
  PID = (Kp * P) + (Ki * I) + (Kd*D);
  erroAnterior = erro;
}
