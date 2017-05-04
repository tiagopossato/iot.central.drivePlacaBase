#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
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
} ParametrosThreadMonitor;

/*
Padrão da URI:
idRede/tipoGrandeza/grandeza/valor
*/
void enviaMensagem(Saida *dados, int portaSerial)
{
    if (dados == NULL)
        return;
    char uri[64];
    sprintf(uri, "%d/%d/%d/%f\n", dados->idRede, dados->tipoGrandeza, dados->grandeza, dados->valor);
    printf("Enviando: %s", uri);
    write(portaSerial, uri, strlen(uri));
    dados->ttl--;
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
        pthread_mutex_lock(&fila->mutex);
        if (fila->head == NULL)
        {
            /*Libera mutex*/
            pthread_mutex_unlock(&fila->mutex);
            continue;
        }
        printf("Timestamp: %d\n", time(0));
        Saida *tmp = fila->head;
        while (tmp != NULL)
        {
            if (tmp->ttl > 0)
            {
                printf("Reenviando...\n");
                enviaMensagem(tmp, params->portaSerial);
            }
            else
            {
                char msgTmp[128];
                sprintf(msgTmp, "Mensagem: [%d/%d/%d/%f] não foi enviada!", tmp->idRede, tmp->tipoGrandeza, tmp->grandeza, tmp->valor);
                logMessage("ENVIA", msgTmp, true);
                /*Libera mutex pois é usado na função apaga saida*/
                pthread_mutex_unlock(&fila->mutex);
                apagaNoSaida(tmp, fila);
            }
            tmp = (Saida *)tmp->next;
        }
        /*Libera mutex*/
        pthread_mutex_unlock(&fila->mutex);
    }
}