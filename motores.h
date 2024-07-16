#ifndef MOTORES_H
#define MOTORES_H

#include <Arduino.h>

void setup_motor();
void andar(int velocidadeDireita, int velocidadeEsquerda);
void parar(int velocidadeDireita, int velocidadeEsquerda);
void andar_de_re(int velocidadeDireita, int velocidadeEsquerda);

#endif
