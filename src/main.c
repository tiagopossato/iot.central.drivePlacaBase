#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <signal.h>
#include <pthread.h>
#include <sqlite3.h>
#include <sys/socket.h>
#include <errno.h> /* Error number definitions */

#include "../lib/filaEntrada.h"
#include "../lib/definicoes.h"
#include "../lib/util.h"

#include "enviaDados.c"
#include "portaSerial.c"
#include "socketServer.c"
#include "salvaBanco.c"
#include "recebeDados.c"

FilaEntrada *ENTRADA;
FilaSaida *SAIDA;

void encerraExecucao(int dummy)
{
    printf("\nSinal de encerramento recebido, mostrando, encerrando fila e saindo.\n");
    logMessage("MAIN", "Encerrando aplicação", true);
    /*
    int i = ENTRADA->quantidade;
    for (; i > 0; i--)
    {
        Entrada *tmp;
        tmp = peekDados(ENTRADA);
        mostraNoEntrada(tmp);
        removeDoInicio(tmp, ENTRADA);
    }
    */
    liberaFilaEntrada(ENTRADA);
    liberaFilaSaida(SAIDA);
    exit(EXIT_SUCCESS);
}

int main(int argc, char **argv)
{
    char msgTmp[128];
    pthread_mutex_init(&mutexBanco, NULL);
    pthread_cond_init(&condicaoBanco, NULL);

    sqlite3 *db;

    if (sqlite3_open("/opt/iot.central/banco/db.sqlite3", &db))
    {
        sprintf(msgTmp, "ERRO -> Problema na conexão com o Banco de Dados: %s\n", sqlite3_errmsg(db));
        logMessage("MAIN", msgTmp, false);
        return (0);
    }
    else
    {
        printf("SUCESSO -> Conexão com o Banco de Dados realizada!\n");
        sqlite3_exec(db, "PRAGMA foreign_keys = ON;", NULL, NULL, NULL);
    }

    logMessage("MAIN", "Iniciando aplicação...", true);
    char portname[16];
    /*Abre o arquivo de configurações e pega o porta*/
    FILE *arq = fopen("/opt/iot.central/driveConfig", "r");
    if (arq == NULL)
    {
        logMessage("MAIN", "Arquivo de configurações não encontrado!", true);
        return;
    }
    fscanf(arq, "%s", portname); //le a porta
    fscanf(arq, "%d", &maxTTL);  // le o TTL, variável declarada na filaSaida
    fclose(arq);                 //fecha o arquivo

    ENTRADA = iniciaFila();
    if (ENTRADA == NULL)
    {
        logMessage("MAIN", "Não pode iniciar a fila de entrada!", true);
        exit(EXIT_FAILURE);
    }

    SAIDA = iniciaFilaSaida();
    if (SAIDA == NULL)
    {
        logMessage("MAIN", "Não pode iniciar a fila de saida!", true);
        exit(EXIT_FAILURE);
    }

    signal(SIGINT, encerraExecucao);

    int portaSerial = open(portname, O_RDWR | O_NOCTTY | O_SYNC);
    if (portaSerial < 0)
    {
        char tmp[128];
        sprintf(tmp, "error %d opening %s: %s", errno, portname, strerror(errno));
        logMessage("MAIN", tmp, true);
        return;
    }
    set_interface_attribs(portaSerial, B115200, 0); // set speed to 115,200 bps, 8n1 (no parity)
    set_blocking(portaSerial, 1);                   // set blocking

    ParametrosSalvaBanco paramsSalvaBanco;
    paramsSalvaBanco.filaEntrada = ENTRADA;
    paramsSalvaBanco.db = db;
    pthread_t thSalvaBanco;
    pthread_create(&thSalvaBanco, NULL, salvaBanco, &paramsSalvaBanco);

    ParametrosThreadRecebe paramsRecebe;
    paramsRecebe.filaEntrada = ENTRADA;
    paramsRecebe.filaSaida = SAIDA;
    paramsRecebe.portaSerial = portaSerial;
    pthread_t thRecebeDados;
    pthread_create(&thRecebeDados, NULL, recebeDados, &paramsRecebe);

    ParametrosThreadMonitor paramsMonitor;
    paramsMonitor.filaSaida = SAIDA;
    paramsMonitor.portaSerial = portaSerial;
    pthread_t thMonitor;
    pthread_create(&thMonitor, NULL, monitoraMensagens, &paramsMonitor);

    //Abre socket para receber comandos
    int socket = abreSocket();
    if (socket == -1)
    {
        logMessage("MAIN", "Não pode iniciar o socket de entrada!", true);
        exit(EXIT_FAILURE);
    }

    int cl, rc;
    char buf[64];
    //recebe os comandos via socket e envia para placaBase
    while (1)
    {
        memset(buf, '\0', sizeof(buf));
        if ((cl = accept(socket, NULL, NULL)) == -1) //chamada bloqueante
        {
            logMessage("MAIN", "Erro ao aceitar conexão do cliente no socket", true);
            continue;
        }

        while ((rc = read(cl, buf, sizeof(buf))) > 0)
        {
            if (rc > 1)
            {
                printf("\n******************************\nSolicitando:%s\n", buf);
                int res = insereDadosSaida(buf, SAIDA);
                //sprintf(buf, "%d", res);
                //printf("%s\n", buf);
                //write(socket, buf, rc);
                enviaMensagem(SAIDA->tail, SAIDA, portaSerial);
                //imprimeFilaSaida(SAIDA);
            }
        }
        if (rc == -1)
        {
            logMessage("MAIN", "Erro ao ler dados enviados pelo cliente no socket", true);
            continue;
            //exit(-1);
        }
        else if (rc == 0)
        {
            logMessage("MAIN", "Cliente desconectado...", false);
            close(cl);
        }
    }

    pthread_join(thRecebeDados, NULL);
    pthread_join(thMonitor, NULL);
    return (EXIT_SUCCESS);
}
