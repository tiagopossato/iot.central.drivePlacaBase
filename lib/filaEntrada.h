#ifndef FILAENTRADA_H_
#define FILAENTRADA_H_

#ifdef	__cplusplus
extern "C" {
#endif

#include "filaSaida.h"
#include <pthread.h>
#include <stdbool.h>

typedef struct NoEntrada
{
    unsigned int idRede;
    unsigned int tipoGrandeza;                                                                                                                                                     
    unsigned int grandeza;
    float valor;
    time_t timestamp;
    struct Entrada *prev;
    struct Entrada *next;
}Entrada;

//typedef struct No Entrada;

typedef struct sFilaDados
{
    Entrada *head;
    Entrada *tail;
    int quantidade;
    pthread_mutex_t mutex;
} FilaEntrada;

/*
 * Todas as funcoes publicas devem ser protegidas pelo semaforo
 * de controle de acesso aos Dados
 */
//extern bool insereDadosEntrada(unsigned int _idRede, unsigned int _tipoGrandeza, unsigned int _grandeza, float _valor, FilaEntrada *fila);
extern bool insereDadosEntrada(char *uri, FilaEntrada *filaEntrada, FilaSaida *filaSaida);
extern Entrada *peekDados(FilaEntrada *fila);
extern bool removeDoInicio(FilaEntrada *fila);
extern void imprimeFilaDados(FilaEntrada *fila);
extern void mostraNoEntrada(Entrada *dado);
extern void liberaFilaEntrada(FilaEntrada *fila);
extern FilaEntrada *iniciaFila();

#ifdef	__cplusplus
}
#endif
#endif /* FILAENTRADA_H_ */