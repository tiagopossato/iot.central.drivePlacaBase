import socket
import sys

# Cria o socket
sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)

# Conecta o socket no arquivo
server_address = '/tmp/placaBase.socket'
try:
    sock.connect(server_address)
except socket.error as msg:
    print(msg)
    sys.exit(1)

try:
    # Send data
    message = '[3/3202/3303/]'
    enviados = sock.send(bytes(message, 'UTF-8'))
    print("Bytes enviados: "+ str(enviados))
finally:
    print('Fechando conexão')
    sock.close()