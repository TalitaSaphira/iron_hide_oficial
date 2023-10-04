import serial
import time

#=========================================================
ser = serial.Serial('/dev/ttyACM0', 115200, timeout=1.0)
time.sleep(3)
ser.reset_input_buffer()
print("Serial OK")
#=========================================================

try:
    while True:
        time.sleep(0.1)
#        if ser.in_wating > 0 :
#            line = ser.readline().decode('utf-8')
#            print(line) 
        print("Send message to arduino")
        ser.write("Hello from Raspberry Pi\n".encode('utf-8'))
except KeyboardInterrupt:
    print ('Close Serial communication')
    ser.close()