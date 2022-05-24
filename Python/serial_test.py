import serial
from time import sleep

ser = serial.Serial(port='COM4',baudrate=9600,parity=serial.PARITY_NONE,stopbits=serial.STOPBITS_ONE,bytesize=serial.EIGHTBITS,timeout=1)
print("connected to: " + ser.portstr)
ser.write(b'a')
sleep(1)
ser.close()