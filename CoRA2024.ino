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
#define OFFSET 0
#define PARAR -5

int velocidadeBaseDireita = 150; //150
int velocidadeBaseEsquerda = 170; //170
const int velocidadeCurva90 = 170;

// variaveis para o calculo do PID
int erro = 0; int erroAnterior = 0;
float I = 0, P = erro, D = 0, PID = 0; 
const float Ki = 0.1, Kd = 0.5, Kp = 45; 

void setup() {
  pinMode(SENSOR_0, INPUT);
  pinMode(SENSOR_1, INPUT);
  pinMode(SENSOR_2, INPUT);
  pinMode(SENSOR_3, INPUT);
  pinMode(SENSOR_4, INPUT);

  Serial.begin(9600);
  delay(1000);
}

void ler_sensores(){
  SENSOR[0] = digitalRead(SENSOR_0);
  SENSOR[1] = digitalRead(SENSOR_1);
  SENSOR[2] = digitalRead(SENSOR_2);
  SENSOR[3] = digitalRead(SENSOR_3);
  SENSOR[4] = digitalRead(SENSOR_4);
}

void ajusta_movimento() {
  int velocidadeDireita = constrain(velocidadeBaseDireita + PID, 1, 255);
  int velocidadeEsquerda = constrain(velocidadeBaseEsquerda - PID, 1, 255);
  andar(velocidadeDireita, velocidadeEsquerda);
}

bool verifica_curva_90() {
  if (SENSOR[0] == BRANCO && SENSOR[1] == PRETO && SENSOR[2] == BRANCO && SENSOR[3] == PRETO && SENSOR[4] == PRETO) {
    Serial.println("CURVA A ESQUERDA");
    return true;
  } else if (SENSOR[0] == PRETO && SENSOR[1] == PRETO && SENSOR[2] == BRANCO && SENSOR[3] == PRETO && SENSOR[4] == BRANCO) { 
    Serial.println("CURVA A DIREITA");
    return true;
  } else if (SENSOR[0] == BRANCO && SENSOR[1] == PRETO && SENSOR[2] == BRANCO && SENSOR[3] == PRETO && SENSOR[4] == BRANCO) {
    Serial.println("curva em duvida");
    return true;
  }
  return false;
}

void calcula_erro() {
  if (SENSOR[0] == PRETO && SENSOR[4] == PRETO) {
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
      erro = PARAR;
    } 
  }
}

bool verifica_carro_centralizado() {
  ler_sensores();
  if (SENSOR[1] == PRETO && SENSOR[2] == BRANCO && SENSOR[3] == PRETO) {
    return true;
  } 
  return false;
}

void calcula_PID() {
  PID = 0;
  P = erro;
  I = constrain(I+P, -255, 255);
  D = erro - erroAnterior;
  PID = (Kp * P) + (Ki * I) + (Kd*D) + OFFSET;
  erroAnterior = erro;
}

void imprime_serial() {
  Serial.print("Erro: ");
  Serial.print(erro);
  Serial.print(" PID: ");
  Serial.print(PID);
  Serial.print(" Velocidade Direita: ");
  Serial.print(constrain(velocidadeBaseDireita + PID, 1, 255));
  Serial.print(" Velocidade Esquerda: ");
  Serial.println(constrain(velocidadeBaseEsquerda - PID, 1, 255));
}

void loop() {
  ler_sensores();  
  calcula_erro();

  imprime_serial();

  if (verifica_curva_90()) {
    while(!verifica_carro_centralizado()) {
      if (SENSOR[0] == BRANCO) {
        curva_esquerda(velocidadeCurva90, velocidadeCurva90);
      } else {
        curva_direita(velocidadeCurva90, velocidadeCurva90);
      }
      delay(100);
    }
    parar();
    delay(100);
  } else {
    if (erro != PARAR) {
      calcula_PID();
      ajusta_movimento();
    } else {
      parar();
      erro = 0;
    }
  }
  
  delay(10);
}
