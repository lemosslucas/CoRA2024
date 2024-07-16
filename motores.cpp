#include "motores.h"

// define a porta dos motores
const int MOTOR_ESQUERDO_HORARIO = 5;
const int MOTOR_ESQUERDO_ANTI = 6;
const int MOTOR_DIREITO_HORARIO = 7;
const int MOTOR_DIREITO_ANTI = 9;

const int MOTOR_PWM_ESQUERDO = 10;
const int MOTOR_PWM_DIREITO = 11;

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
  analogWrite(MOTOR_PWM_ESQUERDO, velocidadeDireita);
  analogWrite(MOTOR_PWM_DIREITO, velocidadeEsquerda);
}

void parar(int velocidadeDireita, int velocidadeEsquerda) {
  digitalWrite(MOTOR_ESQUERDO_ANTI, LOW);
  digitalWrite(MOTOR_ESQUERDO_HORARIO, LOW);
  digitalWrite(MOTOR_DIREITO_HORARIO, LOW);
  digitalWrite(MOTOR_DIREITO_ANTI, LOW);
  
  // controla a velocidade
  analogWrite(MOTOR_PWM_ESQUERDO, velocidadeDireita);
  analogWrite(MOTOR_PWM_DIREITO, velocidadeEsquerda);
}

void andar_de_re(int velocidadeDireita, int velocidadeEsquerda) {
  digitalWrite(MOTOR_ESQUERDO_ANTI, HIGH);
  digitalWrite(MOTOR_ESQUERDO_HORARIO, LOW);
  digitalWrite(MOTOR_DIREITO_HORARIO, LOW);
  digitalWrite(MOTOR_DIREITO_ANTI, HIGH);

  // controla a velocidade 
  analogWrite(MOTOR_PWM_ESQUERDO, velocidadeDireita);
  analogWrite(MOTOR_PWM_DIREITO, velocidadeEsquerda);
}


// funcao para a curva de 90 a direita
void curva_direita() {
  
}

//funcao para a curva de 90 a esquerda
void curva_esquerda() {
  
}

