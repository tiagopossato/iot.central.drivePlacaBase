gcc -c ../lib/dados.c -o dados.o 
gcc -c ../lib/definicoes.c -o definicoes.o 
ar rv dados.a dados.o
ar rv definicoes.a definicoes.o
gcc  main.c -L. dados.a definicoes.o -o main.bin -lm -lrt -lpthread
./main.bin

rm dados.a dados.o
rm definicoes.a definicoes.o

rm main.bin