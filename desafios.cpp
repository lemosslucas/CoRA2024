#include "desafios.h"
#include "motores.h"

#define BRANCO 0 
#define PRETO 1
#define QUANTIDADE_TOTAL_SENSORES 5

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

bool verifica_curva_90(int SENSOR[], int SENSOR_CURVA[]) {
  if (calcula_sensores_ativos(SENSOR) == 4) {
    if (SENSOR_CURVA[0] == BRANCO && SENSOR_CURVA[1] == PRETO ||
       SENSOR_CURVA[0] == PRETO && SENSOR_CURVA[0] == BRANCO ||
       SENSOR_CURVA[0] == BRANCO && SENSOR_CURVA[1] == BRANCO) {
      return true;
    }
  }
  return false;
}

void realiza_curva_90(int SENSOR_CURVA[]) {
  andar(velocidadeBaseDireita, velocidadeBaseEsquerda);
  delay(300);
  parar();

  if (SENSOR_CURVA[0] == BRANCO && SENSOR_CURVA[1] == PRETO) {
    curva_esquerda(velocidadeBaseDireita, velocidadeBaseEsquerda);
  } else if (SENSOR_CURVA[0] == PRETO && SENSOR_CURVA[1] == BRANCO) {
    curva_direita(velocidadeBaseDireita, velocidadeBaseEsquerda);
  } else if (SENSOR_CURVA[0] == BRANCO && SENSOR_CURVA[1] == BRANCO) {
    curva_esquerda(velocidadeBaseDireita, velocidadeBaseEsquerda);
    //curva_direita(velocidadeBaseDireita, velocidadeBaseEsquerda);
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

