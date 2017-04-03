#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../lib/dados.h"
#include "../lib/definicoes.h"

void *recebeDados(void *args){
    FilaDados *fila =  (FilaDados *)args;
    unsigned int idRede;
    unsigned int tipoGrandeza;
    unsigned int grandeza;
    float valor;
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

        insereDados(idRede, tipoGrandeza, grandeza, valor, fila);
        imprimeFilaDados(fila);
    }
}