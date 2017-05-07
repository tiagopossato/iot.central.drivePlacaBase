rm drivePlacaBase.bin

gcc -c ../lib/filaEntrada.c -o filaEntrada.o
gcc -c ../lib/filaSaida.c -o filaSaida.o
gcc -c ../lib/definicoes.c -o definicoes.o
gcc -c ../lib/util.c -o util.o

ar rv filaEntrada.a filaEntrada.o
ar rv definicoes.a definicoes.o
ar rv filaSaida.a filaSaida.o
ar rv util.a util.o

gcc  main.c -L. filaEntrada.a filaSaida.a definicoes.a util.a -o drivePlacaBase.bin -lm -lrt -lpthread -lsqlite3

rm filaEntrada.a filaEntrada.o
rm definicoes.a definicoes.o
rm filaSaida.a filaSaida.o
rm util.a util.o

./drivePlacaBase.bin