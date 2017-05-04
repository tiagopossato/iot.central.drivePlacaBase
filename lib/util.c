#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sqlite/sqlite3.h>

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

void logMessage(char *tipo, char *msg, bool salvar)
{
    sqlite3 *dbLog;
    char *zErrMsg;
    char sql[256];

    time_t timer;
    char data[26];
    struct tm *tm_info;

    time(&timer);
    tm_info = localtime(&timer);

    strftime(data, 26, "%Y-%m-%d %H:%M:%S", tm_info);

    printf("[%s] %s: %s\n", data, tipo, msg);

    if (!salvar)
        return;

        
    if (sqlite3_open("/opt/optativa/db.sqlite3", &dbLog))
    {
        printf("ERRO -> Problema na conexão com o Banco de Dados: %s\n", sqlite3_errmsg(dbLog));
        return;
    }

    sprintf(sql, "INSERT INTO central_log (tipo, mensagem, tempo) VALUES(\"%s\", \"%s\", %d)", tipo, msg, time(0));

    printf("%s\n", sql);

    sqlite3_exec(dbLog, sql, NULL, NULL, &zErrMsg);
    if (zErrMsg != NULL)
    {
        printf("Erro na inserção do LOG: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }

    sqlite3_close(dbLog);
}