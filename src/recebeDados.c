#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../lib/dados.h"
#include "../lib/definicoes.h"
#include "portaSerial.c"

typedef struct s
{
    FilaDados *fila;
    int portaSerial;
}ParametrosRecebe;

int buscaCaracter(char *buf, char caracter)
{
    int pos = 0;
    for (; pos <= strlen(buf); pos++)
    {
        if (caracter == buf[pos])
        {
            return pos;
        }
    }
    return -1;
}

void *recebeDados(void *args)
{
    ParametrosRecebe *params;
    params = (ParametrosRecebe *) args;
    
    // FilaDados *fila = (FilaDados *)args;
    FilaDados *fila = params->fila;
    int fd = params->portaSerial;

    unsigned int idRede;
    unsigned int tipoGrandeza;
    unsigned int grandeza;
    float valor;

    char buf[64];
    char uri[64];
    int inicio = -1;
    int fim = -1;
    while (true)
    {
        //limpa as variaveis
        memset(buf, 1, sizeof(buf));
        memset(uri, '\0', sizeof(uri));
        //le porta serial
        if (read(fd, buf, sizeof(buf)) > 0)
        {
            inicio = buscaCaracter(buf, '[');
            fim = buscaCaracter(buf, ']');
            //validação 1 da entrada
            if (inicio < 0 || fim <= inicio)
            {
                continue;
            }
            //extração 1 dos dados
            memcpy(uri, &buf[inicio + 1], fim - (inicio + 1));
            //printf("%s\n", uri);
        }
        else
        {
            continue;
        }

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

        insereDados(idRede, tipoGrandeza, grandeza, valor, fila);
        imprimeFilaDados(fila);
    }
}