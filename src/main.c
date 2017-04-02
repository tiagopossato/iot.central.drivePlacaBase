#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <signal.h>
#include "../lib/dados.h"

FilaDados *DADOS;

bool validaGrandeza(unsigned int grandeza);
bool validaTipoGrandeza(unsigned int tipo);

void intHandler(int dummy)
{
    printf("\nSinal de encerramento recebido, encerrando fila e saindo...\n");
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

    signal(SIGINT, intHandler);

    char uri[20];
    while (true)
    {
        memset(uri, 1, sizeof(uri));
        scanf("%s", uri);
        char saida[5];
        unsigned int i = 0;
        unsigned char j = 0;
        unsigned char k = 0;
        //-------------------------------------------------
        for (k = 0; k < strlen(saida); k++)
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
        j = 0;
        idRede = (unsigned int)atoi(saida);
        //-------------------------------------------------
        for (k = 0; k < strlen(saida); k++)
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
        j = 0;
        tipoGrandeza = (unsigned int)atoi(saida);
        //-------------------------------------------------
        for (k = 0; k < strlen(saida); k++)
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
        j = 0;
        grandeza = (unsigned int)atoi(saida);
        //-------------------------------------------------
        for (k = 0; k < strlen(saida); k++)
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
        j = 0;
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

bool validaGrandeza(unsigned int grandeza)
{
    enum G
    {
        temperatura = 3303,
        umidadeAr = 3304,
        umidadeSolo = 3320
    } Grandezas;
    switch (grandeza)
    {
    case temperatura:
        return true;
    case umidadeAr:
        return true;
    case umidadeSolo:
        return true;
    default:
        return false;
    }
}

bool validaTipoGrandeza(unsigned int tipo)
{
    enum TP
    {
        entradaDigital = 3200,
        saidaDigital = 3201,
        entradaAnalogica = 3202
    } TipoGrandezas;
    switch (tipo)
    {
    case entradaDigital:
        return true;
    case saidaDigital:
        return true;
    case entradaAnalogica:
        return true;
    default:
        return false;
    }
}