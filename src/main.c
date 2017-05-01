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

FilaEntrada *DADOS;
FilaSaida *SAIDA;

void intHandler(int dummy)
{
    printf("\nSinal de encerramento recebido, mostrando, encerrando fila e saindo...\n");

    int i = DADOS->quantidade;
    for (; i > 0; i--)
    {
        Entrada *tmp;
        tmp = peekDados(DADOS);
        mostraNoEntrada(tmp);
        removeDoInicio(tmp, DADOS);
    }

    libera(DADOS);
    exit(EXIT_SUCCESS);
}

int main(int argc, char **argv)
{
    DADOS = iniciaFila();
    if (DADOS == NULL)
    {
        printf("Não pode iniciar a fila de dados!\n");
        exit(EXIT_FAILURE);
    }

    SAIDA = iniciaFilaSaida();
    if (SAIDA == NULL)
    {
        printf("Não pode iniciar a fila de saida!\n");
        exit(EXIT_FAILURE);
    }

    signal(SIGINT, intHandler);

    char *portname = "/dev/ttyUSB0";
    int portaSerial = open(portname, O_RDWR | O_NOCTTY | O_SYNC);
    if (portaSerial < 0)
    {
        printf("error %d opening %s: %s", errno, portname, strerror(errno));
        return;
    }
    set_interface_attribs(portaSerial, B115200, 0); // set speed to 115,200 bps, 8n1 (no parity)
    set_blocking(portaSerial, 1);                   // set blocking

    pthread_t thRecebeDados;

    ParametrosThreadRecebe params;
    params.filaEntrada = DADOS;
    params.filaSaida = SAIDA;
    params.portaSerial = portaSerial;

    pthread_create(&thRecebeDados, NULL, recebeDados, &params);

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
        if ((cl = accept(socket, NULL, NULL)) == -1)//chamada bloqueante
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
    return (EXIT_SUCCESS);
}