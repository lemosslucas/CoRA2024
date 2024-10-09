close all; clear all;

dados = load('dados.txt', '-ascii', '\t');

Kp = input("Qual o valor de Kp? ");

erro = dados(:, 1);
PID = dados(:, 2);
tempo = dados(:, 3);

tempo = tempo ./ 1000;

figure 1
hold on; grid on;
plot(tempo, erro);
plot(tempo, erro, '.', 'color', 'k', 'markersize', 10)
title('tempo x erro')
xlabel('tempo (S)')
ylabel('erro')
axis([0, max(tempo), -5, 5]);
print('-djpg', sprintf('kp%.2f.jpg', Kp));

figure 2
hold on; grid on;
plot(tempo, PID);
title('tempo x PID')
xlabel('tempo (S)')
ylabel('PID')
axis([0, max(tempo), -max(PID) - 5, max(PID) + 5]);
