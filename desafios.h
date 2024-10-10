#ifndef DESAFIOS_H
#define DESAFIOS_H

#include <Arduino.h>

int verifica_curva_90(int SENSOR[], int SENSOR_CURVA[]);
void realiza_curva_90(int curvaEncontrada);
int inverte_sensor(int SENSOR);
void verifica_inversao(int SENSOR[]);
int calcula_sensores_ativos(int SENSOR[]);
void volta_inatividade(int velocidadeDireita, int velocidadeEsquerda);

#endif
