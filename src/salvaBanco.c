#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h> /* UNIX standard function definitions */

#include "../lib/filaEntrada.h"
#include "../lib/definicoes.h"
#include "../lib/util.h"

typedef struct sSB
{
    FilaEntrada *filaEntrada;
} ParametrosSalvaBanco;

void *salvaBanco(void *args)
{
    //pega os parametros enviados por argumento para a thread
    ParametrosSalvaBanco *params = (ParametrosSalvaBanco *)args;

    while (true)
    {
        //aguarda X microsegundos
        usleep(5000);
    }
}