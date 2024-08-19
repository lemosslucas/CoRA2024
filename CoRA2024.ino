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
#define TEMPO_CURVA 300
#define OFFSET 10

int velocidadeBaseDireita = 170;
int velocidadeBaseEsquerda = 170;
const int velocidadeCurva90 = 170;

// variaveis para o calculo do PID
int erro = 0; int PID = 0; int erroAnterior = 0;
int I = 0; int P = erro; int D = 0; 
const int Ki = 1, Kd = 5, Kp = 10; 
bool carro_centralizado = true;

void setup() {
  pinMode(SENSOR_0, INPUT);
  pinMode(SENSOR_1, INPUT);
  pinMode(SENSOR_2, INPUT);
  pinMode(SENSOR_3, INPUT);
  pinMode(SENSOR_4, INPUT);

  delay(1000);
  andar(velocidadeBaseDireita, velocidadeBaseEsquerda);
  delay(TEMPO_CURVA);
  andar_de_re(velocidadeBaseDireita, velocidadeBaseEsquerda);
  delay(TEMPO_CURVA);
  parar();
  Serial.begin(9600);
  delay(4000);
}

void ler_sensores(){
  SENSOR[0] = digitalRead(SENSOR_0);
  SENSOR[1] = digitalRead(SENSOR_1);
  SENSOR[2] = digitalRead(SENSOR_2);
  SENSOR[3] = digitalRead(SENSOR_3);
  SENSOR[4] = digitalRead(SENSOR_4);
}

void loop() {
  bool curva_detectada;

  ler_sensores();  
  calcula_erro();
  curva_detectada = verifica_curva_90();

  if (curva_detectada == false) {
    if (erro != NULL) {
      erro = 0;
      calcula_PID();
      ajusta_movimento();
    } else {
      parar();
    }
  }
  
  Serial.print("Erro: ");
  Serial.print(erro);
  Serial.print(" PID: ");
  Serial.print(PID);
  Serial.print(" Velocidade Direita: ");
  Serial.print(velocidadeBaseDireita - PID);
  Serial.print(" Velocidade Esquerda: ");
  Serial.println(velocidadeBaseEsquerda + PID);

  delay(50);
}

void ajusta_movimento() {
  int velocidadeDireita = constrain(velocidadeBaseDireita - PID, 0, 255);
  int velocidadeEsquerda = constrain(velocidadeBaseEsquerda + PID, 0, 255);

  andar(velocidadeDireita, velocidadeEsquerda);
}

bool verifica_curva_90() {
  if (SENSOR[0] == BRANCO && SENSOR[1] == PRETO && SENSOR[2] == BRANCO && SENSOR[3] == PRETO && SENSOR[4] == PRETO) {
    Serial.println("curva esquerda"); 
    carro_centralizado = verifica_carro_centralizado();

    while(!carro_centralizado) {
      curva_esquerda(velocidadeCurva90, velocidadeCurva90);
    } 
    parar();

    return true;
  } else if (SENSOR[0] == PRETO && SENSOR[1] == PRETO && SENSOR[2] == BRANCO && SENSOR[3] == PRETO && SENSOR[4] == BRANCO) {
    Serial.println("curva direita");
    carro_centralizado = verifica_carro_centralizado();

    while(!carro_centralizado) {
      curva_direita(velocidadeCurva90, velocidadeCurva90);
    } 
    parar();

    return true;
  } else if (SENSOR[0] == BRANCO && SENSOR[1] == PRETO && SENSOR[2] == BRANCO && SENSOR[3] == PRETO && SENSOR[4] == BRANCO) {
    Serial.println("curva em duvida");
    carro_centralizado = verifica_carro_centralizado();

    // vai decidir no dia qual trajeto é menor
    while(!carro_centralizado) {
      curva_direita(velocidadeCurva90, velocidadeCurva90); 
      // curva_esquerda(velocidadeCurva90, velocidadeCurva90);
    } 
    parar();

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
      erro = NULL;
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
  P = erro;
  I = constrain(I+P, 1, 255);
  D = erro - erroAnterior;
  PID = (Kp * P) + (Ki * I) + (Kd*D) + OFFSET;
  erroAnterior = erro;
}