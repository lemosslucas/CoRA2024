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
  if (SENSOR[0] == BRANCO && SENSOR[1] == PRETO && SENSOR[2] == BRANCO && SENSOR[3] == PRETO && SENSOR[4] == PRETO) {
    curva_esquerda(255, 255);
    delay(TEMPO_CURVA);
  } else if (SENSOR[0] == PRETO && SENSOR[1] == PRETO && SENSOR[2] == BRANCO && SENSOR[3] == PRETO && SENSOR[4] == BRANCO) {
    curva_direita(255, 255);
    delay(TEMPO_CURVA);
  } else if (SENSOR[0] == BRANCO && SENSOR[1] == PRETO && SENSOR[2] == BRANCO && SENSOR[3] == PRETO &&SENSOR[4] == BRANCO) {
    // vai decidir no dia qual trajeto é menor
    //curva_direita(255, 255);
    //curva_esquerda(255, 255);
    delay(TEMPO_CURVA);
  }
  
  calcula_erro();
  calcula_PID();
  if (erro == -10) {
    parar();
  } else {
    ajusta_movimento();
  }

  delay(50);
}

void ajusta_movimento() {
  int velocidadeDireita = constrain(velocidadeBaseDireita + PID, 1, 255);
  int velocidadeEsquerda = constrain(velocidadeBaseEsquerda - PID, 1, 255);

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

void calcula_PID() {
  P = erro;
  if (erro == 0) {
    I = 0;
  }
  I = I + P;
  if (I > 255) {
    I = 255;
  } else if (I < -255){
    I = -255;
  }
  D = erro - erroAnterior;
  PID = (Kp * P) + (Ki * I) + (Kd*D);
  erroAnterior = erro;
}
