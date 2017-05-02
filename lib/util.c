#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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
    //printf("entrada: %s[%d]\n", entrada,strlen(entrada));
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

void logMessage(char *tipo, char *msg)
{
    time_t timer;
    char data[26];
    struct tm *tm_info;

    time(&timer);
    tm_info = localtime(&timer);

    strftime(data, 26, "%Y-%m-%d %H:%M:%S", tm_info);

    printf("[%s] %s: %s\n", data, tipo, msg);
}