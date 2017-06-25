#!/usr/bin/env python3

import subprocess
continuar = True
while(continuar):
    try:
        retornoDrive = subprocess.call("/opt/iot.central/drivePlacaBase.bin")
        print("Resposta do processo: " + str(retornoDrive))
        if(retornoDrive == -11):#falha de segmentação
            continuar = True
        else:
            continuar = False
    except KeyboardInterrupt as e:
        print('Encerrando aplicacao')
        continuar = False

    except Exception as e:
        print('Encerrando aplicacao')
        continuar = False
        print(e)
