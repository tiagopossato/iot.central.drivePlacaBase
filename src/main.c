#include <stdio.h>
#include <stdlib.h>
#include "../lib/entradaSegura.h"

/*
 * Testando a biblioteca
 */
int main(int argc, char** argv) {

     printf("Voce digitou: %u\n", leUInt("Digite um numero sem sinal: "));
     printf("Voce digitou: %d\n", leInt("Digite um numero: "));
    return (EXIT_SUCCESS);
}
