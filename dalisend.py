#!/usr/bin/env python

import serial
import sys
import time

device = sys.argv[1]

ser = serial.Serial(device, baudrate=115200, timeout=1)

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

while True:
    line = sys.stdin.readline()
    if line is None:
        break
    a, b = line.strip().split()
    a = int(a, base=16)
    b = int(b, base=16)
    msg([a << 1 | 1, b])
