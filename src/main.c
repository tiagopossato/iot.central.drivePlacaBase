#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include "../lib/dados.h"

/*
 * Testando a biblioteca
 */
int main(int argc, char **argv)
{
    printf("Iniciando...\n");
    if (sem_init(&semaforo, 0, 1) == -1)
    {
        printf("Erro no semáforo!");
    }
    unsigned int id = 1;
    unsigned char codigo = 70;
    unsigned char msg[7] = {};
    msg[0] = 23;
    msg[1] = 57;
    insereDados(id, codigo, msg);
    id = 2;
    codigo = 70;
    msg[0] = 19;
    msg[1] = 32;
    insereDados(id, codigo, msg);
    imprimeListaDados();

    return (EXIT_SUCCESS);
}
