#include "filaEntrada.h"
#include "definicoes.h"
#include "util.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG

//Função privada para adicioar elemento no final da fila
void adicionaFim(Entrada *novo, FilaEntrada *fila);

/**
 *  Inicia todos os recursos da fila
 */
extern FilaEntrada *iniciaFila()
{
    FilaEntrada *fila;
    fila = (FilaEntrada *)malloc(sizeof(FilaEntrada));
    fila->head = NULL;
    fila->tail = NULL;
    fila->quantidade = 0;
    if (pthread_mutex_init(&fila->mutex, NULL) == -1)
    {
        logMessage("FLENT", "Erro no mutex!", true);
        return NULL;
    }

    if (fila == NULL)
    {
        logMessage("FLENT", "Erro ao criar fila de dados!", true);
        return NULL;
    }
    return fila;
}

/*
Cria uma estrutura do tipo Dados com os parâmetros recebidos
e insere na fila de Dados
*/
extern bool insereDadosEntrada(char *uri, FilaEntrada *filaEntrada, FilaSaida *filaSaida)
{
    char msgTmp[128];
    /*TODO: Validar os Dados antes de inserir na fila*/
    pthread_mutex_lock(&filaEntrada->mutex);

    //cria nova estrutura
    Entrada *novo = (Entrada *)malloc(sizeof(Entrada));
    if (!novo)
    {
        pthread_mutex_unlock(&filaEntrada->mutex);
        return false;
    }
    novo->idRede = (unsigned int)extraiParte(uri);
    novo->tipoGrandeza = (unsigned int)extraiParte(uri);
    novo->grandeza = (unsigned int)extraiParte(uri);
    novo->valor = extraiParte(uri);
    novo->timestamp = time(0);
    novo->prev = NULL;
    novo->next = NULL;

    //VALIDA TIPO DA GRANDEZA
    if (!validaTipoGrandeza(novo->tipoGrandeza))
    {
        sprintf(msgTmp, "Tipo de grandeza: %d não reconhecido!", novo->tipoGrandeza);
        logMessage("FLENT", msgTmp, true);
        free(novo);
        pthread_mutex_unlock(&filaEntrada->mutex);
        return false;
    }
    //VALIDA GRANDEZA
    if (!validaGrandeza(novo->grandeza, novo->tipoGrandeza))
    {
        sprintf(msgTmp, "Grandeza: %d não reconhecida!", novo->grandeza);
        logMessage("FLENT", msgTmp, true);
        free(novo);
        pthread_mutex_unlock(&filaEntrada->mutex);
        return false;
    }

    //Adiciona Dados no final da fila
    adicionaFim(novo, filaEntrada);
    /* libera mutex */
    pthread_mutex_unlock(&filaEntrada->mutex);

    //retira nó corresponte à solicitação na fila de saida
    Saida *busca = buscaNoSaida(novo->idRede, novo->tipoGrandeza, novo->grandeza, filaSaida);
    apagaNoSaida(busca, filaSaida);

    return true;
}

/**
 * Adiciona novos Dados no final da fila
 * @param novo Estrutura do tipo Dados
 */
void adicionaFim(Entrada *novo, FilaEntrada *fila)
{
    //printf("Tamanho da fila: %d\n", fila->quantidade);
    //verifica se a fila esta vazia
    if (fila->head == NULL)
    {
        //caso estiver, aponta para a estrutura criada
        fila->head = novo;
        fila->tail = novo;
    }
    else
    {
        //caso nao estiver vazia
        //insere na fila
        novo->prev = fila->tail;
        fila->tail->next = novo;
        //aponta fim da fila para o último
        fila->tail = novo;
    }
    fila->quantidade++;
}

extern Entrada *peekDados(FilaEntrada *fila)
{
    pthread_mutex_lock(&fila->mutex);

    //pega o primeiro nó da fila
    Entrada *tmp = fila->head;

    /*Libera mutex*/
    pthread_mutex_unlock(&fila->mutex);

    //retorna o nó
    return tmp;
}
/**
 * Remove item na posicao desejada
 */
extern bool removeDoInicio(FilaEntrada *fila)
{
    pthread_mutex_lock(&fila->mutex);

    if (fila->head == NULL)
    {
        pthread_mutex_unlock(&fila->mutex);
        return true;
    }

    Entrada *tmp = fila->head;
    fila->head = tmp->next;

    tmp->next = NULL;
    free(tmp);

    fila->quantidade--;
    /*Libera mutex*/
    pthread_mutex_unlock(&fila->mutex);
    return true;
}

extern void imprimeFilaDados(FilaEntrada *fila)
{
    pthread_mutex_lock(&fila->mutex);

    if (fila->head == NULL)
    {
        /*Libera mutex*/
        pthread_mutex_unlock(&fila->mutex);
        return;
    }

    Entrada *tmp = fila->head;
    printf("\nFila de DADOS:\n");
    while (tmp != NULL)
    {
        mostraNoEntrada(tmp);
        tmp = (Entrada *)tmp->next;
    }
    printf("\n");
    /*Libera mutex*/
    pthread_mutex_unlock(&fila->mutex);
}

extern void mostraNoEntrada(Entrada *dado)
{

    printf("\n--------[ %p ]---------\n", dado);
    if (dado == NULL)
        return;
    printf("idRede: %d\n", dado->idRede);
    printf("tipoGrandeza: %d\n", dado->tipoGrandeza);
    printf("grandeza: %d\n", dado->grandeza);
    printf("valor: %f\n", dado->valor);
    printf("timestamp: %d\n", (int)dado->timestamp);
}

extern void liberaFilaEntrada(FilaEntrada *fila)
{
    if (fila->head != NULL)
    {
        Entrada *proxDado, *atual;
        atual = fila->head;
        while (atual != NULL)
        {
            proxDado = (Entrada *)atual->next;
            free(atual);
            atual = proxDado;
        }
    }
}