#ifndef DADOS_H_
#define DADOS_H_

#include <semaphore.h>
#include <time.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

sem_t semaforo;

typedef struct sDados
{
    unsigned int id;
    unsigned char codigo;
    unsigned char msg[7];
    time_t timestamp;
    struct Dados *prox;
}Dados;

/*
 * Todas as funcoes publicas devem ser protegidas pelo semaforo
 * de controle de acesso aos Dados
 */
bool insereDados(unsigned int _id, unsigned char _codigo, unsigned char *_msg);
Dados *popDados();
bool removeDados(Dados *dado);
void imprimeListaDados();
//gravacao de Dados offline
void gravaDados();

#endif /* DADOS_H_ */