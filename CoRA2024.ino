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
#define LINHA_NAO_DETECTADA -5
#define QUANTIDADE_TOTAL_SENSORES 5

const int velocidadeBaseDireita = 160; //160
const int velocidadeBaseEsquerda = 210; //210

// variaveis para o calculo do PID
float erro = 0; float erroAnterior = 0;
float I = 0, P = erro, D = 0, PID = 0; 
//utilizacao de UltimateGain 35 e 12 ate agora
//const float Kcr = 150, Pcr = 0.5;

//parece que o melhor Kp é 50
const float Kp = 55, Ki = 0, Kd = 0;
//const float Kp = (0.6 * Kcr), Ki = ((2 * Kp) / Pcr), Kd = ((Kp * Pcr) / 8); 

//apenas para testar o carro
unsigned long tempoInicial = millis();

void setup() {
  pinMode(SENSOR_0, INPUT);
  pinMode(SENSOR_1, INPUT);
  pinMode(SENSOR_2, INPUT);
  pinMode(SENSOR_3, INPUT);
  pinMode(SENSOR_4, INPUT);

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
}

void ajusta_movimento() {
  int velocidadeDireita = constrain(velocidadeBaseDireita + PID, 1, 200);
  int velocidadeEsquerda = constrain(velocidadeBaseEsquerda - PID, 1, 200);
  andar(velocidadeDireita, velocidadeEsquerda);
}


bool verifica_curva_90() {
  if (SENSOR[0] == BRANCO && SENSOR[1] == PRETO && SENSOR[2] == BRANCO && SENSOR[3] == PRETO && SENSOR[4] == PRETO) {
    //Serial.println("CURVA A ESQUERDA");
    return true;
  } else if (SENSOR[0] == PRETO && SENSOR[1] == PRETO && SENSOR[2] == BRANCO && SENSOR[3] == PRETO && SENSOR[4] == BRANCO) { 
    //Serial.println("CURVA A DIREITA");
    return true;
  } else if (SENSOR[0] == BRANCO && SENSOR[1] == PRETO && SENSOR[2] == BRANCO && SENSOR[3] == PRETO && SENSOR[4] == BRANCO) {
    //Serial.println("curva em duvida");
    return true;
  }
  return false;
}

void calcula_erro() {
  ler_sensores(); 

  int pesos[5] = {-2, -1, 0, 2, 1};
  int somatorioErro = 0;
  int sensoresInativos = 0;

  for(int i = 0; i < 5; i++) {
    somatorioErro += SENSOR[i] * pesos[i];
    sensoresInativos += SENSOR[i];
  }

  if(sensoresInativos == QUANTIDADE_TOTAL_SENSORES){
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
  Serial.print(SENSOR[0]);
  Serial.print(SENSOR[1]);
  Serial.print(SENSOR[2]);
  Serial.print(SENSOR[3]);
  Serial.print(SENSOR[4]);

  Serial.print("Erro: ");
  Serial.print(erro);
  Serial.print(" PID: ");
  Serial.print(PID);
  Serial.print(" Velocidade Direita: ");
  Serial.print(constrain(velocidadeBaseDireita + PID, 1, 200));
  Serial.print(" Velocidade Esquerda: ");
  Serial.println(constrain(velocidadeBaseEsquerda - PID, 1, 200));
  

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
  imprime_serial();

  if (erro == LINHA_NAO_DETECTADA) {
    Serial.println("LINHA_NAO_DETECTADA");
    parar();
  } else {
    calcula_PID();
    ajusta_movimento();
  }  

  delay(5);
}
