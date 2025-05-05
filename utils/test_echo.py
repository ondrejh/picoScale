#!/usr/bin/python3

from serial import Serial


test_string = b'{"name":"Alice","age":30.123}\n'
with Serial('/dev/ttyACM0', timeout=0.1) as port:
    #port.write(b'ahoj\n')
    port.write(test_string)
    print(port.readall())
