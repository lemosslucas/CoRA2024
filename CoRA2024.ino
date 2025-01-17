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

const int velocidadeBaseDireita = 160; 
const int velocidadeBaseEsquerda = 180; 
int velocidadeDireita = 0;
int velocidadeEsquerda = 0;

// variaveis para o calculo do PID
float erro = 0;
float erroAnterior = 0;
float I = 0, P = erro, D = 0, PID = 0;

//Constante para a utilizacao do Ultimate Gain
//const float Kcr = 150, Pcr = 0.05;

//parece que o melhor Kp é [150]
const float Kp = 150, Ki = 0, Kd = 0;
//const float Kp = (0.6 * Kcr), Ki = ((2 * Kp) / Pcr), Kd = ((Kp * Pcr) / 8);

// variavel para deteccao da faixa de pedestre
bool faixa_de_pedestre = false;

void setup() {
  // inicializacao dos sensores
  pinMode(sensor1_A1, INPUT);
  pinMode(sensor2_A2, INPUT);
  pinMode(sensor3_A3, INPUT);
  pinMode(sensor4_A4, INPUT);
  pinMode(sensor5_A5, INPUT);
  pinMode(sensor0_curva_A0, INPUT);
  pinMode(sensor6_curva_A6, INPUT);

  // garante que o carro começe parado
  parar();
  delay(2000);
  Serial.begin(9600);
}

/**
 * @brief Le os estados [0, 1] detectados pelo sensor.
 * 
 * Atualiza os arrays SENSOR & SENSOR_CURVA com os valores
 * atuais detectados pelo sensor
 */
void ler_sensores() {
  SENSOR[0] = digitalRead(sensor1_A1);
  SENSOR[1] = digitalRead(sensor2_A2);
  SENSOR[2] = digitalRead(sensor3_A3);
  SENSOR[3] = digitalRead(sensor4_A4);
  SENSOR[4] = digitalRead(sensor5_A5);

  SENSOR_CURVA[0] = digitalRead(sensor0_curva_A0);
  SENSOR_CURVA[1] = digitalRead(sensor6_curva_A6);
}

/**
 * @brief Ajusta o movimento do carro
 * 
 * Apartir do valor gerado pelo PID junto com a velocidade base dos motores
 * altera a direção do movimento para permanecer na linha
 */
void ajusta_movimento() {
  // altera o valor de velocidade
  velocidadeDireita = constrain(velocidadeBaseDireita - PID, 0, 255);
  velocidadeEsquerda = constrain(velocidadeBaseEsquerda + PID, 0, 255);
  
  // envia a nova velocidade para a função andar
  andar(velocidadeDireita, velocidadeEsquerda);
}

/**
 * @brief Calcula o erro do carro em relação a linha
 * 
 * Realiza uma media ponderada com os valores do sensor
 * gerando o erro do carro em relacao a linha.
 */
void calcula_erro() {
  // atualiza os valores do sensor
  ler_sensores();

  // verifica se houve inversao, sinalizando uma faixa de pedestre
  if (verifica_inversao(SENSOR, SENSOR_CURVA)) {
    faixa_de_pedestre = true;
  }

  // inicializa as variaveis para o calculo do erro
  int pesos[5] = {-2, -1, 0, 1, 2};
  int somatorioErro = 0;
  int sensoresAtivos = 0;

  // realiza um somatorio com os valores e o peso dos sensores
  for (int i = 0; i < 5; i++) {
    somatorioErro += SENSOR[i] * pesos[i];
    sensoresAtivos += SENSOR[i];
  }

  // determina o erro do carro
  if (sensoresAtivos == QUANTIDADE_TOTAL_SENSORES) {
    erro = LINHA_NAO_DETECTADA;
  } else {
    int sensoresInativos = QUANTIDADE_TOTAL_SENSORES - sensoresAtivos;
    erro = somatorioErro / sensoresInativos;
  }
}

/**
 * @brief Realiza o calculo do PID
 * 
 * A partir do erro e os valores das constantes,
 * Kp, Ki e Kd predefinidas realiza o calculo do
 * Controle Proporcional Derivativo (PID)
 */
void calcula_PID() {
  // inicializa as variaveis para o calculo
  PID = 0;
  P = erro;
  I = constrain(I + P, -255, 255);
  D = erro - erroAnterior;

  // calcula o PID
  PID = (Kp * P) + (Ki * I) + (Kd * D) + OFFSET;

  // atualiza o valor do PID
  erroAnterior = erro;
}

/**
 * @brief Imprime os dados do carro no monitor serial
 * 
 * Imprime os valores lido, PID, erro e a velocidade do carro
 */
void imprime_serial() {
  // imprime os valores do sensores
  Serial.print(SENSOR_CURVA[0]);
  Serial.print(" | ");

  for (int i = 0; i < 5; i++) {
    Serial.print(SENSOR[i]);
    Serial.print(" | ");
  }

  Serial.print(SENSOR_CURVA[1]);
  Serial.print(" | ");

  // imprime as variaveis Erro, PID, e velocidades
  Serial.print("\tErro: ");
  Serial.print(erro);
  Serial.print(" PID: ");
  Serial.print(PID);
  Serial.print(" Velocidade Direita: ");
  Serial.print(velocidadeDireita);
  Serial.print(" Velocidade Esquerda: ");
  Serial.println(velocidadeEsquerda);
}

/**
 * @brief Determina o lado da saída em uma rotatória baseado no número de marcações detectadas.
 * 
 * @param marcacoesEsquerda Armazena as marcacoes a esquerda
 * @param marcacoesDireita Armazena as marcacoes a direita
 * 
 * A partir do numero de marcações a função atualiza o lado correto para
 * a saida do carro na rotatoria.
 * 
 * @return int saidaDesejada 
 */
int determina_saida_rotatoria(int marcacoesEsquerda, int marcacoesDireita) {
  // inicializa com 0 a variavel
  int saidaDesejada = 0;

  // verifica qual lado deve ser feito a saida
  if (marcacoesEsquerda > marcacoesDireita) {
    // calcula a saida apartir do numero de marcacaoes por detecacao no intervalo de tempo + OFFSET(1)
    saidaDesejada = (marcacoesEsquerda / DETECCAO_POR_QUADRADO) + 1;
    // realiza o movimento para entrar na rotatoria
    curva_esquerda(velocidadeBaseDireita, velocidadeBaseEsquerda);
    // atualiza o valor da saida
    saida_rotatoria = SAIDA_ESQUERDA;
  } else if(marcacoesEsquerda < marcacoesDireita) {
    // calcula a saida apartir do numero de marcacaoes por detecacao no intervalo de tempo + OFFSET(1)
    saidaDesejada = (marcacoesDireita / DETECCAO_POR_QUADRADO) + 1; 
    // realiza o movimento para entrar na rotatoria
    curva_direita(velocidadeBaseDireita, velocidadeBaseEsquerda);
    // atualiza o valor da saida
    saida_rotatoria = SAIDA_DIREITA;
  }
  
  //retorna o valor de saida
  return saidaDesejada;
}

/**
 * @brief Realiza a saida na rotatoria
 * 
 * @param saidaDesejada Numero que deve ser feito a saida da rotatoria
 * 
 * O carro segue o percurso ate chegar na `saidaDesejada`
 */
void realiza_rotatoria(int saidaDesejada){
  // inicializa a saida atual
  int saidaAtual = 1;

  // loop para que o carro sai apenas na saida correta
  while(saidaAtual != saidaDesejada) {
    // calcula o erro para manter o carro na linha
    calcula_erro();
    ajusta_movimento();

    // verifica qual lado deve ser feito a saida
    if (saida_rotatoria == SAIDA_ESQUERDA) {
      // verifica se ha alguma marcacao na direita
      if (calcula_sensores_ativos(SENSOR) <= 3 && SENSOR_CURVA[0] == PRETO && SENSOR_CURVA[1] == BRANCO) {
        delay(200);
        // atualiza o valor da saida atual
        saidaAtual++;
      }
    } else if(saida_rotatoria == SAIDA_DIREITA) {
      // verifica se ha alguma marcacao na esquerda
      if (calcula_sensores_ativos(SENSOR) <= 3 && SENSOR_CURVA[0] == BRANCO && SENSOR_CURVA[1] == PRETO) {
        delay(200);
        // atualiza o valor da saida atual
        saidaAtual++;
      }
    }
  }
  
  // sai para o lado correto da rotatoria
  if (saida_rotatoria == SAIDA_ESQUERDA) {
    curva_esquerda(velocidadeBaseDireita, velocidadeBaseEsquerda);
  } else {
    curva_direita(velocidadeBaseDireita, velocidadeBaseEsquerda);
  }
}

/**
 * @brief Realiza a marcha re
 * 
 * @param int historico
 * 
 * A partir do historico o robo segue o trajeto dando marcha re e
 * ao chegar ao determinado local ele vira para o lado indicado
 */
void realiza_marcha_re(int historico[]) {
  // atualiza o valor de historico
  for (int i = 0; i < DETECCAO_POR_QUADRADO; i++) {
    if (i > 0) {
      historico[i] == historico[i - 1];
    } 
  }

  // realiza a re ate chegar no local maximo
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
  // iniciliza as variaveis
  int marcacoesDireita = 0, marcacoesEsquerda = 0;
  // calcula o erro do carro sobre a linha
  calcula_erro();

  // verifica se ha uma curva de 90
  int saidaCurva = verifica_curva_90(SENSOR, SENSOR_CURVA);
  int historico_curva[4 * DETECCAO_POR_QUADRADO];
  int i = 0;

  // verifica se a curva foi detectada
  if (saidaCurva != CURVA_NAO_ENCONTRADA) { 
    // caso tenha curva armazena a quantidade de marcaçoes
    while(erro != LINHA_NAO_DETECTADA) {
      // atualiza o valor dos sensores
      ler_sensores();
      
      // atualiza os valores de identificacao
      if (SENSOR_CURVA[0] == BRANCO) {
        marcacoesEsquerda++;
        historico_curva[i] = SAIDA_ESQUERDA;
      } else if (SENSOR_CURVA[1] == BRANCO) {
        marcacoesDireita++;
        historico_curva[i] = SAIDA_DIREITA;
      }

      i++;
      // garantem que o robo continue na linha
      calcula_erro();
      calcula_PID();
      ajusta_movimento();
    }
    
    // determina qual acao deve ser feita
    if (marcacoesEsquerda == DETECCAO_POR_QUADRADO || marcacoesDireita == DETECCAO_POR_QUADRADO) {
      realiza_curva_90(saidaCurva);
    } else if ((marcacoesEsquerda / DETECCAO_POR_QUADRADO) > 1 && (marcacoesDireita / DETECCAO_POR_QUADRADO) > 1) {
      realiza_marcha_re(historico_curva);
    } else{
      determina_saida_rotatoria(marcacoesEsquerda, marcacoesDireita);
    }

    parar();
  } else {
    // caso nao detectado curva e tenha perdido a linha
    if (erro == LINHA_NAO_DETECTADA) {
      // atualiza o numero de sensores ativos
      int sensoresAtivos = calcula_sensores_ativos(SENSOR);
      PID = 0;
      parar();

      // verifica se é uma faixa de pedestre
      if (faixa_de_pedestre == true) {
        realiza_faixa_de_pedestre();
        faixa_de_pedestre = false;
      } else {  
        delay(10000);

        // realiza uma re ate que volte para a linha
        while(sensoresAtivos == QUANTIDADE_TOTAL_SENSORES) {
          ler_sensores();
          sensoresAtivos = calcula_sensores_ativos(SENSOR);
          andar_de_re(255, 255);
        }

        parar();
      }

    } else {
      // caso o carro detecte a linha ele segue a linha
      calcula_PID();
      ajusta_movimento();
    }
  }

  // obtem a saida dos dados do carro
  imprime_serial(); 
  delay(5);
}
