#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../lib/filaSaida.h"
#include "../lib/definicoes.h"
#include "../lib/util.h"
//#include <sys/time.h>
//porta serial
#include <unistd.h> /* UNIX standard function definitions */
#include <fcntl.h>  /* File control definitions */

typedef struct sPE
{
    FilaSaida *filaSaida;
    int portaSerial;
} ParametrosThreadMonitor;


/*
Padrão da URI:
idRede/tipoGrandeza/grandeza/valor
*/
long long lastTimestamp;
void enviaMensagem(Saida *dados, FilaSaida *fila, int portaSerial)
{

    if (dados == NULL)
        return;

    while ((currentTimestamp() - lastTimestamp) < 50)
    {
        usleep(500);
    }
    //printf("Esperou: %lld\n", currentTimestamp() -lastTimestamp);
    lastTimestamp = currentTimestamp();

    char uri[64];
    sprintf(uri, "[%d/%d/%d/%.2f]\n", dados->idRede, dados->tipoGrandeza, dados->grandeza, dados->valor);
    //printf("%s", uri);
    write(portaSerial, uri, strlen(uri));
    //printf("enviaMensagem: Bloqueando fila de saida\n");

    pthread_mutex_lock(&fila->mutex);
    dados->ttl--;
    /*Libera mutex*/
    pthread_mutex_unlock(&fila->mutex);
    //printf("enviaMensagem: Liberou fila de saida\n");
}

void *monitoraMensagens(void *args)
{
    //pega os parametros enviados por argumento para a thread
    ParametrosThreadMonitor *params = (ParametrosThreadMonitor *)args;
    //pega a fila de dados
    FilaSaida *fila = params->filaSaida;
    //tenta enviar as mensagens da fila de saída
    while (true)
    {
        //aguarda 2 segundos
        sleep(2);
        if (fila->head == NULL)
        {
            continue;
        }
        //printf("Timestamp: %d\n", time(0));
        Saida *tmp = fila->head;
        while (tmp != NULL)
        {
            if (tmp->ttl > 0)
            {
                printf("Reenviando: %d/%d/%d/%.2f\n", tmp->idRede, tmp->tipoGrandeza, tmp->grandeza, tmp->valor);                
                enviaMensagem(tmp, fila, params->portaSerial);
            }
            else
            {
                char msgTmp[128];
                sprintf(msgTmp, "Mensagem: [%d/%d/%d/%f] não obteve resposta!", tmp->idRede, tmp->tipoGrandeza, tmp->grandeza, tmp->valor);
                logMessage("ENVIA", msgTmp, true);
                apagaNoSaida(tmp, fila);
            }
            tmp = (Saida *)tmp->next;
        }
    }
}