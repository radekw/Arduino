#!/usr/bin/python


import serial, time

#ser = serial.Serial('/dev/tty.usbmodem411', 9600);
ser = serial.Serial('/dev/tty.usbserial-A700eCvT', 9600);

time.sleep(1);
cmd = [250, 20, 100, 0, 0, 251]
sent = ser.write(bytearray(cmd))
print sent

time.sleep(3);
cmd = [250, 30, 0, 100, 0, 251]
sent = ser.write(bytearray(cmd))
print sent

time.sleep(4);
cmd = [250, 30, 0, 0, 100, 251]
sent = ser.write(bytearray(cmd))
print sent

time.sleep(4)
cmd = [250, 20, 100, 100, 100, 251]
sent = ser.write(bytearray(cmd))
print sent

