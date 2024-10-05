#include "motores.h"

// define os sensores
// SENSOR_0 == ESQUERDA :: SENSOR_4 == DIREITA
const int SENSOR_0 = 14;
const int SENSOR_1 = 15;
const int SENSOR_2 = 16;
const int SENSOR_3 = 17;
const int SENSOR_4 = 18;

//sensores para a curva
const int SENSOR_ESQUERDA = 19;
const int SENSOR_DIREITA = 20;

// variaveis para ler a saida do sensor
int SENSOR[5];
int SENSOR_CURVA[2];

#define BRANCO 0 
#define PRETO 1
#define OFFSET 0
#define LINHA_NAO_DETECTADA -5
#define QUANTIDADE_TOTAL_SENSORES 5

const int velocidadeBaseDireita = 160; //160
const int velocidadeBaseEsquerda = 160; //210
int velocidadeDireita = 0;
int velocidadeEsquerda = 0;

// variaveis para o calculo do PID
float erro = 0; float erroAnterior = 0;
float I = 0, P = erro, D = 0, PID = 0;

//utilizacao de UltimateGain 35 e 12 ate agora
//const float Kcr = 150, Pcr = 0.5;

//parece que o melhor Kp é [125]
const float Kp = 125, Ki = 0, Kd = 0;
//const float Kp = (0.6 * Kcr), Ki = ((2 * Kp) / Pcr), Kd = ((Kp * Pcr) / 8); 

//apenas para testar o carro
unsigned long tempoInicial = millis();

void setup() {
  pinMode(SENSOR_0, INPUT);
  pinMode(SENSOR_1, INPUT);
  pinMode(SENSOR_2, INPUT);
  pinMode(SENSOR_3, INPUT);
  pinMode(SENSOR_4, INPUT);

  parar();
  delay(2000);
  Serial.begin(9600);
  tempoInicial = millis();
}

void ler_sensores(){
  SENSOR[0] = digitalRead(SENSOR_0);
  SENSOR[1] = digitalRead(SENSOR_1);
  SENSOR[2] = digitalRead(SENSOR_2);
  SENSOR[3] = digitalRead(SENSOR_3);
  SENSOR[4] = digitalRead(SENSOR_4);

  SENSOR_CURVA[0] = digitalRead(SENSOR_ESQUERDA);
  SENSOR_CURVA[1] = digitalRead(SENSOR_DIREITA);
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

  velocidadeDireita = constrain(velocidadeBaseDireita + PID, 0, 200);
  velocidadeEsquerda = constrain(velocidadeBaseEsquerda - PID, 0, 200);
  andar(velocidadeDireita, velocidadeEsquerda);
}

bool verifica_curva_90() {
  if (SENSOR_CURVA[0] == BRANCO && SENSOR[0] == BRANCO && SENSOR[1] == PRETO && SENSOR[2] == BRANCO && SENSOR[3] == PRETO && SENSOR[4] == PRETO && SENSOR_CURVA[1] == PRETO) {
    //Serial.println("CURVA A ESQUERDA");
    return true;
  } else if (SENSOR_CURVA[0] == PRETO && SENSOR[0] == PRETO && SENSOR[1] == PRETO && SENSOR[2] == BRANCO && SENSOR[3] == PRETO && SENSOR[4] == BRANCO && SENSOR_CURVA[1] == BRANCO) { 
    //Serial.println("CURVA A DIREITA");
    return true;
  } else if (SENSOR_CURVA[0] == BRANCO && SENSOR[0] == BRANCO && SENSOR[1] == PRETO && SENSOR[2] == BRANCO && SENSOR[3] == PRETO && SENSOR[4] == BRANCO && SENSOR_CURVA[1] == BRANCO) {
    //Serial.println("curva em duvida");
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
  for (int i = 0; i < 5; i++) {
    Serial.print(SENSOR[i]);
    Serial.print(" | ");
  }

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
