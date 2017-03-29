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
 * Testando a biblioteca
 */
int main(int argc, char **argv)
{
    unsigned int idRede;
    unsigned int tipoGrandeza;
    unsigned int grandeza;
    float valor;
    FilaDados *DADOS = iniciaFila();

    char uri[20];
    loop:
    scanf("%s", uri);

    char saida[5];
    unsigned int i = 0;
    unsigned char j = 4;
    unsigned char k = 0;
    //-------------------------------------------------
    for (k = 0; k < 5; k++)
    {
        saida[k] = '\r';
    }
    for (; i < strlen(uri); i++)
    {
        if (uri[i] == '/')
        {
            break;
        }
        saida[j++] = uri[i];
    }
    i++;
    j = 4;
    idRede = (unsigned int)atoi(saida);
    //-------------------------------------------------
    for (k = 0; k < 5; k++)
    {
        saida[k] = '\r';
    }
    for (; i < strlen(uri); i++)
    {
        if (uri[i] == '/')
        {
            break;
        }
        saida[j++] = uri[i];
    }
    i++;
    j = 4;
    tipoGrandeza = (unsigned int)atoi(saida);
    //-------------------------------------------------
    for (k = 0; k < 5; k++)
    {
        saida[k] = '\r';
    }
    for (; i < strlen(uri); i++)
    {
        if (uri[i] == '/')
        {
            break;
        }
        saida[j++] = uri[i];
    }
    i++;
    j = 4;
    grandeza = (unsigned int)atoi(saida);
    //-------------------------------------------------
    for (k = 0; k < 5; k++)
    {
        saida[k] = '\r';
    }
    for (; i < strlen(uri); i++)
    {
        if (uri[i] == '/')
        {
            break;
        }
        saida[j++] = uri[i];
    }
    i++;
    j = 4;
    valor = atof(saida);
    //-------------------------------------------------

    insereDados(idRede, tipoGrandeza, grandeza, valor, DADOS);
    imprimeFilaDados(DADOS);
    memset(uri, 1, sizeof(uri));
    goto loop;

    return (EXIT_SUCCESS);
}
