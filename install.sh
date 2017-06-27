#!/bin/bash

if [ "$(id -u)" != "0" ]; then
	echo "Este script precisa ser executado com permissões de super-usuário (root)!"
	exit 1
fi

supervisorctl stop drivePlacaBase
sudo rm -f /opt/iot.central/drivePlacaBase.bin

cp drivePlacaBase.bin /opt/iot.central/drivePlacaBase.bin
cp src/driveConfig /opt/iot.central/driveConfig
cp servico/drivePlacaBase.py /opt/iot.central/drivePlacaBase.py
cp servico/drivePlacaBase.conf /etc/supervisor/conf.d/drivePlacaBase.conf

supervisorctl reload drivePlacaBase
