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
ser = serial.Serial('/dev/ttyS0', 9600, timeout=1)
time.sleep(3)
ser.reset_input_buffer()

while ser.isOpen() == False:
    print('Waiting')

print("Serial OK")

try:
    while True:

        # Lê os dados da MPU-6050 do Arduino via comunicação serial
        imu_data = ser.readline().decode().strip()

        if imu_data:
            #print("Dados da MPU-6050:", imu_data)
            print("mpu 6050 w: ", imu_data.w)
            print("mpu 6050 x: ", imu_data.x)
            print("mpu 6050 y: ", imu_data.y)
            print("mpu 6050 z: ", imu_data.z)
        
        #calculo da covariância
        dados_x = imu_data.accelX
        dados_y = imu_data.accelY

        # Calcular a média dos dados
        media_x = sum(dados_x) / len(dados_x)
        media_y = sum(dados_y) / len(dados_y)

        # Calcular a covariância
        covariancia = sum((x - media_x) * (y - media_y) for x, y in zip(dados_x, dados_y)) / len(dados_x)
        print("Covariância:", covariancia)
        
        # Lê os dados dos encoders AS5600 do Arduino via comunicação serial


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
