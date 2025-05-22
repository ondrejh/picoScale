#!/usr/bin/python3

import socket

HOST = "127.0.0.1"
PORT = 12345
BUFFER_SIZE = 1024

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
  sock.connect((HOST, PORT))
  print(f"Connected to server {HOST}:{PORT}")

  msg = "Ahoj vole\n"
  sock.sendall(msg.encode())
  print(f"Send: {msg}")

  data = sock.recv(BUFFER_SIZE)
  print(f"Recv: {msg}")
