#ifndef DADOS_H_
#define DADOS_H_

#include <semaphore.h>
#include <time.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct No
{
    unsigned int idRede;
    unsigned int tipoGrandeza;                                                                                                                                                     
    unsigned int grandeza;
    float valor;
    time_t timestamp;
    struct Dados *prox;
};

typedef struct No Dados;

typedef struct sFilaDados
{
    Dados *head;
    int quantidade;
} FilaDados;

/*
 * Todas as funcoes publicas devem ser protegidas pelo semaforo
 * de controle de acesso aos Dados
 */
bool insereDados(unsigned int _idRede, unsigned int _tipoGrandeza, unsigned int _grandeza, float _valor, FilaDados *fila);
Dados *peekDados(FilaDados *fila);
bool removeDados(Dados *dado, FilaDados *fila);
void imprimeFilaDados(FilaDados *fila);
void mostraDados(Dados *dado);
void libera(FilaDados *fila);
FilaDados *iniciaFila();

#endif /* DADOS_H_ */