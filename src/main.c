#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <signal.h>
#include <pthread.h>

#include "../lib/filaEntrada.h"
#include "../lib/definicoes.h"
#include "recebeDados.c"
#include "portaSerial.c"

FilaDados *DADOS;

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
    DADOS = iniciaFila();
    if (DADOS == NULL)
    {
        printf("Não pode iniciar a fila de dados!\n");
        exit(EXIT_FAILURE);
    }

    ParametrosRecebe params;
    params.fila = DADOS;
    params.portaSerial = fd;

    pthread_create(&thRecebeDados, NULL, recebeDados, &params);

    pthread_join(thRecebeDados, NULL);
    return (EXIT_SUCCESS);
}