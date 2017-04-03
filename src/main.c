#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <signal.h>
#include "../lib/dados.h"
#include "../lib/definicoes.h"

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
    unsigned int idRede;
    unsigned int tipoGrandeza;
    unsigned int grandeza;
    float valor;
    DADOS = iniciaFila();
    if (DADOS == NULL)
    {
        printf("Não pode iniciar a fila de dados!\n");
        exit(EXIT_FAILURE);
    }

    signal(SIGINT, intHandler);

    char uri[20];
    while (true)
    {
        memset(uri, 1, sizeof(uri));
        scanf("%s", uri);
        char saida[5];
        unsigned int i = 0;
        unsigned char j = 0;
        //-------------------------------------------------
        memset(saida, '\r', sizeof(saida));
        for (; i < strlen(uri); i++)
        {
            if (uri[i] == '/')
            {
                i++;
                j = 0;
                break;
            }
            saida[j++] = uri[i];
        }
        idRede = (unsigned int)atoi(saida);
        //-------------------------------------------------
        memset(saida, '\r', sizeof(saida));
        for (; i < strlen(uri); i++)
        {
            if (uri[i] == '/')
            {
                i++;
                j = 0;
                break;
            }
            saida[j++] = uri[i];
        }
        tipoGrandeza = (unsigned int)atoi(saida);
        //-------------------------------------------------
        memset(saida, '\r', sizeof(saida));
        for (; i < strlen(uri); i++)
        {
            if (uri[i] == '/')
            {
                i++;
                j = 0;
                break;
            }
            saida[j++] = uri[i];
        }
        grandeza = (unsigned int)atoi(saida);
        //-------------------------------------------------
        memset(saida, '\r', sizeof(saida));
        for (; i < strlen(uri); i++)
        {
            if (uri[i] == '/')
            {
                i++;
                j = 0;
                break;
            }
            saida[j++] = uri[i];
        }
        valor = atof(saida);
        //-------------------------------------------------

        if (!validaTipoGrandeza(tipoGrandeza))
        {
            printf("Tipo de grandeza: %d não reconhecido!\n", tipoGrandeza);
            continue;
        }

        if (!validaGrandeza(grandeza))
        {
            printf("Grandeza: %d não reconhecida!\n", grandeza);
            continue;
        }

        insereDados(idRede, tipoGrandeza, grandeza, valor, DADOS);
        imprimeFilaDados(DADOS);
    }

    return (EXIT_SUCCESS);
}