#include "filaEntrada.h"
#include "definicoes.h"
#include "util.h"
#include <pthread.h>
#include <time.h>
#include <stdbool.h>
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
#if defined(DEBUG)
        printf("Erro no mutex!");
#endif
        return NULL;
    }

    if (fila == NULL)
    {
#if defined(DEBUG)
        printf("Erro ao criar fila de dados!");
#endif
        return NULL;
    }
    return fila;
}

/*
Cria uma estrutura do tipo Dados com os parâmetros recebidos
e insere na fila de Dados
*/
extern bool insereDadosEntrada(char *uri, FilaEntrada *fila)
{
    /*TODO: Validar os Dados antes de inserir na fila*/
    pthread_mutex_lock(&fila->mutex);

    //cria nova estrutura
    Entrada *novo = (Entrada *)malloc(sizeof(Entrada));
    if (!novo)
    {
        pthread_mutex_unlock(&fila->mutex);
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
        printf("Tipo de grandeza: %d não reconhecido!\n", novo->tipoGrandeza);
        free(novo);
        pthread_mutex_unlock(&fila->mutex);
        return false;
    }
    //VALIDA GRANDEZA
    if (!validaGrandeza(novo->grandeza, novo->tipoGrandeza))
    {
        printf("Grandeza: %d não reconhecida!\n", novo->grandeza);
        free(novo);
        pthread_mutex_unlock(&fila->mutex);
        return false;
    }

    //Adiciona Dados no final da fila
    adicionaFim(novo, fila);
    /* libera mutex */
    pthread_mutex_unlock(&fila->mutex);
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
extern bool removeDoInicio(Entrada *dado, FilaEntrada *fila)
{
    pthread_mutex_lock(&fila->mutex);

    if (dado == fila->head)
    {
        fila->head = (Entrada *)dado->next;
    }
    else
    {
#if defined(DEBUG)
        printf("Não é o primeiro nó!");
#endif
        pthread_mutex_unlock(&fila->mutex);
        return false;
    }

    dado->next = NULL;
    free(dado);

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
    if(dado == NULL) return;
    printf("idRede: %d\n", dado->idRede);
    printf("tipoGrandeza: %d\n", dado->tipoGrandeza);
    printf("grandeza: %d\n", dado->grandeza);
    printf("valor: %f\n", dado->valor);
    printf("timestamp: %d\n", (int)dado->timestamp);
}

extern void libera(FilaEntrada *fila)
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