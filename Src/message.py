import serial
import PySimpleGUI as sg

ser = serial.Serial()
ser.baudrate = 115200
ser.port = "COM3"
ser.open()

sg.theme('DarkAmber')   # Add a touch of color
layout = [[sg.Slider(range=(0,255),
         default_value=150,
         key="RED_SLIDER",
         enable_events=True,
         size=(20,15),
         orientation='horizontal',
         font=('Helvetica', 12))]]

window = sg.Window('RGB Control', layout)
# while True:
#     event, values = window.read()
#     if event in (None, 'Cancel'):   # if user closes window or clicks cancel
#         break
#     print('You entered ', values)

#     ser.write(b'r')
#     print(ser.readline())
#     ser.write(str(int(values["RED_SLIDER"])).encode())
#     print(ser.readline())
# print(ser.readline())
#     print(ser.readline())


while(1):
    print(ser.readline())
    i = input().encode()
    ser.write(i)
    print(ser.readline())
