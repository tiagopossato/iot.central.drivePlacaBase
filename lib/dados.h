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
      
    time_t timestamp;
    struct Dados *prox;
};

typedef struct No Dados;


/*
 * Todas as funcoes publicas devem ser protegidas pelo semaforo
 * de controle de acesso aos Dados
 */
bool insereDados(unsigned int _idRede, unsigned char _codigo, unsigned char *_msg);
Dados *peekDados();
bool removeDados(Dados *dado);
void imprimeFilasDados();
void mostraDados(Dados *dado);
//gravacao de Dados offline
void gravaDados();

#endif /* DADOS_H_ */