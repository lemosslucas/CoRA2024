#include "desafios.h"
#include "motores.h"

#define BRANCO 0 
#define PRETO 1
#define QUANTIDADE_TOTAL_SENSORES 5

#define CURVA_ESQUERDA 1
#define CURVA_DIREITA 2
#define CURVA_EM_DUVIDA 3
#define CURVA_NAO_ENCONTRADA 0

#define DETECCAO_POR_QUADRADO 1

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

int verifica_curva_90(int SENSOR[], int SENSOR_CURVA[]) {
  if (SENSOR_CURVA[0] == BRANCO && SENSOR[0] == BRANCO && SENSOR[1] == PRETO && SENSOR[2] == BRANCO && SENSOR[3] == PRETO && SENSOR[4] == PRETO && SENSOR_CURVA[1] == PRETO
  || calcula_sensores_ativos(SENSOR) == 3 && SENSOR_CURVA[0] == BRANCO && SENSOR_CURVA[1] == PRETO) {
    return CURVA_ESQUERDA;
  } else if (SENSOR_CURVA[0] == PRETO && SENSOR[0] == PRETO && SENSOR[1] == PRETO && SENSOR[2] == BRANCO && SENSOR[3] == PRETO && SENSOR[4] == BRANCO && SENSOR_CURVA[1] == BRANCO) {
    return CURVA_DIREITA;
  } else if (SENSOR_CURVA[0] == BRANCO && SENSOR[0] == BRANCO && SENSOR[1] == PRETO && SENSOR[2] == BRANCO && SENSOR[3] == PRETO && SENSOR[4] == BRANCO && SENSOR_CURVA[1] == BRANCO) {
    return CURVA_EM_DUVIDA;
  }

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
    // lado determinado no dia da prova
    //curva_esquerda(velocidadeBaseDireita, velocidadeBaseEsquerda);
    curva_direita(velocidadeBaseDireita, velocidadeBaseEsquerda);
  }
}

/**
 * @brief Inverte o valor obtido pelos sensores
 * 
 * @param int sensor - saida dos sensores
 * 
 * Responsavel por garantir que o carro siga a linha quando houver
 * a inversao de cores preto/branco -> branco/preto aplicando a 
 * logica de uma porta logica NOT.
 */
int inverte_sensor(int sensor){
  if (sensor == 1){ 
    return 0;
  } 
  return 1;
}

inverte_sensor()

bool verifica_inversao(int SENSOR[], int SENSOR_CURVA[]) {
  if (calcula_sensores_ativos(SENSOR) == 1 && SENSOR_CURVA[0] == BRANCO && SENSOR_CURVA[1] == BRANCO) {
    for (int i = 0; i < 5; i++) {
      SENSOR[i] = inverte_sensor(SENSOR[i]);
    }
    return true;
  }
  
  if (calcula_sensores_ativos(SENSOR) == 1) {
    for (int i = 0; i < 5; i++) {
      SENSOR[i] = inverte_sensor(SENSOR[i]);
    }
    return true;
  }
  
  return false;
}

void realiza_faixa_de_pedestre() {
  delay(6000);
  andar(255, 255);
  delay(2000); 
}

 