#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <signal.h>
#include <pthread.h>
#include <sys/socket.h>

#include "../lib/filaEntrada.h"
#include "../lib/definicoes.h"
#include "recebeDados.c"
#include "enviaDados.c"
#include "portaSerial.c"
#include "socketServer.c"
#include "salvaBanco.c"

FilaEntrada *ENTRADA;
FilaSaida *SAIDA;

void encerraExecucao(int dummy)
{
    printf("\nSinal de encerramento recebido, mostrando, encerrando fila e saindo...\n");

    int i = ENTRADA->quantidade;
    for (; i > 0; i--)
    {
        Entrada *tmp;
        tmp = peekDados(ENTRADA);
        mostraNoEntrada(tmp);
        removeDoInicio(tmp, ENTRADA);
    }
    liberaFilaEntrada(ENTRADA);
    liberaFilaSaida(SAIDA);
    exit(EXIT_SUCCESS);
}

int main(int argc, char **argv)
{
    ENTRADA = iniciaFila();
    if (ENTRADA == NULL)
    {
        printf("Não pode iniciar a fila de entrada!\n");
        exit(EXIT_FAILURE);
    }

    SAIDA = iniciaFilaSaida();
    if (SAIDA == NULL)
    {
        printf("Não pode iniciar a fila de saida!\n");
        exit(EXIT_FAILURE);
    }

    signal(SIGINT, encerraExecucao);

    char *portname = "/dev/ttyUSB0";
    int portaSerial = open(portname, O_RDWR | O_NOCTTY | O_SYNC);
    if (portaSerial < 0)
    {
        printf("error %d opening %s: %s", errno, portname, strerror(errno));
        return;
    }
    set_interface_attribs(portaSerial, B115200, 0); // set speed to 115,200 bps, 8n1 (no parity)
    set_blocking(portaSerial, 1);                   // set blocking

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

    ParametrosSalvaBanco paramsSalvaBanco;
    paramsSalvaBanco.filaEntrada = ENTRADA;
    pthread_t thSalvaBanco;
    pthread_create(&thSalvaBanco, NULL, salvaBanco, &paramsSalvaBanco);

    //Abre socket para receber comandos
    int socket = abreSocket();
    if (socket == -1)
    {
        printf("Não pode iniciar o socket de entrada!\n");
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
            perror("accept error");
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
                enviaMensagem(SAIDA->tail, portaSerial);
                imprimeFilaSaida(SAIDA);
            }
        }
        if (rc == -1)
        {
            perror("read");
            continue;
            //exit(-1);
        }
        else if (rc == 0)
        {
            printf("EOF\nCliente desconectado...\n");
            close(cl);
        }
    }

    pthread_join(thRecebeDados, NULL);
    pthread_join(thMonitor, NULL);
    return (EXIT_SUCCESS);
}