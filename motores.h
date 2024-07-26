#ifndef MOTORES_H
#define MOTORES_H

#include <Arduino.h>

void setup_motor();
void andar(int velocidadeDireita, int velocidadeEsquerda);
void curva_esquerda(int velocidadeDireita, int velocidadeEsquerda);
void curva_direita(int velocidadeDireita, int velocidadeEsquerda);
void parar();
void andar_de_re(int velocidadeDireita, int velocidadeEsquerda);

#endif
