#ifndef FILASAIDA_H_
#define FILASAIDA_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <pthread.h>

#define TTL_SAIDA 5

typedef struct NoSaida
{
    unsigned int idRede;
    unsigned int tipoGrandeza;
    unsigned int grandeza;
    float valor;
    int ttl;
    struct Saida *prev;
    struct Saida *next;
} Saida;

typedef struct sFilaSaida
{
    Saida *head;
    Saida *tail;
    int quantidade;
    pthread_mutex_t mutex;
} FilaSaida;

/*
 * Todas as funcoes publicas devem ser protegidas pelo semaforo
 * de controle de acesso aos Dados
 */
extern bool insereDadosSaida(char *uri, FilaSaida *fila);
extern Saida *peekNoSaida(FilaSaida *fila);
extern Saida *buscaNoSaida(unsigned int idRede,
                           unsigned int tipoGrandeza,
                           unsigned int grandeza,
                           FilaSaida *fila);
extern bool apagaNoSaida(Saida *no, FilaSaida *fila);
extern void imprimeFilaSaida(FilaSaida *fila);
extern void mostraNoSaida(Saida *no);
extern void liberaFilaSaida(FilaSaida *fila);
extern FilaSaida *iniciaFilaSaida();

#ifdef __cplusplus
}
#endif
#endif /* FILASAIDA_H_ */