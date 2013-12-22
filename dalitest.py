#!/usr/bin/env python

import serial
import sys
import time

ser = serial.Serial('/dev/tty.usbmodem1411', baudrate=115200, timeout=1)

units = [0, 1, 2]

def msg(data):
    # packet starts with the number of message bytes
    buf = chr(len(data))
    
    # then the message
    for d in data:
        buf += chr(d)
        
    # add a null in case we get out of sync
    # (a null on its own will be interpreted as a zero-length message)
    buf += chr(0)

    # send the data over serial
    ser.write(buf)
    ser.flush()

def chase(t=0.25):
    while True:
        for i in range(0, len(units)):
            a = units[i]
            b = units[i % len(units)-1]
            msg([a << 1, 254, b << 1, 1])
            time.sleep(t)

try:
    chase()
except KeyboardInterrupt:
    for unit in units:
        msg([unit << 1, 254])
        