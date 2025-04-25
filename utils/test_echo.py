from serial import Serial

with Serial('/dev/ttyACM0', timeout=0.1) as port:
    port.write(b'ahoj')
    print(port.readall())
