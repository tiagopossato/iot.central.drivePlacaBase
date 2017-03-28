#include "dados.h"
#include <semaphore.h>
#include <time.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG

sem_t semaforo;
bool iniciado = false;

//Função privada para adicioar elemento no final da fila
void adicionaFim(Dados *novo, FilaDados *fila);

/**
 *  Inicia todos os recursos da fila
 */
FilaDados *iniciaFila(){
    if (sem_init(&semaforo, 0, 1) == -1)
    {
        #if defined(DEBUG)
            printf("Erro no semáforo!");
        #endif
        return false;
    }
    FilaDados *fila;
    fila = (FilaDados *)malloc(sizeof(FilaDados));
    fila->head = NULL;
    fila->quantidade = 0;

    if(fila == NULL){
        #if defined(DEBUG)
            printf("Erro ao criar fila de dados!");
        #endif
        
        return false;
    }
    iniciado = true;
    return fila;
    
}

/*
Cria uma estrutura do tipo Dados com os parâmetros recebidos
e insere na fila de Dados
*/
bool insereDados(unsigned int _idRede, unsigned char _tipoGrandeza, unsigned int _grandeza, float _valor, FilaDados *fila)
{
    /*TODO: Validar os Dados antes de inserir na fila*/
    sem_wait(&semaforo);
    
    //cria nova estrutura
    Dados *novo = (Dados *)malloc(sizeof(Dados));
    if (!novo)
    {
        return false;
    }
    novo->idRede = _idRede;
    novo->tipoGrandeza = _tipoGrandeza;
    novo->grandeza = _grandeza;
    novo->valor = _valor;
    novo->timestamp = time(0);
    novo->prox = NULL;
    
    //Adiciona Dados no final da fila
    adicionaFim(novo,fila);
    /* libera semaforo */
    sem_post(&semaforo);
    return true;
}


/**
 * Adiciona novos Dados no final da fila
 * @param novo Estrutura do tipo Dados
 */
void adicionaFim(Dados *novo, FilaDados *fila)
{
    printf("Tamanho da fila: %d\n", fila->quantidade);
    //verifica se a fila esta vazia
    if (fila->head == NULL)
    {
        //caso estiver, aponta para a estrutura criada
        fila->head = novo;
    }
    else
    {
        //caso nao estiver vazia
        //cria um nó temporario, apontando para o primeiro elemento da fila
        Dados *tmp = fila->head;
        //percorre a fila ate encontrar o ultimo elemento
        while (tmp->prox != NULL)
        {
            tmp = tmp->prox;
        }
        //insere o elemento novo no final da fila
        tmp->prox = novo;
    }
    fila->quantidade++;
}

Dados *peekDados(FilaDados *fila){
    if (!iniciado)
    {
        iniciaFila(fila);
    }
    sem_wait(&semaforo);
    
    //pega o primeiro nó da fila
    Dados *tmp = fila->head;

    /*Libera semaforo*/
    sem_post(&semaforo);
    
    //retorna o nó
    return tmp;
}
/**
 * Remove item na posicao desejada
 */
bool removeDados(Dados *dado, FilaDados *fila){
    
#if defined(DEBUG)
  printf("Removendo no\n");
#endif
    
    //cria nova struct e aponta para o primeiro valor da fila
    Dados *anterior = fila->head,
          //cria nova struct e aponta para o segundo valor
        *proxima = anterior->prox;

    //caso for o primeiro elemento
    if (dado == anterior)
    {
        fila->head = proxima;
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

    fila->quantidade--;
    return true;
}

void imprimeFilaDados(FilaDados *fila)
{
    sem_wait(&semaforo);

    if (fila->head == NULL)
    {
        /*Libera semaforo*/
        sem_post(&semaforo);
        return;
    }

    Dados *tmp = fila->head;
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
        printf("idRede: %d\n", dado->idRede);
        printf("tipoGrandeza: %d\n", dado->tipoGrandeza);
        printf("grandeza: %d\n", dado->grandeza);
        printf("valor: %f\n", dado->valor);
        printf("timestamp: %d\n\n", (int)dado->timestamp);
}

void libera(FilaDados *fila)
{
    if (fila->head != NULL)
    {
        Dados *proxDado, *atual;
        atual = fila->head;
        while (atual != NULL)
        {
            proxDado = atual->prox;
            free(atual);
            atual = proxDado;
        }
    }
}