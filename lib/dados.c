#include "dados.h"
#include <semaphore.h>
#include <time.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct sListaDados
{
    Dados *head;
    int quantidade;
} ListaDados;

ListaDados *DADOS;

ListaDados *criaListaDados();
Dados *aloca(unsigned int _id, unsigned char _codigo, unsigned char *_msg);
void adicionaFim(Dados *novo);
bool removeLeitura(char posicao);

/*
Cria uma estrutura do tipo Dados com os parâmetros recebidos
e insere na lista de Dados
*/
bool insereDados(unsigned int _id, unsigned char _codigo, unsigned char *_msg)
{
    /*TODO: Validar os Dados antes de inserir na lista*/
    if (DADOS == NULL)
    {
        DADOS = criaListaDados();
    }

    sem_wait(&semaforo);
    //cria nova estrutura
    Dados *novo = aloca(_id, _codigo, _msg);
    //Adiciona Dados no final da lista
    adicionaFim(novo);
    /* libera semaforo */
    sem_post(&semaforo);
    return true;
}

/**
 * Cria a lista de Dados
 * @return Estrutura com a lista de Dados
 */
ListaDados *criaListaDados()
{

    sem_wait(&semaforo);

    ListaDados *lista;
    lista = (ListaDados *)malloc(sizeof(ListaDados));
    lista->head = NULL;
    lista->quantidade = 0;

    /*Libera semaforo*/
    sem_post(&semaforo);
    return lista;
}

/**
 * Cria nova estutura do tipo Dados e aloca espaco na memória
 * @return A estrutura alocada.
 */
Dados *aloca(unsigned int _id, unsigned char _codigo, unsigned char *_msg)
{
    Dados *novo = (Dados *)malloc(sizeof(Dados));
    if (!novo)
    {
        return NULL;
    }
    novo->id = _id;
    novo->codigo = _codigo;
    int i;    
    for (i = 0; i < 7; i++)
    {        
        novo->msg[i] = _msg[i];
    }
    novo->timestamp = time(0);
    novo->prox = NULL;
    return novo;
}

/**
 * Adiciona novos Dados no final da lista
 * @param novo Estrutura do tipo Dados
 */
void adicionaFim(Dados *novo)
{
    //verifica se a lista esta vazia
    if (DADOS->head == NULL)
    {
        //caso estiver, aponta para a estrutura criada
        DADOS->head = novo;
    }
    else
    {
        //caso nao estiver vazia
        //cria um nó temporario, apontando para o primeiro elemento da lista
        Dados *tmp = DADOS->head;
        //percorre a lista ate encontrar o ultimo elemento
        while (tmp->prox != NULL)
        {
            tmp = tmp->prox;
        }
        //insere o elemento novo no final da lista
        tmp->prox = novo;
    }
    DADOS->quantidade++;
}

/**
 * Remove item na posicao desejada
 */
bool removeDados(Dados *dado)
{

    //cria nova struct e aponta para o primeiro valor da lista
    Dados *anterior = DADOS->head,
          //cria nova struct e aponta para o segundo valor
        *proxima = anterior->prox;

    //caso for o primeiro elemento
    if (dado == anterior)
    {
        DADOS->head = proxima;
        //libera memoria
        free(anterior);
    }
    else
    {
        while (proxima != dado)
        {
            //desloca para o próximo item
            anterior = proxima;
            proxima = proxima->prox;
            //verifica se chegou no fim da lista
            if (proxima == NULL)
            {
                return false;
            }
        }
    }
    //aponta para o elemento apontado pelo proximo elemento da lista;
    anterior->prox = proxima->prox;
    //libera memoria
    free(proxima);

    DADOS->quantidade--;
    return true;
}

void imprimeListaDados()
{
    sem_wait(&semaforo);

    if (DADOS->head == NULL)
    {
        /*Libera semaforo*/
        sem_post(&semaforo);
        return;
    }

    Dados *tmp = DADOS->head;
    printf("\nDADOS:\n");
    int i = 0;
    while (tmp != NULL)
    {
        printf("%d ", tmp->id);
        printf("%d ", tmp->codigo);
        for (i = 0; i < 7; i++)
        {
            printf("%d ", tmp->msg[i]);
        }
        printf("%d\n", (int)tmp->timestamp);
        tmp = tmp->prox;
    }
    printf("\n");
    /*Libera semaforo*/
    sem_post(&semaforo);
}

void libera()
{
    if (DADOS->head != NULL)
    {
        Dados *proxDado, *atual;
        atual = DADOS->head;
        while (atual != NULL)
        {
            proxDado = atual->prox;
            free(atual);
            atual = proxDado;
        }
    }
}