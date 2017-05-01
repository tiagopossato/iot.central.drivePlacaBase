#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"

extern int buscaCaracter(char *buf, char caracter)
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
extern float extraiParte(char *entrada)
{
    char i = 0;
    char saida[6];
    memset(saida, '\r', sizeof(saida));
    for (i = 0;; i++)
    {
        //aceita valores de -32,768 até 32,767
        if (i > 5)
            break;
        if (entrada[i] == '/')
            break;
        saida[i] = entrada[i];
    }
    sprintf(entrada, entrada + i + 1);
    return atof(saida);
}