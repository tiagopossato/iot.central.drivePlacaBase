gcc -c ../lib/dados.c -o dados.o
ar rv dados.a dados.o
gcc  main.c -L. dados.a -o main.bin -lm -lrt -lpthread
rm dados.a dados.o
./main.bin
rm main.bin