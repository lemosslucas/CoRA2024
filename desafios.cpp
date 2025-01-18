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

// define a velocidade base
const int velocidadeBaseDireita = 160; //160
const int velocidadeBaseEsquerda = 180; //210

/**
 * @brief Calcula o numero de sensores ativos
 * 
 * @param SENSOR Array com as saidas dos sensores
 * 
 * @note Um sensor esta no estado ativo é quando a saida do sensor
 * for igual a 1 = Preto
 * 
 * @return int sensoresAtivos- O numero de sensores ativos
 */
int calcula_sensores_ativos(int SENSOR[]) {
  int sensoresAtivos = 0;
  // calcula o numero de sensores ativos
  for(int i = 0; i < 5; i++) {
    sensoresAtivos += SENSOR[i];
  }
  
  // retorna o numero de sensores ativos
  return sensoresAtivos;
}

/**
 * @brief Determina se o carro deve realizar uma curva de 90 graus.
 * 
 * Esta função analisa o estado dos sensores principais e dos sensores de curva 
 * para identificar se o carro deve realizar uma curva de 90 graus e, em caso 
 * positivo, determina o lado da curva.
 * 
 * @param SENSOR Array contendo os estados dos sensores principais.
 * @param SENSOR_CURVA Array contendo os estados dos sensores responsáveis pela 
 * detecção da curva de 90 graus.
 * 
 * @return int Representa o lado da curva:
 * - CURVA_ESQUERDA se a curva for para a esquerda.
 * - CURVA_DIREITA se a curva for para a direita.
 * - CURVA_EM_DUVIDA se não houver certeza sobre o lado da curva.
 * - CURVA_NAO_ENCONTRADA se nenhuma curva for detectada.
 */

int verifica_curva_90(int SENSOR[], int SENSOR_CURVA[]) {
  // verifica se a curva é para esquerda
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

/**
 * @brief Realiza a curva de 90
 * 
 * @param curvaEncontrada - lado que deve ser feito a curva
 * 
 * A partir do lado determinado da curva a função realiza o movimento de curva
 * de 90.
 */
void realiza_curva_90(int curvaEncontrada) {
  // para o carro para maior estabilidade
  parar();
  delay(200);

  // verifica qual lado deve ser feito a curva
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

/**
 * @brief Verifica se houve uma inversão nas cores da pista.
 * 
 * Esta função analisa o estado dos sensores principais e dos sensores de curva 
 * para determinar se houve uma inversão nas cores da pista (preto/branco). 
 * Caso detectada, a função inverte os valores dos sensores principais para 
 * que o carro possa continuar seguindo a linha.
 * 
 * @param SENSOR Array contendo os estados dos sensores principais.
 *               Cada posição representa o estado de um sensor.
 * @param SENSOR_CURVA Array contendo os estados dos sensores de curva.
 *                     Usado para complementar a verificação de inversão.
 * 
 * @return bool Retorna:
 * - `true` se uma inversão foi detectada e os sensores principais foram invertidos.
 * - `false` se nenhuma inversão foi detectada.
 */
bool verifica_inversao(int SENSOR[], int SENSOR_CURVA[]) {
  if (calcula_sensores_ativos(SENSOR) == 1 && SENSOR_CURVA[0] == BRANCO && SENSOR_CURVA[1] == BRANCO) {
    // inverte os estados dos sensores
    for (int i = 0; i < 5; i++) {
      SENSOR[i] = inverte_sensor(SENSOR[i]);
    }
    // retorna true para indiciar que houve inversão
    return true;
  }
  
  if (calcula_sensores_ativos(SENSOR) == 1) {
    // inverte os estados dos sensores
    for (int i = 0; i < 5; i++) {
      SENSOR[i] = inverte_sensor(SENSOR[i]);
    }
    // retorna true para indiciar que houve inversão
    return true;
  }
  
  // retorna false ja que não houve a inversão
  return false;
}

/**
 * @brief Realiza o movimento de atraversar a faixa de pedestre
 * 
 * Esta função simula o comportamento de um robô ao atravessar uma faixa 
 * de pedestres. Primeiro, ele espera o tempo mínimo necessário para garantir 
 * segurança antes de atravessar, depois avança para atravessar a pista.
 */
void realiza_faixa_de_pedestre() {
  // espera o tempo minimo de 5seg para poder atravessar
  delay(6000);
  // anda para frente para atravessar a pista
  andar(255, 255);
  delay(2000); 
}

 