#ifndef DESAFIOS_H
#define DESAFIOS_H

#include <Arduino.h>

int verifica_curva_90(int SENSOR[], int SENSOR_CURVA[]);
void realiza_curva_90(int curvaEncontrada);
int inverte_sensor(int SENSOR);
bool verifica_inversao(int SENSOR[], int SENSOR_CURVA[]);
int calcula_sensores_ativos(int SENSOR[]);
void realiza_faixa_de_pedestre();

#endif
