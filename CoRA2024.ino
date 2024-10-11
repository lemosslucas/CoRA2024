#include "motores.h"
#include "desafios.h"

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
#define CURVA_NAO_ENCONTRADA 0

#define DETECCAO_POR_QUADRADO 1
#define SAIDA_ESQUERDA 0
#define SAIDA_DIREITA 1
int saida_rotatoria = -1;

const int velocidadeBaseDireita = 160;   //160
const int velocidadeBaseEsquerda = 180;  //210
int velocidadeDireita = 0;
int velocidadeEsquerda = 0;

// variaveis para o calculo do PID
float erro = 0;
float erroAnterior = 0;
float I = 0, P = erro, D = 0, PID = 0;

//utilizacao de UltimateGain 35 e 12 ate agora
//const float Kcr = 150, Pcr = 0.05;

//parece que o melhor Kp é [150]
const float Kp = 150, Ki = 0, Kd = 0;
//const float Kp = (0.6 * Kcr), Ki = ((2 * Kp) / Pcr), Kd = ((Kp * Pcr) / 8);

//apenas para testar o carro
unsigned long tempoInicial = millis();
bool faixa_de_pedestre = false;

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

void ler_sensores() {
  SENSOR[0] = digitalRead(sensor1_A1);
  SENSOR[1] = digitalRead(sensor2_A2);
  SENSOR[2] = digitalRead(sensor3_A3);
  SENSOR[3] = digitalRead(sensor4_A4);
  SENSOR[4] = digitalRead(sensor5_A5);

  SENSOR_CURVA[0] = digitalRead(sensor0_curva_A0);
  SENSOR_CURVA[1] = digitalRead(sensor6_curva_A6);
}

void ajusta_movimento() {
  velocidadeDireita = constrain(velocidadeBaseDireita - PID, 0, 255);
  velocidadeEsquerda = constrain(velocidadeBaseEsquerda + PID, 0, 255);
  andar(velocidadeDireita, velocidadeEsquerda);
}

void calcula_erro() {
  ler_sensores();

  if (verifica_inversao(SENSOR, SENSOR_CURVA)) {
    faixa_de_pedestre = true;
  }

  int pesos[5] = {-2, -1, 0, 1, 2};
  int somatorioErro = 0;
  int sensoresAtivos = 0;

  for (int i = 0; i < 5; i++) {
    somatorioErro += SENSOR[i] * pesos[i];
    sensoresAtivos += SENSOR[i];
  }

  // || (sensoresAtivos == 0 && SENSOR_CURVA[0] == BRANCO && SENSOR_CURVA[1] == BRANCO)
  if (sensoresAtivos == QUANTIDADE_TOTAL_SENSORES) {
    erro = LINHA_NAO_DETECTADA;
  } else {
    int sensoresInativos = QUANTIDADE_TOTAL_SENSORES - sensoresAtivos;
    erro = somatorioErro / sensoresInativos;
  }
}

void calcula_PID() {
  PID = 0;
  P = erro;
  I = constrain(I + P, -255, 255);
  D = erro - erroAnterior;

  PID = (Kp * P) + (Ki * I) + (Kd * D) + OFFSET;

  erroAnterior = erro;
}

void imprime_serial() {
  /*
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
  
  */
  /* 
  //usado para a parte de geracao do grafico
  Serial.print(erro);
  Serial.print("\t");
  Serial.print(PID);
  Serial.print("\t");
  Serial.println(millis() - tempoInicial);
  */
}

int determina_saida_rotatoria(int marcacoesEsquerda, int marcacoesDireita) {
  int saidaDesejada = 0;

  if (marcacoesEsquerda > marcacoesDireita) {
    saidaDesejada = (marcacoesEsquerda / DETECCAO_POR_QUADRADO) + 1;
    curva_esquerda(velocidadeBaseDireita, velocidadeBaseEsquerda);
    saida_rotatoria = SAIDA_ESQUERDA;
  } else if(marcacoesEsquerda < marcacoesDireita) {
    saidaDesejada = (marcacoesDireita / DETECCAO_POR_QUADRADO) + 1; 
    curva_direita(velocidadeBaseDireita, velocidadeBaseEsquerda);
    saida_rotatoria = SAIDA_DIREITA;
  }
  
  return saidaDesejada;
}

void realiza_rotatoria(int saidaDesejada){
  int saidaAtual = 1;

  while(saidaAtual != saidaDesejada) {
    calcula_erro();
    ajusta_movimento();

    if (saida_rotatoria == SAIDA_ESQUERDA) {
      if (calcula_sensores_ativos(SENSOR) <= 3 && SENSOR_CURVA[0] == PRETO && SENSOR_CURVA[1] == BRANCO) {
        // a condicao de parar é so um teste pra ver se esta tudo ok
        parar();
        delay(200);
        saidaAtual++;
      }
    } else if(saida_rotatoria == SAIDA_DIREITA) {
      if (calcula_sensores_ativos(SENSOR) <= 3 && SENSOR_CURVA[0] == BRANCO && SENSOR_CURVA[1] == PRETO) {
        // a condicao de parar é so um teste pra ver se esta tudo ok
        parar();
        delay(200);
        saidaAtual++;
      }
    }
  }
  
  if (saida_rotatoria == SAIDA_ESQUERDA) {
    curva_esquerda(velocidadeBaseDireita, velocidadeBaseEsquerda);
  } else {
    curva_direita(velocidadeBaseDireita, velocidadeBaseEsquerda);
  }
}

void realiza_marcha_re(int historico[]) {
  /* talvez precise desse for para deteccao
  for (int i = 0; i < DETECCAO_POR_QUADRADO; i++) {
    if (i > 0) {
      historico[i] == historico[i - 1];
    } 
  }
  */

  while (erro != LINHA_NAO_DETECTADA) {
    andar_de_re(255, 255);
    calcula_erro();
  }
  parar();
  delay(500);

  // se o primeiro for esquerda ele vira a direita
  if (historico[1] == SAIDA_ESQUERDA) {
    curva_direita(velocidadeBaseDireita, velocidadeBaseEsquerda);
  } else {
    curva_esquerda(velocidadeBaseDireita, velocidadeBaseEsquerda);
  }
}

void loop() {
  int marcacoesDireita = 0, marcacoesEsquerda = 0;
  calcula_erro();

  int saidaCurva = verifica_curva_90(SENSOR, SENSOR_CURVA);
  int historico_curva[4 * DETECCAO_POR_QUADRADO];
  int i = 0;

  if (saidaCurva != CURVA_NAO_ENCONTRADA) { 
    while(erro != LINHA_NAO_DETECTADA) { // < 3
      ler_sensores();
      
      if (SENSOR_CURVA[0] == BRANCO) {
        marcacoesEsquerda++;
        historico_curva[i] = SAIDA_ESQUERDA;
      } else if (SENSOR_CURVA[1] == BRANCO) {
        marcacoesDireita++;
        historico_curva[i] = SAIDA_DIREITA;
      }

      i++;
      calcula_erro();
      calcula_PID();
      ajusta_movimento();
    }
    
    if (marcacoesEsquerda == DETECCAO_POR_QUADRADO || marcacoesDireita == DETECCAO_POR_QUADRADO) {
      realiza_curva_90(saidaCurva);
    } else if ((marcacoesEsquerda / DETECCAO_POR_QUADRADO) > 1 && (marcacoesDireita / DETECCAO_POR_QUADRADO) > 1) {
      realiza_marcha_re(historico_curva);
    } else{
      determina_saida_rotatoria(marcacoesEsquerda, marcacoesDireita);
    }

    parar();
  } else {
    if (erro == LINHA_NAO_DETECTADA) {
      int sensoresAtivos = calcula_sensores_ativos(SENSOR);
      PID = 0;
      
      parar();

      if (faixa_de_pedestre == true) {
        realiza_faixa_de_pedestre();
        faixa_de_pedestre = false;
      } else {  
        delay(10000);

        while(sensoresAtivos == QUANTIDADE_TOTAL_SENSORES) {
          ler_sensores();
          sensoresAtivos = calcula_sensores_ativos(SENSOR);
          andar_de_re(255, 255);
        }

        parar();
      }

    } else {
      calcula_PID();
      ajusta_movimento();
    }
  }

  imprime_serial();
  delay(5);
}
