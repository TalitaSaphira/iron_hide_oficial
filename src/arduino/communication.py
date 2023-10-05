#! /usr/bin/env python3
#Primeiro deve-se receber os dados do arduino:
#   - imu mpu 6050
#   - encoders AS5600
#Depois vamos enviar informações pro arduino controlar as rodas:
#   - drivers A4988

#Implementar publishers e subscriber para a manipulação dos dados

import serial
import time

#import rospy

# Configuração da porta serial (substitua '/dev/ttyS0' pelo seu dispositivo serial)
ser = serial.Serial('/dev/ttyUSB0', 115200, timeout=1)
time.sleep(3)
ser.reset_input_buffer()

while ser.isOpen() == False:
    print('Waiting')

print("Serial OK")

try:
    while True:
        
#         data = ser.read(4 * 4)  # 4 floats para quaternions (4 bytes cada)
#         quaternion = struct.unpack('ffff', data)
# 
#         data = ser.read(9 * 4)  # 9 floats para covariância (9 bytes cada)
#         covariance = struct.unpack('fffffffff', data)
# 
#         # Faça algo com os dados (imprima, armazene, etc.)
#         print("Quaternion:", quaternion)
#         print("Covariance:", covariance)

        #velocidade das rodas em PWM - vão ser recebidas da odometria por um subscriver 
        velLB = 3.14159
        velRB = 2.71828
        velLF = 1.41421
        velRF = 1.41421

        # Formate os valores em uma string com delimitadores (por exemplo, vírgula)
        dados = f"{velLB},{velLF},{velRB},{velRF}\n"

        # Envie a string pela porta serial
        ser.write(dados.encode())

except KeyboardInterrupt:
    print ('Close Serial communication')
    ser.close()
