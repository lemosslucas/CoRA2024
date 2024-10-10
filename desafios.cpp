#include "desafios.h"
#include "motores.h"

#define BRANCO 0 
#define PRETO 1
#define QUANTIDADE_TOTAL_SENSORES 5

#define CURVA_ESQUERDA 1
#define CURVA_DIREITA 2
#define CURVA_EM_DUVIDA 3
#define CURVA_NAO_ENCONTRADA 0

const int velocidadeBaseDireita = 160; //160
const int velocidadeBaseEsquerda = 180; //210

// um sensor ativo eh quando a saida do sensor for 1(PRETO)
int calcula_sensores_ativos(int SENSOR[]) {
  int sensoresAtivos = 0;
  
  for(int i = 0; i < 5; i++) {
    sensoresAtivos += SENSOR[i];
  }
  
  return sensoresAtivos;
}

int calcula_erro_sensores(int SENSOR[]) {
  int pesos[5] = {-2, -1, 0, 1, 2};
  int somatorioErro = 0;
  int sensoresAtivos = 0;

  for (int i = 0; i < 5; i++) {
    somatorioErro += SENSOR[i] * pesos[i];
    sensoresAtivos += SENSOR[i];
  }

  int sensoresInativos = QUANTIDADE_TOTAL_SENSORES - sensoresAtivos;
  return somatorioErro / sensoresInativos;
}

int verifica_curva_90(int SENSOR[], int SENSOR_CURVA[]) {
  if (SENSOR_CURVA[0] == BRANCO && SENSOR[0] == BRANCO && SENSOR[1] == PRETO && SENSOR[2] == BRANCO && SENSOR[3] == PRETO && SENSOR[4] == PRETO && SENSOR_CURVA[1] == PRETO) {
    return CURVA_ESQUERDA;
  } else if (SENSOR_CURVA[0] == PRETO && SENSOR[0] == PRETO && SENSOR[1] == PRETO && SENSOR[2] == BRANCO && SENSOR[3] == PRETO && SENSOR[4] == BRANCO && SENSOR_CURVA[1] == BRANCO) {
    return CURVA_DIREITA;
  } else if (SENSOR_CURVA[0] == BRANCO && SENSOR[0] == BRANCO && SENSOR[1] == PRETO && SENSOR[2] == BRANCO && SENSOR[3] == PRETO && SENSOR[4] == BRANCO && SENSOR_CURVA[1] == BRANCO) {
    return CURVA_EM_DUVIDA;
  }
  
  /*
  int erro = calcula_erro_sensores(SENSOR);
  Serial.println(erro);
  
  if (erro >= -1 && erro <= 1) {
    if (SENSOR_CURVA[0] == BRANCO && SENSOR_CURVA[1] == PRETO) {
      Serial.print("EU vi");
      return CURVA_ESQUERDA;
    } else if (SENSOR_CURVA[0] == PRETO && SENSOR_CURVA[1] == BRANCO) {
      return CURVA_DIREITA;
    } else if (SENSOR_CURVA[0] == BRANCO && SENSOR_CURVA[1] == BRANCO) {
      return CURVA_EM_DUVIDA;
    }
  }
  */ 
  return CURVA_NAO_ENCONTRADA;
}

void realiza_curva_90(int curvaEncontrada) {
  parar();
  delay(200);

  if (curvaEncontrada == CURVA_ESQUERDA) {
    curva_esquerda(velocidadeBaseDireita, velocidadeBaseEsquerda);
  } else if (curvaEncontrada == CURVA_DIREITA) {
    curva_direita(velocidadeBaseDireita, velocidadeBaseEsquerda);
  } else if (curvaEncontrada == CURVA_EM_DUVIDA) {
    //curva_esquerda(velocidadeBaseDireita, velocidadeBaseEsquerda);
    curva_direita(velocidadeBaseDireita, velocidadeBaseEsquerda);
  }
}

int inverte_sensor(int sensor){
  if (sensor == 1){ 
    return 0;
  } 
  return 1;
}

void verifica_inversao(int SENSOR[]) {
  if (calcula_sensores_ativos(SENSOR) == 1) {
    for (int i = 0; i < 5; i++) {
      SENSOR[i] = inverte_sensor(SENSOR[i]);
    }
  }
}

void volta_inatividade(int velocidadeDireita, int velocidadeEsquerda) {
  delay(10000);
  andar_de_re(velocidadeDireita, velocidadeEsquerda);
  delay(250);
  parar();
  delay(100);
}


