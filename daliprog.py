#!/usr/bin/env python

import serial
import sys
import time

device = sys.argv[1]
address = int(sys.argv[2])

ser = serial.Serial(device, baudrate=115200, timeout=1)

def msg(data):
    # packet starts with the number of message bytes
    buf = [len(data)]
    
    # then the message
    buf.extend(data)
        
    # add a null in case we get out of sync
    # (a null on its own will be interpreted as a zero-length message)
    buf.append(0)

    # send the data over serial
    ser.write(buf)
    ser.flush()

msg([0xFF, 32]) # Broadcast, reset
msg([0xFF, 32])
msg([0b10100101, 0]) # Special command 258, initialise
msg([0b10100101, 0])
msg([0b10110111, address << 1 | 1]) # Special command 267, program short address

# test (high-low-high-low)
msg([address << 1, 254])
time.sleep(0.5)
msg([address << 1, 1])
time.sleep(0.5)
msg([address << 1, 254])
time.sleep(0.5)
msg([address << 1, 1])
