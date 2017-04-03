#include "dados.h"
#include <semaphore.h>
#include <time.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG

//Função privada para adicioar elemento no final da fila
void adicionaFim(Dados *novo, FilaDados *fila);

/**
 *  Inicia todos os recursos da fila
 */
extern FilaDados *iniciaFila()
{
    FilaDados *fila;
    fila = (FilaDados *)malloc(sizeof(FilaDados));
    fila->head = NULL;
    fila->tail = NULL;
    fila->quantidade = 0;
    if (sem_init(&fila->semaforo, 0, 1) == -1)
    {
#if defined(DEBUG)
        printf("Erro no semáforo!");
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
extern bool insereDados(unsigned int _idRede, unsigned int _tipoGrandeza, unsigned int _grandeza, float _valor, FilaDados *fila)
{
    /*TODO: Validar os Dados antes de inserir na fila*/
    sem_wait(&fila->semaforo);

    //cria nova estrutura
    Dados *novo = (Dados *)malloc(sizeof(Dados));
    if (!novo)
    {
        sem_post(&fila->semaforo);
        return false;
    }
    novo->idRede = _idRede;
    novo->tipoGrandeza = _tipoGrandeza;
    novo->grandeza = _grandeza;
    novo->valor = _valor;
    novo->timestamp = time(0);
    novo->next = NULL;
    novo->next = NULL;

    //Adiciona Dados no final da fila
    adicionaFim(novo, fila);
    /* libera semaforo */
    sem_post(&fila->semaforo);
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

extern Dados *peekDados(FilaDados *fila)
{
    sem_wait(&fila->semaforo);

    //pega o primeiro nó da fila
    Dados *tmp = fila->head;

    /*Libera semaforo*/
    sem_post(&fila->semaforo);

    //retorna o nó
    return tmp;
}
/**
 * Remove item na posicao desejada
 */
extern bool removeDoInicio(Dados *dado, FilaDados *fila)
{
    sem_wait(&fila->semaforo);
    
    if(dado == fila->head){
        fila->head = dado->next;
    }
    else{
        #if defined(DEBUG)
            printf("Não é o primeiro nó!");
        #endif
        sem_post(&fila->semaforo);
        return false;
    }
    
    dado->next == NULL;
    free(dado);
    
    fila->quantidade--;
    /*Libera semaforo*/
    sem_post(&fila->semaforo);
    return true;
}

extern void imprimeFilaDados(FilaDados *fila)
{
    sem_wait(&fila->semaforo);

    if (fila->head == NULL)
    {
        /*Libera semaforo*/
        sem_post(&fila->semaforo);
        return;
    }

    Dados *tmp = fila->head;
    printf("\nFila de DADOS:\n");
    while (tmp != NULL)
    {
        mostraDados(tmp);
        tmp = tmp->next;
    }
    printf("\n");
    /*Libera semaforo*/
    sem_post(&fila->semaforo);
}

extern void mostraDados(Dados *dado)
{
    printf("\n--------[ %p ]---------\n",dado);
    printf("idRede: %d\n", dado->idRede);
    printf("tipoGrandeza: %d\n", dado->tipoGrandeza);
    printf("grandeza: %d\n", dado->grandeza);
    printf("valor: %f\n", dado->valor);
    printf("timestamp: %d\n", (int)dado->timestamp);
}

extern void libera(FilaDados *fila)
{
    if (fila->head != NULL)
    {
        Dados *proxDado, *atual;
        atual = fila->head;
        while (atual != NULL)
        {
            proxDado = atual->next;
            free(atual);
            atual = proxDado;
        }
    }
}