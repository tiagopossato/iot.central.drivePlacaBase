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
#include "portaSerial.c"
#include "socketServer.c"

FilaDados *DADOS;
FilaSaida *SAIDA;

void intHandler(int dummy)
{
    printf("\nSinal de encerramento recebido, mostrando, encerrando fila e saindo...\n");

    int i = DADOS->quantidade;
    for (; i > 0; i--)
    {
        Dados *tmp;
        tmp = peekDados(DADOS);
        mostraDados(tmp);
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
    int fd = open(portname, O_RDWR | O_NOCTTY | O_SYNC);
    if (fd < 0)
    {
        printf("error %d opening %s: %s", errno, portname, strerror(errno));
        return;
    }
    set_interface_attribs(fd, B115200, 0); // set speed to 115,200 bps, 8n1 (no parity)
    set_blocking(fd, 1);                   // set blocking

    pthread_t thRecebeDados;

    ParametrosRecebe params;
    params.fila = DADOS;
    params.portaSerial = fd;

    pthread_create(&thRecebeDados, NULL, recebeDados, &params);

    int socket = abreSocket();
    if (socket == -1)
    {
        printf("Não pode iniciar o socket de entrada!\n");
        exit(EXIT_FAILURE);
    }
    int cl, rc;
    char buf[100];

    while (1)
    {
        memset(buf, '\0', sizeof(buf));
        if ((cl = accept(socket, NULL, NULL)) == -1)
        {
            perror("accept error");
            continue;
        }

        while ((rc = read(cl, buf, sizeof(buf))) > 0)
        {
            if (rc > 1)
            {
                printf("%s", buf);
                int res = insereDadosSaida(buf, SAIDA);
                sprintf(buf, "%d", res);
                printf("%s\n", buf);
                write(socket, buf, rc);
                mostraNoSaida(SAIDA->tail);
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
            printf("EOF\n");
            close(cl);
        }
    }

    pthread_join(thRecebeDados, NULL);
    return (EXIT_SUCCESS);
}