from serial import Serial, readline
import os

porta_serial = 'COM5'
saida_serial = Serial(porta_serial, 9600)

pasta = os.getcwd()
local_arquivo = os.path.join(pasta, 'dados.txt')

with open(local_arquivo, 'w') as arquivo:
    while True:
        if saida_serial.in_waiting > 0:
            line = saida_serial.readline().decode('utf-8').strip()
            arquivo.write(line + '\n')
