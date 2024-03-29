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
		message = [
			'['+str(random.randint(1,6))+'/3202/330'+str(random.randint(3,4))+']',
			'['+str(random.randint(1,6))+'/3202/3320]',
			'['+str(random.randint(1,6))+'/3201/'+str(random.randint(0,7))+'/'+str(random.randint(0,1))+']'
		]
		saida = message[random.randint(0,2)]
		print(saida)
		enviados = sock.send(bytes(saida, 'UTF-8'))
		#enviados = sock.send(bytes(message[0], 'UTF-8'))
		print("Bytes enviados: "+ str(enviados))
	except OSError as e:
		print("Erro: " + str(e))
	finally:
		print('Fechando conexão')
		sock.close()
	sleep(.01)
