gcc -c ../lib/entradaSegura.c -o entradaSegura.o
ar rv entradaSegura.a entradaSegura.o
gcc  main.c -L. entradaSegura.a -o main.bin -lm
rm entradaSegura.a entradaSegura.o
./main.bin
rm main.bin