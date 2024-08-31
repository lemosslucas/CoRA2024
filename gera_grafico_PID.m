dados = load('dados.txt', '\t');

erro = dados(:, 1);
PID = dados(:, 2);
tempo = dados(:, 3);

figure 1
plot(tempo, erro, '.');
title('tempo x erro')
xlabel('tempo')
ylabel('erro')

figure 2
plot(tempo, PID, '.');
title('tempo x PID')
xlabel('tempo')
ylabel('PID')
