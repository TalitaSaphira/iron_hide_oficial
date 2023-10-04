import serial
import time

# Precisa instalar pyserial

ser = serial.Serial('COM3', 9600)  # Talvez o nome /dev/ttyACM0 seja diferente pra esse arduino
ser.reset_input_buffer()
time.sleep(1)
while ser.isOpen() == False:
    print('Waiting')

# Se x não for uma lista, ele será convertido em uma lista. Caso seja, será retornado ele mesmo
def asList(x):
    if type(x) is list:
        return x
    else:
        return [x]


# Se x não for uma lista, ele será convertido em uma lista. Caso seja, será retornado ele mesmo
def asListOfLists(x):
    if type(x) is list and type(x[0]) is list:
        return x
    else:
        return [x]


# Recebe dados dos sensores infravermelhos.
# O input "numeros" serão os IDs do sensor infravermelho (1,2,3 ou 4) que você quer ler. Pode ser uma lista ex: [1,2] ou um inteiro ex:3.
# O valor retornado será uma lista dos dados, independente se foi requisitado os dados de um ou mais sensores.
def receiveInfrared(numeros):
    ret = []
    listnums = asList(numeros)
    for num in listnums:
        comando = 1 + num
        ser.write(chr(comando).encode('raw_unicode_escape'))
        while True:
            dado = ser.read()
            if dado != b'':
                ret.append(dado)
                break
    return ret


# Recebe dados dos encoders.
# O input "numeros" serão os IDs do encoder (1,2,3,4 ou 5, com 5 sendo do motor do manipulador) que você quer ler. Pode ser uma lista ex: [1,2] ou um inteiro ex:3.
# O valor retornado será uma lista das posições angulares dos encoders, independente se foi requisitado os dados de um ou mais encoders.
def receiveEncoder(numeros):
    ret = []
    listnums = asList(numeros)
    for num in listnums:
        comando = 4 + num
        ser.write(chr(comando).encode('raw_unicode_escape'))
        while True:
            dado = ser.read()
            if dado != b'':
                ret.append(dado)
                break
    return ret


# Recebe dados da chave de fim de curso
# O valor retornado será um inteiro com o valor do sensor
def receiveFDC():
    ser.write(chr(10).encode('raw_unicode_escape'))
    while True:
        dado = ser.read()
        if dado != b'':
            return dado


# Faz o robô andar
# O input "comando" define o tipo de direção que você quer mover o robô
def actWheels(comando, angulo):
    ser.write(chr(11).encode('raw_unicode_escape'))
    while True:
        dado = ser.read()
        if dado != b'':
            ser.write(chr(comando).encode('raw_unicode_escape'))
            break
    while True:
        dado = ser.read()
        if dado != b'':
            ser.write(chr(angulo).encode('raw_unicode_escape'))
            break
    return


# Envia a posição do motor de passo do manipulador
# O input "pos" é a posição em graus do motor de 0 a 255
def actManip(pos):
    ser.write(chr(12).encode('raw_unicode_escape'))
    while True:
        dado = ser.read()
        if dado != b'':
            ser.write(chr(pos).encode('raw_unicode_escape'))
            break
    return


# Envia a posição do motor servo da garra
# O input "pos" é a posição em graus do motor, de 0 a 255
def actClaw(pos):
    ser.write(chr(13).encode('raw_unicode_escape'))
    while True:
        dado = ser.read()
        if dado == b'\x01':
            ser.write(chr(pos).encode('raw_unicode_escape'))
            break
    return

# Recebe a posição do robô
# Retorna a
def getPosRobot():
    ret = ()
    ser.write(chr(14).encode('raw_unicode_escape'))
    while True:
        dado = ser.read()
        print(dado)
        if dado == b'\x01':
            posx = int.from_bytes(ser.read(), "little")
            posx = posx + 0.01 * int.from_bytes(ser.read(), "little")
            posy = int.from_bytes(ser.read(), "little")
            posy = posy + 0.01 * int.from_bytes(ser.read(), "little")
            posz = int.from_bytes(ser.read(), "little")
            posz = posz + 0.01 * int.from_bytes(ser.read(), "little")
            posrot = int.from_bytes(ser.read(), "little")
            posrot = posrot + 0.01 * int.from_bytes(ser.read(), "little")
            ret = (posx, posy, posz, posrot)
            break
    return ret