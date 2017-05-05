import socket
import sys
from time import sleep
import random

# Conecta o socket no arquivo
server_address = '/tmp/placaBase.socket'
while(True):

	# Cria o socket
	sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
	try:
		sock.connect(server_address)
	except socket.error as msg:
		print(msg)
		#sys.exit(1)

	try:
		# Send data
		message = '['+str(random.randint(3,6))+'/3202/330'+str(random.randint(3,4))+'/]'
		print(message)
		enviados = sock.send(bytes(message, 'UTF-8'))
		print("Bytes enviados: "+ str(enviados))
	finally:
		print('Fechando conexão')
		sock.close()
	sleep(.3)
