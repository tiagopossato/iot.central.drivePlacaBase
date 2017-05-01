#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include "../lib/filaSaida.h"
#include "../lib/definicoes.h"
#include "../lib/util.h"
//porta serial
#include <unistd.h> /* UNIX standard function definitions */
#include <fcntl.h>  /* File control definitions */

typedef struct sPE
{
    FilaSaida *filaSaida;
    int portaSerial;
} ParametrosThreadEnvia;

/*
Padrão da URI:
idRede/tipoGrandeza/grandeza/valor
*/
void *monitoraMensagens(void *args)
{
    //pega os parametros enviados por argumento para a thread
    ParametrosThreadEnvia *params = (ParametrosThreadEnvia *)args;
    //pega a fila de dados
    FilaSaida *fila = params->filaSaida;
    //pega o descritor da porta serial
    int fd = params->portaSerial;
}

void enviaMensagem(Saida *dados, int portaSerial)
{
    if (dados == NULL)
        return;
    char uri[64];
    sprintf(uri, "%d/%d/%d/%f\n", dados->idRede, dados->tipoGrandeza, dados->grandeza, dados->valor);
    write(portaSerial, uri, strlen(uri));
    dados->ttl--;
}