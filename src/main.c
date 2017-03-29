#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include "../lib/dados.h"

/*
Padrão da URI:
idRede/tipoGrandeza/grandeza/valor
*/

/*
Função que recebe uma URI e retira o último valor
*/
void extraiMsg(char *uri, char *saida)
{
    unsigned int i = 0;
    unsigned char j = 4;
    for (i = 0; i < sizeof(uri); i++)
    {
        if (i > 4 || uri[strlen(uri) - i] == '/')
        {
            break;
        }
        saida[j--] = uri[strlen(uri) - i];
    }
}
/*
 * Testando a biblioteca
 */
int main(int argc, char **argv)
{
    char uri[20];
    scanf("%s", uri);

    char saida[5];
    unsigned int i = 0;
    unsigned char j = 4;

     memset(saida, 1, sizeof(saida));
    for (; i < strlen(uri); i++)
    {
        if (uri[i] == '/')
        {
            break;
        }
        saida[j++] = uri[i];
    }
    printf("%s\n", saida);
    i++;
    j = 4;
    memset(saida, 1, sizeof(saida));

    for (; i < strlen(uri); i++)
    {
        if (uri[i] == '/')
        {
            break;
        }
        saida[j++] = uri[i];
    }
    printf("%s\n", saida);
    i++;
    j = 4;
    memset(saida, 1, sizeof(saida));

    for (; i < strlen(uri); i++)
    {
        if (uri[i] == '/')
        {
            break;
        }
        saida[j++] = uri[i];
    }
    printf("%s\n", saida);
    i++;
    j = 4;
    memset(saida, 1, sizeof(saida));

    for (; i <= strlen(uri); i++)
    {
        if (uri[i] == '/')
        {
            break;
        }
        saida[j++] = uri[i];
    }
    printf("%s\n", saida);

    return (EXIT_SUCCESS);

    unsigned int idRede = 2;
    unsigned int tipoGrandeza = 3202;
    unsigned int grandeza = 3303;
    float valor = 25.69;

    FilaDados *DADOS = iniciaFila();
    insereDados(idRede, tipoGrandeza, grandeza, valor, DADOS);

    idRede = 2;
    tipoGrandeza = 3202;
    grandeza = 3304;
    valor = 56.89;
    insereDados(idRede, tipoGrandeza, grandeza, valor, DADOS);
    imprimeFilaDados(DADOS);

    Dados *tmp;
    tmp = peekDados(DADOS);
    mostraDados(tmp);
    removeDados(tmp, DADOS);
    imprimeFilaDados(DADOS);

    return (EXIT_SUCCESS);
}
