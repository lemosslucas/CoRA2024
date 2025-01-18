#include "motores.h"

// define a porta dos motores
const int MOTOR_ESQUERDO_HORARIO = 3;
const int MOTOR_ESQUERDO_ANTI = 4;
const int MOTOR_DIREITO_HORARIO = 7;
const int MOTOR_DIREITO_ANTI = 8;

const int MOTOR_PWM_ESQUERDO = 5;
const int MOTOR_PWM_DIREITO = 6;

void setup_motor() {
  pinMode(MOTOR_ESQUERDO_HORARIO, OUTPUT);
  pinMode(MOTOR_ESQUERDO_ANTI, OUTPUT);
  pinMode(MOTOR_DIREITO_HORARIO, OUTPUT);
  pinMode(MOTOR_DIREITO_ANTI, OUTPUT);

  pinMode(MOTOR_PWM_ESQUERDO, OUTPUT);
  pinMode(MOTOR_PWM_DIREITO, OUTPUT);
}

/**
 * @brief Move o veiculo para frente com as velocidades especificadas.
 * 
 * Configura os motores para se moverem para frente e ajusta suas velocidades
 * individualmente usando PWM.
 * 
 * @param velocidadeDireita Velocidade do motor direito (0 a 255).
 * @param velocidadeEsquerda Velocidade do motor esquerdo (0 a 255).
 */
void andar(int velocidadeDireita, int velocidadeEsquerda) {
  digitalWrite(MOTOR_ESQUERDO_ANTI, LOW);
  digitalWrite(MOTOR_ESQUERDO_HORARIO, HIGH);
  digitalWrite(MOTOR_DIREITO_HORARIO, HIGH);
  digitalWrite(MOTOR_DIREITO_ANTI, LOW);

  analogWrite(MOTOR_PWM_ESQUERDO, velocidadeEsquerda);
  analogWrite(MOTOR_PWM_DIREITO, velocidadeDireita);
}

/**
 * @brief Para todos os motores, interrompendo o movimento do veiculo.
 */
void parar() {
  digitalWrite(MOTOR_ESQUERDO_ANTI, LOW);
  digitalWrite(MOTOR_ESQUERDO_HORARIO, LOW);
  digitalWrite(MOTOR_DIREITO_HORARIO, LOW);
  digitalWrite(MOTOR_DIREITO_ANTI, LOW);
}

/**
 * @brief Move o veiculo para tras com as velocidades especificadas.
 * 
 * Configura os motores para se moverem para tras e ajusta suas velocidades
 * individualmente usando PWM.
 * 
 * @param velocidadeDireita Velocidade do motor direito (0 a 255).
 * @param velocidadeEsquerda Velocidade do motor esquerdo (0 a 255).
 */
void andar_de_re(int velocidadeDireita, int velocidadeEsquerda) {
  digitalWrite(MOTOR_ESQUERDO_ANTI, HIGH);
  digitalWrite(MOTOR_ESQUERDO_HORARIO, LOW);
  digitalWrite(MOTOR_DIREITO_HORARIO, LOW);
  digitalWrite(MOTOR_DIREITO_ANTI, HIGH);

  analogWrite(MOTOR_PWM_ESQUERDO, velocidadeEsquerda);
  analogWrite(MOTOR_PWM_DIREITO, velocidadeDireita);
}

/**
 * @brief Executa uma curva de 90 graus para a direita.
 * 
 * Configura os motores para girar de modo a realizar uma curva para a direita.
 * A funcao ajusta a velocidade do motor esquerdo e usa um `delay` para controlar
 * o tempo da curva.
 * 
 * @param velocidadeDireita Velocidade do motor direito (0 a 255).
 * @param velocidadeEsquerda Velocidade do motor esquerdo (0 a 255).
 * 
 * @note O `delay(600)` determina a duracao da curva; ajuste conforme necessario.
 */
void curva_direita(int velocidadeDireita, int velocidadeEsquerda) {
  digitalWrite(MOTOR_ESQUERDO_ANTI, HIGH);
  digitalWrite(MOTOR_ESQUERDO_HORARIO, LOW);
  digitalWrite(MOTOR_DIREITO_HORARIO, HIGH);
  digitalWrite(MOTOR_DIREITO_ANTI, LOW);
  
  analogWrite(MOTOR_PWM_ESQUERDO, velocidadeEsquerda + 40);
  analogWrite(MOTOR_PWM_DIREITO, velocidadeDireita);
  delay(600);
}

/**
 * @brief Executa uma curva de 90 graus para a esquerda.
 * 
 * Configura os motores para girar de modo a realizar uma curva para a esquerda.
 * A funcao ajusta a velocidade do motor direito e usa um `delay` para controlar
 * o tempo da curva.
 * 
 * @param velocidadeDireita Velocidade do motor direito (0 a 255).
 * @param velocidadeEsquerda Velocidade do motor esquerdo (0 a 255).
 * 
 * @note O `delay(600)` determina a duracao da curva; ajuste conforme necessario.
 */
void curva_esquerda(int velocidadeDireita, int velocidadeEsquerda) {
  digitalWrite(MOTOR_ESQUERDO_ANTI, LOW);
  digitalWrite(MOTOR_ESQUERDO_HORARIO, HIGH);
  digitalWrite(MOTOR_DIREITO_HORARIO, LOW);
  digitalWrite(MOTOR_DIREITO_ANTI, HIGH);

  analogWrite(MOTOR_PWM_ESQUERDO, velocidadeEsquerda);
  analogWrite(MOTOR_PWM_DIREITO, velocidadeDireita + 40);
  delay(600);
}
