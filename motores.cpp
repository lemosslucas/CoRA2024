#include "motores.h"

// define a porta dos motores
const int MOTOR_ESQUERDO_HORARIO = 3;
const int MOTOR_ESQUERDO_ANTI = 4;
const int MOTOR_DIREITO_HORARIO = 7;
const int MOTOR_DIREITO_ANTI = 8;

const int MOTOR_PWM_ESQUERDO = 5;
const int MOTOR_PWM_DIREITO = 6;

#define TEMPO_CURVA 100

void setup_motor() {
  pinMode(MOTOR_ESQUERDO_HORARIO, OUTPUT);
  pinMode(MOTOR_ESQUERDO_ANTI, OUTPUT);
  pinMode(MOTOR_DIREITO_HORARIO, OUTPUT);
  pinMode(MOTOR_DIREITO_ANTI, OUTPUT);

  pinMode(MOTOR_PWM_ESQUERDO, OUTPUT);
  pinMode(MOTOR_PWM_DIREITO, OUTPUT);
}

void andar(int velocidadeDireita, int velocidadeEsquerda) {
  digitalWrite(MOTOR_ESQUERDO_ANTI, LOW);
  digitalWrite(MOTOR_ESQUERDO_HORARIO, HIGH);
  digitalWrite(MOTOR_DIREITO_HORARIO, HIGH);
  digitalWrite(MOTOR_DIREITO_ANTI, LOW);

  // controla a velocidade
  analogWrite(MOTOR_PWM_ESQUERDO, velocidadeEsquerda);
  analogWrite(MOTOR_PWM_DIREITO, velocidadeDireita);
}

void parar() {
  digitalWrite(MOTOR_ESQUERDO_ANTI, LOW);
  digitalWrite(MOTOR_ESQUERDO_HORARIO, LOW);
  digitalWrite(MOTOR_DIREITO_HORARIO, LOW);
  digitalWrite(MOTOR_DIREITO_ANTI, LOW);
}

void andar_de_re(int velocidadeDireita, int velocidadeEsquerda) {
  digitalWrite(MOTOR_ESQUERDO_ANTI, HIGH);
  digitalWrite(MOTOR_ESQUERDO_HORARIO, LOW);
  digitalWrite(MOTOR_DIREITO_HORARIO, LOW);
  digitalWrite(MOTOR_DIREITO_ANTI, HIGH);

  // controla a velocidade 
  analogWrite(MOTOR_PWM_ESQUERDO, velocidadeEsquerda);
  analogWrite(MOTOR_PWM_DIREITO, velocidadeDireita);
}

// funcao para a curva de 90 a direita
void curva_direita(int velocidadeDireita, int velocidadeEsquerda) {
  digitalWrite(MOTOR_ESQUERDO_ANTI, LOW);
  digitalWrite(MOTOR_ESQUERDO_HORARIO, HIGH);
  digitalWrite(MOTOR_DIREITO_HORARIO, LOW);
  digitalWrite(MOTOR_DIREITO_ANTI, HIGH);
  delay(TEMPO_CURVA);
  
  analogWrite(MOTOR_PWM_ESQUERDO, velocidadeEsquerda);
  analogWrite(MOTOR_PWM_DIREITO, velocidadeDireita);
}

//funcao para a curva de 90 a esquerda
void curva_esquerda(int velocidadeDireita, int velocidadeEsquerda) {

  digitalWrite(MOTOR_ESQUERDO_ANTI, HIGH);
  digitalWrite(MOTOR_ESQUERDO_HORARIO, LOW);
  digitalWrite(MOTOR_DIREITO_HORARIO, HIGH);
  digitalWrite(MOTOR_DIREITO_ANTI, LOW);

  analogWrite(MOTOR_PWM_ESQUERDO, velocidadeEsquerda);
  analogWrite(MOTOR_PWM_DIREITO, velocidadeDireita);
}
