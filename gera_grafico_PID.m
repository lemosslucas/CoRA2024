close all; clear all;

dados = load('dados.txt', '-ascii', '\t');

erro = dados(:, 1)
PID = dados(:, 2);
tempo = dados(:, 3);

tempo = tempo ./ 1000;

figure 1
hold on; grid on;
plot(tempo, erro);
plot(tempo, erro, 'x', 'markersize', 10)
title('tempo x erro')
xlabel('tempo (S)')
ylabel('erro')
axis([0, max(tempo), -3, 3]);

figure 2
hold on; grid on;
plot(tempo, PID);
title('tempo x PID')
xlabel('tempo (S)')
ylabel('PID')
axis([0, max(tempo), -max(PID) - 5, max(PID) + 5]);
