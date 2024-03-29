#include <stdio.h>
#include <sqlite3.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h> /* UNIX standard function definitions */
#include <pthread.h>
#include "../lib/filaEntrada.h"
#include "../lib/definicoes.h"
#include "../lib/util.h"

pthread_mutex_t mutexBanco;
pthread_cond_t condicaoBanco;

char *zErrMsg = 0;

bool consultaVazia = true;

sqlite3 *banco;
typedef struct sSB
{
    FilaEntrada *filaEntrada;
    sqlite3 *db;
} ParametrosSalvaBanco;

void mensagemEspecial(Entrada *dado);
void status(char *info, int rc, FilaEntrada *fila);
static int callbackSelectAmbiente(void *_fila, int argc, char **argv, char **azColName);

void *salvaBanco(void *args)
{
    //pega os parametros enviados por argumento para a thread
    ParametrosSalvaBanco *params = (ParametrosSalvaBanco *)args;
    FilaEntrada *filaEntrada = params->filaEntrada;
    banco = params->db;

    char msgTmp[256];

    while (true)
    {
        pthread_cond_wait(&condicaoBanco, &mutexBanco);
        //printf("Chegaram dados, inserindo no banco...\n");

        if (filaEntrada->head == NULL)
        {
            /*Libera mutex*/
            continue;
        }

        while (filaEntrada->head != NULL)
        {
            //pthread_mutex_lock(&mutexBanco);
            char sql[256];

            switch (filaEntrada->head->tipoGrandeza)
            {
            case entradaDigital:
                sprintf( msgTmp, "Dispositivo %d, entrada digital:%d -> %d\n", filaEntrada->head->idRede, filaEntrada->head->grandeza, (int)filaEntrada->head->valor);
                logMessage("DIGITAL", msgTmp, true);
                removeDoInicio(filaEntrada);
                break;
            case saidaDigital:
                printf("Dispositivo %d, saída digital:%d -> %d\n", filaEntrada->head->idRede, filaEntrada->head->grandeza, (int)filaEntrada->head->valor);
                removeDoInicio(filaEntrada);
                break;
            case entradaAnalogica:
                sprintf(sql, "SELECT ambiente_id FROM central_sensor WHERE idRede = %d;", filaEntrada->head->idRede);
                //printf("%s\n", sql);
                consultaVazia = true;
                status("SELECT", sqlite3_exec(banco, sql, callbackSelectAmbiente, (void *)filaEntrada, &zErrMsg), filaEntrada);
                if (zErrMsg == NULL && consultaVazia)
                {
                    sprintf(msgTmp, "O sensor %d não está cadastrado!", filaEntrada->head->idRede);
                    logMessage("SENSOR", msgTmp, true);
                    removeDoInicio(filaEntrada);
                }
                break;
            case especial:
                mensagemEspecial(filaEntrada->head);
                removeDoInicio(filaEntrada);
                break;
            default:
                printf("Tipo de grandeza desconhecido: %d\n", filaEntrada->head->tipoGrandeza);
                removeDoInicio(filaEntrada);
                break;
            }
            //pthread_mutex_unlock(&mutexBanco);
        }
    }
}

void mensagemEspecial(Entrada *dado)
{
    char msgTmp[128];
    if (dado->grandeza == true)
    {
        sprintf(msgTmp, "Dispositivo: %d está online", dado->idRede);
        logMessage("ONLINE", msgTmp, dado->idRede == 0 ? false : true);
    }
}

static int callbackSelectAmbiente(void *_fila, int argc, char **argv, char **azColName)
{
    consultaVazia = false;
    char sql[256];
    FilaEntrada *fila = (FilaEntrada *)_fila;
    int ambienteId;
    int i;
    for (i = 0; i < argc; i++)
    {
        ambienteId = atoi(argv[i] ? argv[i] : "NULL");
        //printf("\t%s = %d\n", azColName[i], ambienteId);
    }

    sprintf(sql, "INSERT INTO central_leitura (valor, grandeza_id, sensor_id, ambiente_id, createdAt, sync) VALUES (%.2f, %d, %d, %d, %d, 0);",
            fila->head->valor, fila->head->grandeza, fila->head->idRede, ambienteId, fila->head->timestamp);
    //printf("callback_select SQL: %s\n", sql);
    status("INSERT", sqlite3_exec(banco, sql, NULL, 0, &zErrMsg), fila);
    if (zErrMsg == NULL)
    {
        //printf("INSERT: NAO DEU ERRO\n");
    }
    removeDoInicio(fila);
    return 0;
}

void status(char *info, int rc, FilaEntrada *fila)
{
    if (rc != SQLITE_OK)
    {
        printf("ERRO -> %s SQL error: %s\n", info, zErrMsg);
        sqlite3_free(zErrMsg);
        removeDoInicio(fila);
    }
    else
    {
        //printf("SUCESSO -> %s executado no Banco de Dados. SQL zErrMsg: %s\n", info, zErrMsg);
        sqlite3_free(zErrMsg);
    }
}
