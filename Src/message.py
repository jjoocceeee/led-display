import serial

ser = serial.Serial()
ser.baudrate = 115200
ser.port = "COM3"
ser.open()

while(1):
    print(ser.readline())
    i = input().encode()
    ser.write(i)
    print(ser.readline())
