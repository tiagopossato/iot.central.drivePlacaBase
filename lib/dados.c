#include "dados.h"
#include <semaphore.h>
#include <time.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct sFilaDados
{
    Dados *head;
    int quantidade;
} FilaDados;

FilaDados *DADOS;

sem_t semaforo;

FilaDados *criaFilaDados();
Dados *aloca(unsigned int _idRede, unsigned char _codigo, unsigned char *_msg);
void adicionaFim(Dados *novo);
bool removeLeitura(char posicao);

bool iniciado = false;
/**
 *  Inicia todos os recursos da fila
 */
bool iniciaFila(){
    if (sem_init(&semaforo, 0, 1) == -1)
    {
        #if defined(DEBUG)
            printf("Erro no semáforo!");
        #endif
        return false;
    }
    DADOS = criaFilaDados();
    if(DADOS == NULL){
        #if defined(DEBUG)
            printf("Erro ao criar fila de dados!");
        #endif
        
        return false;
    }
    iniciado = true;
}

/*
Cria uma estrutura do tipo Dados com os parâmetros recebidos
e insere na fila de Dados
*/
bool insereDados(unsigned int _id, unsigned char _codigo, unsigned char *_msg)
{
    /*TODO: Validar os Dados antes de inserir na fila*/
    if (!iniciado)
    {
        iniciaFila();
    }

    sem_wait(&semaforo);
    //cria nova estrutura
    Dados *novo = aloca(_id, _codigo, _msg);
    //Adiciona Dados no final da fila
    adicionaFim(novo);
    /* libera semaforo */
    sem_post(&semaforo);
    return true;
}

/**
 * Cria a fila de Dados
 * @return Estrutura com a fila de Dados
 */
FilaDados *criaFilaDados()
{

    sem_wait(&semaforo);

    FilaDados *fila;
    fila = (FilaDados *)malloc(sizeof(FilaDados));
    fila->head = NULL;
    fila->quantidade = 0;

    /*Libera semaforo*/
    sem_post(&semaforo);
    return fila;
}

/**
 * Cria nova estutura do tipo Dados e aloca espaco na memória
 * @return A estrutura alocada.
 */
Dados *aloca(unsigned int _idRede, unsigned char _codigo, unsigned char *_msg)
{
    Dados *novo = (Dados *)malloc(sizeof(Dados));
    if (!novo)
    {
        return NULL;
    }
    novo->idRede = _idRede;
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
 * Adiciona novos Dados no final da fila
 * @param novo Estrutura do tipo Dados
 */
void adicionaFim(Dados *novo)
{
    //verifica se a fila esta vazia
    if (DADOS->head == NULL)
    {
        //caso estiver, aponta para a estrutura criada
        DADOS->head = novo;
    }
    else
    {
        //caso nao estiver vazia
        //cria um nó temporario, apontando para o primeiro elemento da fila
        Dados *tmp = DADOS->head;
        //percorre a fila ate encontrar o ultimo elemento
        while (tmp->prox != NULL)
        {
            tmp = tmp->prox;
        }
        //insere o elemento novo no final da fila
        tmp->prox = novo;
    }
    DADOS->quantidade++;
}

Dados *peekDados(){
    if (!iniciado)
    {
        iniciaFila();
    }
    sem_wait(&semaforo);
    
    //cri nó para percorrer a fila
    Dados *tmp = DADOS->head;
    // //percorre a fila ate encontrar o ultimo elemento
    // while (tmp->prox != NULL)
    // {
    //     tmp = tmp->prox;
    // }
    /*Libera semaforo*/
    sem_post(&semaforo);
    return tmp;
}
/**
 * Remove item na posicao desejada
 */
bool removeDados(Dados *dado)
{
    
#if defined(DEBUG)
  printf("Removendo no\n");
#endif
    
    //cria nova struct e aponta para o primeiro valor da fila
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
            //verifica se chegou no fim da fila
            if (proxima == NULL)
            {
                return false;
            }
        }
        //aponta para o elemento apontado pelo proximo elemento da fila;
        anterior->prox = proxima->prox;
        //libera memoria
        free(proxima);
    }

    DADOS->quantidade--;
    return true;
}

void imprimeFilaDados()
{
    sem_wait(&semaforo);

    if (DADOS->head == NULL)
    {
        /*Libera semaforo*/
        sem_post(&semaforo);
        return;
    }

    Dados *tmp = DADOS->head;
    printf("\nFila de DADOS:\n");
    while (tmp != NULL)
    {
        mostraDados(tmp);
        tmp = tmp->prox;
    }
    printf("\n");
    /*Libera semaforo*/
    sem_post(&semaforo);
}

void mostraDados(Dados *dado){
        int i = 0;
        printf("%d ", dado->idRede);
        printf("%d ", dado->codigo);
        for (i = 0; i < 7; i++)
        {
            printf("%d ", dado->msg[i]);
        }
        printf("%d\n", (int)dado->timestamp);
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