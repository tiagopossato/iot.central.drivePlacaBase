#!/bin/bash

if [ "$(id -u)" != "0" ]; then
	echo "Este script precisa ser executado com permissões de super-usuário (root)!"
	exit 1
fi

cp drivePlacaBase.bin /opt/optativa/drivePlacaBase.bin
cp src/config /opt/optativa/config
cp servico/drivePlacaBase.py /opt/optativa/drivePlacaBase.py
cp servico/drivePlacaBase.conf /etc/supervisor/conf.d/drivePlacaBase.conf

supervisorctl reload