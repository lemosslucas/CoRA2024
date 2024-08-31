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

const int velocidadeBaseDireita = 160; //160
const int velocidadeBaseEsquerda = 210; //210

// variaveis para o calculo do PID
int erro = 0; int erroAnterior = 0;
float I = 0, P = erro, D = 0, PID = 0; 
//utilizacao de UltimateGain 35 e 12 ate agora
const float Kcr = 35, Pcr = 12;

//parece que o melhor Kp é 35
const float Kp = 35, Ki = 0, Kd = 0;

//const float Kp = (0.6 * Kcr); 
//const float Ki = ((2 * Kp) / Pcr);
//const float Kd = ((Kp * Pcr) / 8); 


//apenas para testar o carro
unsigned long tempoInicial = millis();

void setup() {
  pinMode(SENSOR_0, INPUT);
  pinMode(SENSOR_1, INPUT);
  pinMode(SENSOR_2, INPUT);
  pinMode(SENSOR_3, INPUT);
  pinMode(SENSOR_4, INPUT);

  Serial.begin(9600);
  
  andar(velocidadeBaseDireita, velocidadeBaseEsquerda);
  //delay(10);
  parar();
  delay(1500);
  tempoInicial = millis();
}

void ler_sensores(){
  SENSOR[0] = digitalRead(SENSOR_0);
  SENSOR[1] = digitalRead(SENSOR_1);
  SENSOR[2] = digitalRead(SENSOR_2);
  SENSOR[3] = digitalRead(SENSOR_3);
  SENSOR[4] = digitalRead(SENSOR_4);
}

void ajusta_movimento() {
  int velocidadeDireita = constrain(velocidadeBaseDireita + PID, 1, 180);
  int velocidadeEsquerda = constrain(velocidadeBaseEsquerda - PID, 1, 220);
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
    } else {
      erro = 0;
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
  /*
  Serial.print("Erro: ");
  Serial.print(erro);
  Serial.print(" PID: ");
  Serial.print(PID);
  Serial.print(" Velocidade Direita: ");
  Serial.print(constrain(velocidadeBaseDireita + PID, 1, 180));
  Serial.print(" Velocidade Esquerda: ");
  Serial.println(constrain(velocidadeBaseEsquerda - PID, 1, 220));
  */
  Serial.print(erro);
  Serial.print("\t");
  Serial.print(PID);
  Serial.print("\t");
  Serial.println(millis() - tempoInicial);
}

void loop() {
  ler_sensores();  
  calcula_erro();

  if(erro != PARAR) {
    imprime_serial();
  }

  if (verifica_curva_90()) {
    if (SENSOR[0] == BRANCO) {
      andar(velocidadeBaseDireita, velocidadeBaseEsquerda);
      delay(500);
      parar();
      curva_esquerda(velocidadeBaseDireita, velocidadeBaseEsquerda);
      delay(200);
    } else if (SENSOR[0] == BRANCO && SENSOR[4] == BRANCO) {
      //curva_esquerda(velocidadeCurva90, velocidadeCurva90);
    } else {
      //curva_direita(velocidadeCurva90, velocidadeCurva90);
    }

  } else {
    if (erro != PARAR) {
      calcula_PID();
      ajusta_movimento();
    } else {
      parar();
    }
  } 
  delay(50);
}
