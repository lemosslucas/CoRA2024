#include "motores.h"

// define os sensores
// SENSOR_0 == ESQUERDA :: SENSOR_4 == DIREITA
const int SENSOR_0 = 3;
const int SENSOR_1 = 4;
const int SENSOR_2 = 5;
const int SENSOR_3 = 6;
const int SENSOR_4 = 7;

// variaveis para ler a saida do sensor
int SENSOR[4];

#define BRANCO 0 
#define PRETO 1

int velocidadeBaseDireita = 255;
int velocidadeBaseEsquerda = 255;

// variaveis para o calculo do PID
int erro = 0; int PID = 0; int erroAnterior = 0;
int I = 0; int P = erro; int D = 0; int Ki = 0; int Kd = 0; int Kp = 50; 

void setup() {
  pinMode(SENSOR_0, INPUT);
  pinMode(SENSOR_1, INPUT);
  pinMode(SENSOR_2, INPUT);
  pinMode(SENSOR_3, INPUT);
  pinMode(SENSOR_4, INPUT);

  Serial.begin(9600);
}

void loop() {
  SENSOR[0] = digitalRead(SENSOR_0);
  SENSOR[1] = digitalRead(SENSOR_1);
  SENSOR[2] = digitalRead(SENSOR_2);
  SENSOR[3] = digitalRead(SENSOR_3);
  SENSOR[4] = digitalRead(SENSOR_4);

  if (SENSOR[0] == BRANCO && SENSOR[1] == PRETO && SENSOR[2] == BRANCO && SENSOR[3] == PRETO && SENSOR[4] == PRETO) {
    curva_esquerda();
  } else if (SENSOR[0] == PRETO && SENSOR[1] == PRETO && SENSOR[2] == BRANCO && SENSOR[3] == PRETO &&SENSOR[4] == BRANCO) {
    curva_direita();
  }
  
  calcula_erro();
  calculaPID();
  if (erro == -10) {
    parar();
  } else {
    ajusta_movimento();
  }

  delay(50);
}

void ajusta_movimento() {
  int velocidadeDireita = constrain(velocidadeBase + PID, 1, 255);
  int velocidadeEsquerda = constrain(velocidadeBase - PID, 1, 255);

  andar(velocidadeDireita, velocidadeEsquerda);
}

void calcula_erro() {
  if (SENSOR[1] == PRETO && SENSOR[2] == BRANCO && SENSOR[3] == PRETO) {
    erro = 0; // Anda reto
  } else if (SENSOR[1] == PRETO && SENSOR[2] == PRETO && SENSOR[3] == BRANCO) {
    erro = -1; // Curva leve para a esquerda
  } else if (SENSOR[1] == BRANCO && SENSOR[2] == PRETO && SENSOR[3] == PRETO) {
    erro = 1; // Curva leve para a direita
  } else if (SENSOR[1] == PRETO && SENSOR[2] == BRANCO && SENSOR[3] == BRANCO) {
    erro = -2; // Curva acentuada para a esquerda
  } else if (SENSOR[1] == BRANCO && SENSOR[2] == BRANCO && SENSOR[3] == PRETO) {
    erro = 2; // Curva acentuada para a direita
  } else if (SENSOR[1] == PRETO && SENSOR[2] == PRETO && SENSOR[3] == PRETO) {
    erro = -10;
  }
}

// calcula a integral para a corrente
void calcula_PID() {
  P = erro;
  if (erro == 0) {
    I = 0;
  }
  I = I + P;
  if (I > 255) {
    I = 255;
  } else if (I < 255){
    I = -255;
  }
  D = erro - erroAnterior;
  PID = (Kp* P) + (Ki * I) + (Kd*D);
  erroAnterior = erro;
}
