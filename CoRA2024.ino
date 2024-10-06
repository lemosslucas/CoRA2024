#include "motores.h"

// define os sensores
// sensor1_A1 == ESQUERDA :: sensor5_A5 == DIREITA // sensor0_curva_A0 == ESQUERDA & sensor6_curva_A6 == DIREITA
const int sensor1_A1 = 15;
const int sensor2_A2 = 16;
const int sensor3_A3 = 17;
const int sensor4_A4 = 18;
const int sensor5_A5 = 19;

//sensores para a curva
const int sensor0_curva_A0 = 14;
const int sensor6_curva_A6 = 10;

// variaveis para ler a saida do sensor
int SENSOR[5];
int SENSOR_CURVA[2];

#define BRANCO 0 
#define PRETO 1
#define OFFSET 0
#define LINHA_NAO_DETECTADA -5
#define QUANTIDADE_TOTAL_SENSORES 5

const int velocidadeBaseDireita = 160; //160
const int velocidadeBaseEsquerda = 180; //210
int velocidadeDireita = 0;
int velocidadeEsquerda = 0;

// variaveis para o calculo do PID
float erro = 0; float erroAnterior = 0;
float I = 0, P = erro, D = 0, PID = 0;

//utilizacao de UltimateGain 35 e 12 ate agora
//const float Kcr = 150, Pcr = 0.5;

//parece que o melhor Kp é [125]
const float Kp = 150, Ki = 0, Kd = 0;
//const float Kp = (0.6 * Kcr), Ki = ((2 * Kp) / Pcr), Kd = ((Kp * Pcr) / 8); 

//apenas para testar o carro
unsigned long tempoInicial = millis();

void setup() {
  pinMode(sensor1_A1, INPUT);
  pinMode(sensor2_A2, INPUT);
  pinMode(sensor3_A3, INPUT);
  pinMode(sensor4_A4, INPUT);
  pinMode(sensor5_A5, INPUT);
  pinMode(sensor0_curva_A0, INPUT);
  pinMode(sensor6_curva_A6, INPUT);

  parar();
  delay(2000);
  Serial.begin(9600);
  tempoInicial = millis();
}

void ler_sensores(){
  SENSOR[0] = digitalRead(sensor1_A1);
  SENSOR[1] = digitalRead(sensor2_A2);
  SENSOR[2] = digitalRead(sensor3_A3);
  SENSOR[3] = digitalRead(sensor4_A4);
  SENSOR[4] = digitalRead(sensor5_A5);

  SENSOR_CURVA[0] = digitalRead(sensor0_curva_A0);
  SENSOR_CURVA[1] = digitalRead(sensor6_curva_A6);
}

void ajusta_movimento() {
  /*
  int velocidadeDireita, velocidadeEsquerda;

  if (PID >= 0) {
    velocidadeDireita = constrain(velocidadeBaseDireita - PID, 1, 200);
    velocidadeEsquerda = constrain(velocidadeBaseEsquerda + PID, 1, 200);
  } else {
    velocidadeDireita = constrain(velocidadeBaseDireita - PID, 1, 200);
    velocidadeEsquerda = constrain(velocidadeBaseEsquerda + PID, 1, 200);
  }
  
  andar(velocidadeDireita, velocidadeEsquerda);
  */

  velocidadeDireita = constrain(velocidadeBaseDireita - PID, 0, 170);
  velocidadeEsquerda = constrain(velocidadeBaseEsquerda + PID, 0, 190);
  andar(velocidadeDireita, velocidadeEsquerda);
}

bool verifica_curva_90() {
  if (SENSOR_CURVA[0] == BRANCO && SENSOR[0] == BRANCO && SENSOR[1] == PRETO && SENSOR[2] == BRANCO && SENSOR[3] == PRETO && SENSOR[4] == PRETO && SENSOR_CURVA[1] == PRETO) {
    //Serial.println("CURVA A ESQUERDA");
    Serial.println();
    Serial.println("Curva esquerda");
    Serial.println();
    return true;
  } else if (SENSOR_CURVA[0] == PRETO && SENSOR[0] == PRETO && SENSOR[1] == PRETO && SENSOR[2] == BRANCO && SENSOR[3] == PRETO && SENSOR[4] == BRANCO && SENSOR_CURVA[1] == BRANCO) { 
    
    Serial.println();
    Serial.println("Curva direita");
    Serial.println();

    return true;
  } else if (SENSOR_CURVA[0] == BRANCO && SENSOR[0] == BRANCO && SENSOR[1] == PRETO && SENSOR[2] == BRANCO && SENSOR[3] == PRETO && SENSOR[4] == BRANCO && SENSOR_CURVA[1] == BRANCO) {

    Serial.println();
    Serial.println("Curva em duvida");
    Serial.println();
    
    return true;
  }
  return false;
}

void realiza_curva_90() {
  if (SENSOR_CURVA[0] == BRANCO && SENSOR[1] == PRETO) {
    curva_esquerda(velocidadeBaseDireita, velocidadeBaseEsquerda);
  } else if (SENSOR[0] == PRETO && SENSOR[1] == BRANCO) {
    curva_direita(velocidadeBaseDireita, velocidadeBaseEsquerda);
  } else if (SENSOR[0] == BRANCO && SENSOR[1] == BRANCO) {
    curva_esquerda(velocidadeBaseDireita, velocidadeBaseEsquerda);
    //curva_direita(velocidadeBaseDireita, velocidadeBaseEsquerda);
  }
}

void calcula_erro() {
  ler_sensores(); 

  int pesos[5] = {-2, -1, 0, 1, 2};
  int somatorioErro = 0;
  int sensoresInativos = 0;

  for(int i = 0; i < 5; i++) {
    somatorioErro += SENSOR[i] * pesos[i];
    sensoresInativos += SENSOR[i];
  }

  if (sensoresInativos == QUANTIDADE_TOTAL_SENSORES){
    erro = LINHA_NAO_DETECTADA;
  } else {
    int sensoresAtivos = QUANTIDADE_TOTAL_SENSORES - sensoresInativos;
    erro = somatorioErro / sensoresAtivos;
  }
}

void calcula_PID() {
  PID = 0;
  P = erro;
  I = constrain(I + P, -255, 255);
  D = erro - erroAnterior;

  PID = (Kp * P) + (Ki * I) + (Kd*D) + OFFSET;

  erroAnterior = erro;
}

void imprime_serial() {
  Serial.print(SENSOR_CURVA[0]);
  Serial.print(" | ");

  for (int i = 0; i < 5; i++) {
    Serial.print(SENSOR[i]);
    Serial.print(" | ");
  }

  Serial.print(SENSOR_CURVA[1]);
  Serial.print(" | ");

  Serial.print("\tErro: ");
  Serial.print(erro);
  Serial.print(" PID: ");
  Serial.print(PID);
  Serial.print(" Velocidade Direita: ");
  Serial.print(velocidadeDireita);
  Serial.print(" Velocidade Esquerda: ");
  Serial.println(velocidadeEsquerda);

  /*
  Serial.print(erro);
  Serial.print("\t");
  Serial.print(PID);
  Serial.print("\t");
  Serial.println(millis() - tempoInicial);
  */
}

void loop() {
  calcula_erro();

  if (verifica_curva_90()) {
    andar(velocidadeBaseDireita, velocidadeBaseEsquerda);
    delay(300);
    parar();
    realiza_curva_90();
  } else {
    if (erro == LINHA_NAO_DETECTADA) {
      PID = 0;
      parar();
    } else {
      calcula_PID();
      ajusta_movimento();
    }  
  }

  imprime_serial();
  delay(5);
}
