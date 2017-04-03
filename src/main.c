#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <signal.h>
#include <pthread.h>
#include "../lib/dados.h"
#include "../lib/definicoes.h"
#include "recebeDados.c"

FilaDados *DADOS;

void intHandler(int dummy)
{
    printf("\nSinal de encerramento recebido, mostrando, encerrando fila e saindo...\n");
    int i = DADOS->quantidade;
    for (; i > 0; i--)
    {
        Dados *tmp;
        tmp = peekDados(DADOS);
        mostraDados(tmp);
        removeDoInicio(tmp, DADOS);
    }

    libera(DADOS);
    exit(EXIT_SUCCESS);
}

/*
Padrão da URI:
idRede/tipoGrandeza/grandeza/valor
*/

/*
 * Testando a biblioteca
 */
int main(int argc, char **argv)
{

    pthread_t thRecebeDados;
    DADOS = iniciaFila();
    if (DADOS == NULL)
    {
        printf("Não pode iniciar a fila de dados!\n");
        exit(EXIT_FAILURE);
    }

    signal(SIGINT, intHandler);

    pthread_create(&thRecebeDados, NULL, recebeDados, DADOS);

    pthread_join(thRecebeDados,NULL);
    return (EXIT_SUCCESS);
}