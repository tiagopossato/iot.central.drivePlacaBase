#include "filaSaida.h"
#include "util.h"
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG

//Função privada para adicionar elemento no final da fila
void adicionaFimSaida(Saida *novo, FilaSaida *fila);

/**
 *  Inicia todos os recursos da fila
 */
extern FilaSaida *iniciaFilaSaida()
{
    FilaSaida *fila;
    fila = (FilaSaida *)malloc(sizeof(FilaSaida));
    fila->head = NULL;
    fila->tail = NULL;
    fila->quantidade = 0;
    if (pthread_mutex_init(&fila->mutex, NULL) == -1)
    {
        logMessage("FLSAIDA", "Erro no mutex!", true);
        return NULL;
    }

    if (fila == NULL)
    {
        logMessage("FLSAIDA", "Erro ao criar fila de dados!", true);
    }
    return fila;
}

/*
Cria uma estrutura do tipo Saida com os parâmetros recebidos
e insere na fila de Saida
*/
extern bool insereDadosSaida(char *uri, FilaSaida *fila)
{
    char msgTmp[128];
    /*TODO: Validar os Dados antes de inserir na fila*/
    char tmp[64];
    memset(tmp, '\0', sizeof(tmp));
    //verifica o início e final da string de dados
    int inicio = buscaCaracter(uri, '[');
    int fim = buscaCaracter(uri, ']');
    //validação 1 da entrada
    if (inicio < 0 || fim <= inicio)
    {
        return false;
    }
    // printf("inicio: %d, fim: %d\n", inicio, fim);
    //extração 1 dos dados
    memcpy(tmp, &uri[inicio + 1], fim - (inicio + 1));

    // printf("Antes de extrair idRede: %s\n", tmp);
    unsigned int idRede = (unsigned int)extraiParte(tmp);
    // printf("Antes de extrair tipoGrandeza: %s\n", tmp);
    unsigned int tipoGrandeza = (unsigned int)extraiParte(tmp);
    // printf("Antes de extrair grandeza: %s\n", tmp);
    unsigned int grandeza = (unsigned int)extraiParte(tmp);
    // printf("Antes de extrair valor: %s\n", tmp);
    float valor = extraiParte(tmp);
    // printf("Valor: %f\n", valor);

    pthread_mutex_lock(&fila->mutex);
    Saida *novo = buscaNoSaida(idRede, tipoGrandeza, grandeza, fila);

    if (novo == NULL)
    {
        //cria nova estrutura
        novo = (Saida *)malloc(sizeof(Saida));
        if (!novo)
        {
            printf("Não pode alocar\n");
            pthread_mutex_unlock(&fila->mutex);
            return false;
        }
        novo->idRede = idRede;
        novo->tipoGrandeza = tipoGrandeza;
        novo->grandeza = grandeza;
        novo->valor = valor;
        novo->ttl = maxTTL;
        novo->prev = NULL;
        novo->next = NULL;

        //VALIDA TIPO DA GRANDEZA
        if (!validaTipoGrandeza(novo->tipoGrandeza))
        {
            sprintf(msgTmp, "Tipo de grandeza: %d não reconhecido!", novo->tipoGrandeza);
            logMessage("FLENT", msgTmp, true);
            free(novo);
            pthread_mutex_unlock(&fila->mutex);
            return false;
        }
        //VALIDA GRANDEZA
        if (!validaGrandeza(novo->grandeza, novo->tipoGrandeza))
        {
            sprintf(msgTmp, "Grandeza: %d não reconhecida!", novo->grandeza);
            logMessage("FLENT", msgTmp, true);
            free(novo);
            pthread_mutex_unlock(&fila->mutex);
            return false;
        }

        //Adiciona Dados no final da fila
        adicionaFimSaida(novo, fila);
    }
    else
    {
        novo->valor = valor;
        novo->ttl = maxTTL;
    }
    /* libera mutex */
    pthread_mutex_unlock(&fila->mutex);
    return true;
}

/**
 * Adiciona novos Dados no final da fila 
 */
void adicionaFimSaida(Saida *novo, FilaSaida *fila)
{
    //verifica se a fila esta vazia
    if (fila->head == NULL)
    {
        //caso estiver, aponta para a estrutura criada
        fila->head = (Saida *)novo;
        fila->tail = (Saida *)novo;
    }
    else
    {
        /*caso nao estiver vazia insere na fila*/
        //Coloca fim da fila no prev do nó
        novo->prev = fila->tail;
        //aponta next do fim da fila para o nó
        fila->tail->next = novo;
        //aponta fim da fila para o no
        fila->tail = novo;
    }
    fila->quantidade++;
    printf("\n\n\n\n\n\n\nNa inserção\n");
    mostraNoSaida(novo);
    printf("\n\n\n\n\n\n\n\n");
}

extern Saida *peekNoSaida(FilaSaida *fila)
{
    pthread_mutex_lock(&fila->mutex);

    //pega o primeiro nó da fila
    Saida *tmp = fila->head;

    /*Libera mutex*/
    pthread_mutex_unlock(&fila->mutex);

    //retorna o nó
    return tmp;
}
/**
* Busca nó pelos dados do cabeçalho
*/

extern Saida *buscaNoSaida(unsigned int idRede, unsigned int tipoGrandeza, unsigned int grandeza, FilaSaida *fila)
{
    //printf("Buscando Nó na saida: %d/%d/%d\n", idRede, tipoGrandeza, grandeza);
    Saida *no = fila->head;
    while (no != NULL)
    {
        if (no->idRede == idRede && no->tipoGrandeza == tipoGrandeza && no->grandeza == grandeza)
        {
            return no;
        }
        no = (Saida *)no->next;
    }
    return NULL;
}
/**
 * Remove item na posicao desejada
 */
extern bool apagaNoSaida(Saida *no, FilaSaida *fila)
{
    if (no == NULL)
        return true;

    pthread_mutex_lock(&fila->mutex);
    Saida *tmp;
    printf("Apagando nó: %p \n", no);
    mostraNoSaida(no);
    
    /*Se for o 1º elemento da fila*/
    if (no == fila->head)
    { //1
        printf("No HEAD\n");
        fila->head = (Saida *)no->next;
        if (fila->head == NULL)
        {
            fila->tail = NULL;
        }
        else
        {
            fila->head->prev = NULL;
        }
    } //1
    else
    { //2
        /*se for o último elemento da lista*/
        if (no == fila->tail)
        { //3
            printf("No TAIL\n");
            fila->tail = (Saida *)no->prev;
            fila->tail->next = NULL;
            //Saida *tail = (Saida *)fila->tail->prev;
            //tail->next = NULL;
            //fila->tail = (Saida *)fila->tail->prev;
        } //3
        else
        { //4
            /*Se for um elemento no meio da fila*/
            if (no != fila->tail && no != fila->head)
            { //5
                printf("No meio\n");
                // tmp = fila->head;
                // while (no != tmp && tmp != NULL)
                // {
                //     tmp = (Saida *)tmp->next;
                // }
                Saida *prev = (Saida *)no->prev;
                Saida *next = (Saida *)no->next;
                prev->next = next;
                next->prev = prev;
            } //5
        }     //4
    }         //2
    no->next = NULL;
    no->prev = NULL;
    no->idRede = 0;
    no->grandeza = 0;
    no->tipoGrandeza = 0;
    free(no);
    fila->quantidade--;
    /*Libera mutex*/
    pthread_mutex_unlock(&fila->mutex);
    imprimeFilaSaida(fila);
    return true;
}

extern void imprimeFilaSaida(FilaSaida *fila)
{
    //pthread_mutex_lock(&fila->mutex);
    if (fila->head == NULL)
    {
        /*Libera mutex*/
        pthread_mutex_unlock(&fila->mutex);
        return;
    }

    printf("\nFila de SAIDA:\n");
    printf("Fila->head: %p, Fila->tail: %p\n", fila->head, fila->tail);

    Saida *tmp = fila->head;
    while (tmp != NULL)
    {
        mostraNoSaida(tmp);
        if (tmp == fila->tail && fila->tail->next != NULL)
        {
            printf("Fila Tail não é NULL!\n");
            exit(-1);
        }
        tmp = (Saida *)tmp->next;
    }
    printf("\n");
    /*Libera mutex*/
    //pthread_mutex_unlock(&fila->mutex);
}

extern void mostraNoSaida(Saida *no)
{
    printf("\n--------[ %p ]---------\n", no);
    if (no == NULL)
        return;
    printf("idRede: %d\n", no->idRede);
    printf("tipoGrandeza: %d\n", no->tipoGrandeza);
    printf("grandeza: %d\n", no->grandeza);
    printf("valor: %f\n", no->valor);
    printf("ttl: %d\n", no->ttl);
    printf("prev: %p\n", no->prev);
    printf("next: %p\n", no->next);

    /*TESTES*/
    if ((Saida *)no->next == no || (Saida *)no->prev == no || ((Saida *)no->next == (Saida *)no->prev && (Saida *)no->next != NULL))
    {
        printf("Nó com problema!\n");
        exit(-1);
    }
    /*FIM DOS TESTES*/
}

extern void liberaFilaSaida(FilaSaida *fila)
{
    if (fila->head != NULL)
    {
        Saida *proxDado, *atual;
        atual = fila->head;
        while (atual != NULL)
        {
            proxDado = (Saida *)atual->next;
            free(atual);
            atual = proxDado;
        }
    }
}